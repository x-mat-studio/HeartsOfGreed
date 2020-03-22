#include "p2Defs.h"
#include "p2Log.h"
#include "App.h"
#include "PathFinding.h"
#include "Brofiler/Brofiler/Brofiler.h"


ModulePathfinding::ModulePathfinding() : Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name.create("pathfinding");
}

ModulePathfinding::~ModulePathfinding()
{
	RELEASE_ARRAY(map);
}


bool ModulePathfinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.clear();
	RELEASE_ARRAY(map);
	return true;
}


void ModulePathfinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width * height];
	memcpy(map, data, width * height);
}


bool ModulePathfinding::CheckBoundaries(const iMPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}


bool ModulePathfinding::IsWalkable(const iMPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}


uchar ModulePathfinding::GetTileAt(const iMPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[(pos.y * width) + pos.x];

	return INVALID_WALK_CODE;
}


const std::vector <iMPoint>* ModulePathfinding::GetLastPath() const
{
	return &last_path;
}


std::list<PathNode>::pointer PathList::Find(const iMPoint& point)
{
	BROFILER_CATEGORY("Find", Profiler::Color::Bisque);

	for (std::list<PathNode>::iterator it = list.begin(); it != list.end(); it++)
	{
		if (it->pos == point)
			return &*it;
	}

	return NULL;

}


PathNode::PathNode() : g(-1), h(-1), pos{ -1, -1 }, parent(NULL)
{}

PathNode::PathNode(float g, float h, const iMPoint& pos, PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}


uint PathNode::FindWalkableAdjacents(PathList& list_to_fill)
{
	iMPoint cell;
	uint before = list_to_fill.list.size();

	// north
	cell.create(pos.x, pos.y + 1);
	if (app->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x, pos.y - 1);
	if (app->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if (app->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if (app->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));


	cell.create(pos.x + 1, pos.y + 1);
	if (app->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x + 1, pos.y - 1);
	if (app->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x - 1, pos.y + 1);
	if (app->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y - 1);
	if (app->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	return list_to_fill.list.size();
}


float PathNode::Score() const
{
	return g + h;
}


float PathNode::CalculateF(const iMPoint& destination)
{

	g = parent->g + 1;

	h = pos.DistanceTo(destination);

	return g + h;
}


int ModulePathfinding::CreatePath(const iMPoint& origin, const iMPoint& destination)
{
	BROFILER_CATEGORY("Pathfinding", Profiler::Color::Gold);

	if (IsWalkable(origin) == false || IsWalkable(destination) == false) {
		return -1;
	}

	last_path.clear();

	std::multimap<int, PathNode> open;

	PathList closed;
	open.insert(std::pair<int, PathNode>(0, PathNode(0, origin.DistanceTo(destination), origin, NULL)));

	while (open.empty() == false)
	{
		std::multimap<int, PathNode>::iterator lowest = open.begin();
		closed.list.push_back(lowest->second);
		std::list<PathNode>::pointer node = &closed.list.back();

		open.erase(lowest);

		if (node->pos == destination)
		{
			for (PathNode* iterator = node; iterator->pos != origin; iterator = iterator->parent)
			{
				last_path.push_back(iterator->pos);
			}
			last_path.push_back(origin);

			std::reverse(last_path.begin(), last_path.end());

			return 0;
		}

		PathList adjList;
		uint limit = node->FindWalkableAdjacents(adjList);

		std::list<PathNode>::iterator it = adjList.list.begin();
		std::multimap<int, PathNode>::iterator it2;

		for (uint i = 0; i < limit; i++)
		{
			if (closed.Find(it->pos) == NULL)
			{
				it2 = Find(it->pos, open);
				if (it2 == open.end())
				{
					it->CalculateF(destination);
					open.insert(std::pair<int, PathNode>(it->Score(), *it));
				}
				else
				{
					it->CalculateF(destination);
					if (it->g < it2->second.g)
					{
						open.erase(it2);
						open.insert(std::pair<int, PathNode>(it->Score(), *it));
					}
				}
			}
			++it;
		}
	}
}

void ModulePathfinding::SavePath(std::vector <iMPoint>* path)
{
	const std::vector <iMPoint>* last_path = app->pathfinding->GetLastPath();
	path->clear();

	for (uint i = 0; i < last_path->size(); ++i)
	{
		path->push_back({ (last_path->begin() + i)->x, (last_path->begin() + i)->y });
	}

}

std::multimap<int, PathNode>::iterator ModulePathfinding::Find(iMPoint point, std::multimap<int, PathNode>& map)
{
	for (std::multimap<int, PathNode>::iterator iterator = map.begin(); iterator != map.end(); iterator++)
	{
		if (iterator->second.pos == point)
		{
			return iterator;
		}
	}

	return map.end();
}