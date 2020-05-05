#include "p2Defs.h"
#include "p2Log.h"
#include "App.h"
#include "PathFinding.h"
#include "Brofiler/Brofiler/Brofiler.h"

ModulePathfinding::ModulePathfinding() : Module(), walkabilityMap(NULL), last_path(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name.create("pathfinding");
}

ModulePathfinding::~ModulePathfinding()
{
	RELEASE_ARRAY(walkabilityMap);
}

bool ModulePathfinding::LineRayCast(iMPoint& p0, iMPoint& p1)
{
	BROFILER_CATEGORY("RayCast", Profiler::Color::Cornsilk);

	std::vector <iMPoint> line = CreateLine(p0, p1);

	bool walkableFound = false;
	bool currWalkability = false;

	for (int i = 0; i < line.size(); i++)
	{
		currWalkability = IsWalkable(line[i]);

		if (currWalkability && !walkableFound)
			walkableFound = true;
		else if (!currWalkability && walkableFound)
			return false;
	}


	return true;
}

std::vector<iMPoint> ModulePathfinding::CreateLine(const iMPoint& p0, const iMPoint& p1, int maxDistance)
{
	last_line.clear();

	float n = p0.DiagonalDistance(p1);

	fMPoint p0f = { (float)p0.x, (float)p0.y };
	fMPoint p1f = { (float)p1.x, (float)p1.y };

	for (int step = 0; step <= n && step < maxDistance; step++)
	{
		float t = n == 0 ? 0.0 : step / n;

		fMPoint nextPointf = p0f.LerpPoint(p1f, t);
		nextPointf.RoundPoint();

		iMPoint nextPoint = { (int)nextPointf.x, (int)nextPointf.y };
		last_line.push_back(nextPoint);

	}

	return last_line;
}

bool ModulePathfinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	//A*-----------------------------------------
	last_path.clear();
	last_line.clear();
	RELEASE_ARRAY(walkabilityMap);

	//HPA-----------------------------------------
	absGraph.entrances.clear();

	for (int i = 0; i < absGraph.lvlClusters.size(); i++)
	{
		for (int j = 0; j < absGraph.lvlClusters[i].size(); j++)
		{
			for (int k = 0; k < absGraph.lvlClusters[i].at(j).clustNodes.size(); k++)
			{
				for (int z = 0; z < absGraph.lvlClusters[i].at(j).clustNodes.at(k)->edges.size(); z++)
				{
					delete absGraph.lvlClusters[i].at(j).clustNodes.at(k)->edges.at(z);
					absGraph.lvlClusters[i].at(j).clustNodes.at(k)->edges.at(z) = nullptr;
				}
				absGraph.lvlClusters[i].at(j).clustNodes.at(k)->edges.clear();

				delete absGraph.lvlClusters[i].at(j).clustNodes.at(k);
				absGraph.lvlClusters[i].at(j).clustNodes.at(k) = nullptr;
			}
			absGraph.lvlClusters[i].at(j).clustNodes.clear();
		}
		absGraph.lvlClusters[i].clear();
	}
	absGraph.lvlClusters.clear();

	generatedPaths.clear();
	return true;
}


void ModulePathfinding::SetMap(uint width, uint height, uchar* data)
{
	//Basic A*----------------------------------------
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(walkabilityMap);
	walkabilityMap = new uchar[width * height];
	memcpy(walkabilityMap, data, width * height);

	//HPA*--------------------------------------------
	preProcessing(MAX_LEVELS);

}

void ModulePathfinding::preProcessing(int maxLevel)
{
	BROFILER_CATEGORY("Pre-processing", Profiler::Color::Cornsilk);
	abstractMaze();
	buildGraph();
	//for (int l = 2; l <= maxLevel; l++)
	//{
	//	addLevelToGraph(l);
	//}
}

void ModulePathfinding::abstractMaze()
{
	absGraph.entrances.clear();

	absGraph.buildClusters(1);
	absGraph.buildEntrances(0);
}

void ModulePathfinding::buildGraph()
{
	Entrance* currEntrance;
	Cluster* c1, * c2;
	HierNode* n1, * n2;

	for (uint i = 0; i < absGraph.entrances.size(); i++)
	{
		currEntrance = &absGraph.entrances[i];
		c1 = currEntrance->from;
		c2 = currEntrance->to;

		switch (currEntrance->dir)
		{
		case ADJACENT_DIR::LATERAL:
		{
			for (int i = currEntrance->pos.y; i < (currEntrance->pos.y + currEntrance->height); i += NODE_MIN_DISTANCE)
			{
				n1 = absGraph.NodeExists({ currEntrance->pos.x,i }, c1);
				if (!n1)
				{
					n1 = new HierNode({ currEntrance->pos.x, i });
					c1->clustNodes.push_back(n1);
				}


				n2 = absGraph.NodeExists({ currEntrance->pos.x + 1, i }, c2);
				if (!n2)
				{
					n2 = new HierNode({ currEntrance->pos.x + 1, i });
					c2->clustNodes.push_back(n2);
				}


				n1->edges.push_back(new Edge(n2, 1, EDGE_TYPE::TP_INTER));
				n2->edges.push_back(new Edge(n1, 1, EDGE_TYPE::TP_INTER));
			}
		}
		break;
		case ADJACENT_DIR::VERTICAL:
		{
			for (int i = currEntrance->pos.x; i < (currEntrance->pos.x + currEntrance->width); i += NODE_MIN_DISTANCE)
			{

				n1 = absGraph.NodeExists({ i, currEntrance->pos.y }, c1);
				if (!n1)
				{
					n1 = new HierNode({ i, currEntrance->pos.y });
					c1->clustNodes.push_back(n1);
				}


				n2 = absGraph.NodeExists({ i, currEntrance->pos.y + 1 }, c2);
				if (!n2)
				{
					n2 = new HierNode({ i, currEntrance->pos.y + 1 });
					c2->clustNodes.push_back(n2);
				}

				n1->edges.push_back(new Edge(n2, 1, EDGE_TYPE::TP_INTER));
				n2->edges.push_back(new Edge(n1, 1, EDGE_TYPE::TP_INTER));
			}
		}
		break;
		}

	}

	Cluster* clusterIt;
	float distanceTo = 0;

	for (int i = 0; i < absGraph.lvlClusters[0].size(); i++)
	{
		clusterIt = &absGraph.lvlClusters[0].at(i);

		for (int y = 0; y < clusterIt->clustNodes.size(); y++)
		{
			for (int k = y + 1; k < clusterIt->clustNodes.size(); k++)
			{
				distanceTo = app->pathfinding->SimpleAPathfinding(clusterIt->clustNodes[y]->pos, clusterIt->clustNodes[k]->pos, CLUSTER_SIZE_LVL * 2);

				if (distanceTo != -1.f)
				{
					clusterIt->clustNodes[y]->edges.push_back(new Edge(clusterIt->clustNodes[k], distanceTo, EDGE_TYPE::TP_INTRA));
					clusterIt->clustNodes[k]->edges.push_back(new Edge(clusterIt->clustNodes[y], distanceTo, EDGE_TYPE::TP_INTRA));
				}

			}
		}
	}
}

HierNode* graphLevel::NodeExists(iMPoint pos, Cluster* c)
{
	for (int i = 0; i < c->clustNodes.size(); i++)
	{
		if (pos == c->clustNodes[i]->pos)
			return c->clustNodes[i];
	}

	return nullptr;
}

void graphLevel::buildEntrances(int lvl)
{
	Cluster* c1;
	Cluster* c2;
	ADJACENT_DIR adjacentDir = ADJACENT_DIR::DIR_NONE;

	for (uint i = 0; i < this->lvlClusters[lvl].size(); ++i)
	{
		c1 = &this->lvlClusters[lvl].at(i);

		for (uint k = i + 1; k < this->lvlClusters[lvl].size(); ++k)
		{
			c2 = &this->lvlClusters[lvl].at(k);

			adjacentDir = adjacents(c1, c2, lvl + 1);

			if (adjacentDir != ADJACENT_DIR::DIR_NONE)
				createEntrance(c1, c2, adjacentDir, lvl);
		}
	}
}

void graphLevel::createEntrance(Cluster* c1, Cluster* c2, ADJACENT_DIR adjDir, int lvl)
{
	int adjDist = CLUSTER_SIZE_LVL * lvl;

	Entrance newEntrance;
	int startedAt = INT_MIN;
	bool isCurrentWalkable = true;

	switch (adjDir)
	{
	case ADJACENT_DIR::VERTICAL:
	{
		for (int i = c1->pos.x; i < (c1->pos.x + c1->width); i++)
		{
			isCurrentWalkable = (app->pathfinding->IsWalkable({ i, c1->pos.y + c1->height - 1 }) && app->pathfinding->IsWalkable({ i, c2->pos.y }));

			if (isCurrentWalkable)
			{
				if (startedAt == INT_MIN)
					startedAt = i;

				if (i == (c1->pos.x + c1->width - 1))
					this->entrances.push_back(Entrance({ startedAt, c1->pos.y + c1->height - 1 }, c1->width - startedAt + c1->pos.x, 2, adjDir, c1, c2));

			}
			else if (!isCurrentWalkable)
			{
				if (startedAt != INT_MIN)
				{
					this->entrances.push_back(Entrance({ startedAt, c1->pos.y + c1->height - 1 }, i - startedAt, 2, adjDir, c1, c2));
					startedAt = INT_MIN;
				}
			}
		}
	}
	break;

	case ADJACENT_DIR::LATERAL:
	{
		for (int i = c1->pos.y; i < (c1->pos.y + c1->height); i++)
		{
			isCurrentWalkable = (app->pathfinding->IsWalkable({ c1->pos.x + c1->width - 1,i }) && app->pathfinding->IsWalkable({ c2->pos.x, i }));

			if (isCurrentWalkable)
			{
				if (startedAt == INT_MIN)
					startedAt = i;

				if (i == (c1->pos.y + c1->height - 1))
					this->entrances.push_back(Entrance({ c1->pos.x + c1->width - 1,  startedAt }, 2, c1->height - startedAt + c1->pos.y, adjDir, c1, c2));
			}
			else if (!isCurrentWalkable)
			{
				if (startedAt != INT_MIN)
				{
					this->entrances.push_back(Entrance({ c1->pos.x + c1->width - 1, startedAt }, 2, i - startedAt, adjDir, c1, c2));
					startedAt = INT_MIN;
				}
			}
		}
	}
	break;

	}
}

ADJACENT_DIR graphLevel::adjacents(Cluster* c1, Cluster* c2, int lvl)
{
	int adjDist = CLUSTER_SIZE_LVL * lvl;

	if (c1->pos.x + adjDist == c2->pos.x && c1->pos.y == c2->pos.y)
		return ADJACENT_DIR::LATERAL;

	else if (c1->pos.y + adjDist == c2->pos.y && c1->pos.x == c2->pos.x)
		return ADJACENT_DIR::VERTICAL;

	return ADJACENT_DIR::DIR_NONE;
}

void graphLevel::buildClusters(int lvl)
{
	int clustSize = CLUSTER_SIZE_LVL * lvl;

	std::vector <Cluster> clusterVector;
	this->lvlClusters.push_back(clusterVector);

	int width = app->pathfinding->width;
	int height = app->pathfinding->height;

	Cluster c;

	for (int i = 0; i < width; i += clustSize)
	{
		if (i + clustSize > width)
			c.width = width - (i);
		else
			c.width = clustSize;

		for (int k = 0; k < height; k += clustSize)
		{
			if (k + clustSize > height)
				c.height = height - (k);
			else
				c.height = clustSize;

			c.pos = { i,k };
			lvlClusters[lvl - 1].push_back(Cluster(c));
		}
	}

}

HierNode* graphLevel::insertNode(iMPoint pos, int maxLvl, bool* toDelete)
{
	BROFILER_CATEGORY("Insert Node", Profiler::Color::Cornsilk);

	HierNode* newNode = nullptr;
	Cluster* c;

	for (int l = 1; l <= maxLvl; l++)
	{
		if (l > this->lvlClusters.size())
			break;

		c = determineCluster(pos, l);
		if (!c)
			return nullptr;

		newNode = NodeExists(pos, c);

		if (!newNode)
		{
			newNode = new HierNode(pos);
			c->clustNodes.push_back(newNode);
			ConnectNodeToBorder(newNode, c, l);
			*toDelete = true;
		}
	}

	return newNode;
}

void graphLevel::ConnectNodeToBorder(HierNode* node, Cluster* c, int lvl)
{
	float distanceTo = 0;
	for (int i = 0; i < c->clustNodes.size(); i++)
	{
		distanceTo = app->pathfinding->SimpleAPathfinding(node->pos, c->clustNodes[i]->pos, lvl * CLUSTER_SIZE_LVL);
		//distanceTo = node->pos.DistanceTo(c->clustNodes[i]->pos);

		if (distanceTo != -1.f)
		{
			node->edges.push_back(new Edge(c->clustNodes[i], distanceTo, EDGE_TYPE::TP_INTRA));
			c->clustNodes[i]->edges.push_back(new Edge(node, distanceTo, EDGE_TYPE::TP_INTRA));
		}
	}

}

Cluster* graphLevel::determineCluster(iMPoint pos, int lvl, Cluster* firstCheck)
{
	BROFILER_CATEGORY("Determine Cluster", Profiler::Color::DeepPink);

	if (firstCheck)
	{
		if (pos.x >= firstCheck->pos.x && pos.y >= firstCheck->pos.y &&
			pos.x < firstCheck->pos.x + firstCheck->width && pos.y < firstCheck->pos.y + firstCheck->height)
			return firstCheck;
	}

	Cluster* it;
	for (int i = 0; i < lvlClusters[lvl - 1].size(); i++)
	{
		it = &lvlClusters[lvl - 1].at(i);

		if (pos.x >= it->pos.x && pos.y >= it->pos.y &&
			pos.x < it->pos.x + it->width && pos.y < it->pos.y + it->height)
			return it;
	}

	return nullptr;
}

void graphLevel::deleteNode(HierNode* toDelete, int maxLvl)
{
	BROFILER_CATEGORY("Delete Node", Profiler::Color::DarkViolet);

	if (toDelete == nullptr || maxLvl < 1)
		return;

	Cluster* c = nullptr;

	for (int l = 1; l <= maxLvl; l++)
	{
		if (l > this->lvlClusters.size())
			break;

		c = determineCluster(toDelete->pos, l);

		for (int i = 0; i < c->clustNodes.size(); i++)
		{
			for (int j = 0; j < c->clustNodes[i]->edges.size(); j++)
			{
				if (c->clustNodes[i]->edges[j]->dest == toDelete)
				{
					c->clustNodes[i]->edges[j]->dest = nullptr;
					delete c->clustNodes[i]->edges[j];

					c->clustNodes[i]->edges.erase(c->clustNodes[i]->edges.begin() + j);
					break;
				}
			}
		}

		for (int i = 0; i < c->clustNodes.size(); i++)
		{
			if (c->clustNodes[i]->pos == toDelete->pos)
			{
				for (int j = 0; j < toDelete->edges.size(); j++)
				{
					c->clustNodes[i]->edges[j]->dest = nullptr;
					delete c->clustNodes[i]->edges[j];

				}

				c->clustNodes.erase(c->clustNodes.begin() + i);
				toDelete->edges.clear();
				delete toDelete;
				break;
			}
		}


	}
}

Edge::Edge(HierNode* dest, float distanceTo, EDGE_TYPE type) : dest(dest), moveCost(distanceTo), type(type)
{}

Entrance::Entrance() : pos{ 0,0 }, width(0), height(0), dir(ADJACENT_DIR::DIR_NONE), from(nullptr), to(nullptr)
{}

Entrance::Entrance(iMPoint pos, int width, int height, ADJACENT_DIR dir, Cluster* from, Cluster* to)
	: pos(pos), width(width), height(height), dir(dir), from(from), to(to)
{}

HierNode::HierNode(iMPoint pos) : PathNode(-1, -1, pos, nullptr, -1, -1, false)
{}

HierNode::HierNode(iMPoint pos, bool tmp) : PathNode(g, h, pos, nullptr, -1, -1, false)
{}

HierNode::HierNode(float g, const iMPoint& pos, PathNode* parent, int parentdir, int myDir, std::vector<Edge*> edges) :
	PathNode(g, -1, pos, parent, parentdir, myDir, false), edges(edges)
{}

Cluster::Cluster() : pos{ -1,-1 }, width(-1), height(-1)
{}

Cluster::Cluster(int width, int height, iMPoint& pos) :

	width(width), height(height), pos(pos)
{}

Cluster::Cluster(const Cluster& clust) :

	width(clust.width), height(clust.height), pos(clust.pos)
{}

generatedPath::generatedPath(std::vector <iMPoint> vector, PATH_TYPE type, int lvl) : type(type), lvl(lvl)
{
	this->path.swap(vector);
	vector.clear();
}


//---------------------------------------------------

iMPoint ModulePathfinding::CheckNearbyTiles(const iMPoint& origin, const iMPoint& destination)
{

	std::vector <iMPoint> line = CreateLine(origin, destination, NEARBY_TILES_CHECK);

	for (int i = 0; i < line.size(); i++)
	{
		if (IsWalkable(line[i]))
			return line[i];
	}

	return origin;

	/*
	//iMPoint retNeg = destination;
	//iMPoint retPos = destination;

	//iMPoint ret = destination;
	//float currDistance = FLT_MAX;

	//for (int i = 0; i < NEARBY_TILES_CHECK; i++)
	//{
	//	retPos.x++;
	//	retNeg.x--;

	//	retPos.y++;
	//	retNeg.y--;


	//	//Diagonals
	//	if (IsWalkable(retNeg) && retNeg.DistanceNoSqrt(ret) < currDistance)
	//	{
	//		currDistance = retNeg.DistanceNoSqrt(ret);
	//		ret = retNeg;
	//	}

	//	if (IsWalkable(retPos) && retPos.DistanceNoSqrt(ret) < currDistance)
	//	{
	//		currDistance = retPos.DistanceNoSqrt(ret);
	//		ret = retPos;
	//	}

	//	// Y
	//	if (IsWalkable({ 0,retPos.y }) && retPos.DistanceNoSqrt({ 0,retPos.y }) < currDistance)
	//	{
	//		currDistance = retPos.DistanceNoSqrt(ret);
	//		ret = retPos;
	//	}

	//	if (IsWalkable({ 0,retNeg.y }) && retPos.DistanceNoSqrt({ 0,retNeg.y }) < currDistance)
	//	{
	//		currDistance = retPos.DistanceNoSqrt(ret);
	//		ret = retPos;
	//	}


	//	// X
	//	if (IsWalkable({ retPos.x,0 }) && retPos.DistanceNoSqrt({ retPos.x,0 }) < currDistance)
	//	{
	//		currDistance = retPos.DistanceNoSqrt(ret);
	//		ret = retPos;
	//	}

	//	if (IsWalkable({ retNeg.x,0 }) && retPos.DistanceNoSqrt({ retNeg.x,0 }) < currDistance)
	//	{
	//		currDistance = retPos.DistanceNoSqrt(ret);
	//		ret = retPos;
	//	}


	//}
	//	return ret;
	*/
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
	if (CheckBoundaries(pos) && walkabilityMap)
		return walkabilityMap[(pos.y * width) + pos.x];

	return INVALID_WALK_CODE;
}


PathNode::PathNode() : g(-1), h(-1), pos{ -1, -1 }, parent(NULL), is_Diagonal(false), myDirection(0), parentDir(0)
{}

PathNode::PathNode(float g, float h, const iMPoint& pos, PathNode* parent, int parentDir, int myDir, bool isdiagonal) :

	g(g), h(h), pos(pos), parent(parent), parentDir(parentDir), myDirection(myDir), is_Diagonal(isdiagonal)
{}

PathNode::PathNode(const PathNode& node) :

	g(node.g), h(node.h), pos(node.pos), parent(node.parent), parentDir(node.parentDir), myDirection(node.myDirection), is_Diagonal(node.is_Diagonal)
{}

uint PathNode::FindWalkableAdjacents(std::vector<PathNode>& list_to_fill)
{
	iMPoint cell;

	// north
	cell.create(pos.x, pos.y + 1);
	if (app->pathfinding->IsWalkable(cell))
		list_to_fill.push_back(PathNode(-1.f, -1.f, cell, this, myDirection, 0, false));

	// south
	cell.create(pos.x, pos.y - 1);
	if (app->pathfinding->IsWalkable(cell))
		list_to_fill.push_back(PathNode(-1.f, -1.f, cell, this, myDirection, 0, false));

	// east
	cell.create(pos.x + 1, pos.y);
	if (app->pathfinding->IsWalkable(cell))
		list_to_fill.push_back(PathNode(-1.f, -1.f, cell, this, myDirection, 0, false));

	// west
	cell.create(pos.x - 1, pos.y);
	if (app->pathfinding->IsWalkable(cell))
		list_to_fill.push_back(PathNode(-1.f, -1.f, cell, this, myDirection, 0, false));

	cell.create(pos.x + 1, pos.y + 1);
	if (app->pathfinding->IsWalkable(cell))
		list_to_fill.push_back(PathNode(-1.f, -1.f, cell, this, myDirection, 0, true));

	// south
	cell.create(pos.x + 1, pos.y - 1);
	if (app->pathfinding->IsWalkable(cell))
		list_to_fill.push_back(PathNode(-1.f, -1.f, cell, this, myDirection, 0, true));

	// east
	cell.create(pos.x - 1, pos.y + 1);
	if (app->pathfinding->IsWalkable(cell))
		list_to_fill.push_back(PathNode(-1.f, -1.f, cell, this, myDirection, 0, true));

	// west
	cell.create(pos.x - 1, pos.y - 1);
	if (app->pathfinding->IsWalkable(cell))
		list_to_fill.push_back(PathNode(-1.f, -1.f, cell, this, myDirection, 0, true));

	return list_to_fill.size();
}

uint HierNode::FindWalkableAdjacents(std::vector<HierNode>& list_to_fill)
{
	int edgeNum = edges.size();
	HierNode* curr = nullptr;

	for (int i = 0; i < edgeNum; i++)
	{
		curr = edges[i]->dest;
		list_to_fill.push_back(HierNode(edges[i]->moveCost + this->g, curr->pos, this, myDirection, curr->parentDir, curr->edges));
	}

	return list_to_fill.size();
}

float PathNode::Score() const
{
	return g + h;
}


float PathNode::CalculateF(const iMPoint& destination)
{
	if (is_Diagonal)
	{
		g = parent->g + 1.41;
	}
	else
	{
		g = parent->g + 1;
	}

	h = pos.OctileDistance(destination);

	return g + h;
}

float HierNode::CalculateF(const iMPoint& destination)
{
	h = pos.OctileDistance(destination);

	return g + h;
}

PATH_TYPE ModulePathfinding::CreatePath(iMPoint& origin, iMPoint& destination, int maxLvl, Entity* pathRequest)
{
	BROFILER_CATEGORY("Create Path", Profiler::Color::Black);

	PATH_TYPE ret = PATH_TYPE::NO_TYPE;
	HierNode* n1, * n2;
	bool toDeleteN1 = false;
	bool toDeleteN2 = false;

	if (IsWalkable(destination) == false)
	{
		iMPoint newDest = CheckNearbyTiles(destination, origin);

		if (newDest == destination || newDest == origin)
			return ret;
		else
			destination = newDest;

	}

	if (IsWalkable(origin) == false)
	{
		iMPoint newDest = CheckNearbyTiles(origin, destination);

		if (newDest == origin || newDest == destination)
			return ret;
		else
			origin = newDest;
	}


	if (LineRayCast(origin, destination))
	{
		last_path = last_line;
		ret = PATH_TYPE::SIMPLE;
	}
	else
	{

		if (maxLvl > 0)
		{
			n1 = absGraph.insertNode(origin, maxLvl, &toDeleteN1);
			n2 = absGraph.insertNode(destination, maxLvl, &toDeleteN2);

			if (!n1 || !n2)
				return ret;

			n1->h = n1->pos.OctileDistance(n2->pos);


			//Abs Path
			bool pathDone = HPAPathfinding(*n1, n2->pos, 1);

			if (toDeleteN1)
				absGraph.deleteNode((HierNode*)n1, maxLvl);
			if (toDeleteN2)
				absGraph.deleteNode((HierNode*)n2, maxLvl);

			if (pathDone)
				ret = PATH_TYPE::ABSTRACT;
			else
				return PATH_TYPE::NO_TYPE;

		}
		else
		{
			SimpleAPathfinding(origin, destination);

			ret = PATH_TYPE::SIMPLE;
		}
	}

	generatedPaths.erase(pathRequest);
	generatedPaths.insert({ pathRequest, generatedPath(last_path, ret, maxLvl) });


	return ret;
}


int ModulePathfinding::HPAPathfinding(const HierNode& origin, const iMPoint& destination, int lvl)
{
	BROFILER_CATEGORY("HPA Algorithm", Profiler::Color::Black);

	last_path.clear();

	std::multimap<float, HierNode> open;

	std::vector<HierNode> closed;
	open.insert(std::pair<float, HierNode>(0, origin));

	HierNode* curr = nullptr;
	std::multimap<float, HierNode>::iterator lowest;
	std::vector<HierNode> adjList;
	uint limit = 0;
	std::multimap<float, HierNode>::iterator it2;

	uint iterations = 0;

	while (open.empty() == false)
	{
		lowest = open.begin();
		closed.push_back(lowest->second);
		curr = &closed.back();

		curr->myDirection = (closed.size() - 1);
		open.erase(lowest);

		iterations++;


		if (curr->pos == destination)
		{
			int dir;
			for (curr; curr->pos != origin.pos; curr = &closed[dir])
			{
				last_path.push_back(curr->pos);
				dir = curr->parentDir;
			}
			last_path.push_back(origin.pos);

			std::reverse(last_path.begin(), last_path.end());

			return 1;
		}

		limit = curr->FindWalkableAdjacents(adjList);

		for (uint i = 0; i < limit; i++)
		{
			if (FindV(adjList[i].pos, &closed) == closed.size())
			{
				it2 = Find(adjList[i].pos, &open);
				adjList[i].CalculateF(destination);
				if (it2 == open.end())
				{
					open.insert(std::pair<float, HierNode>(adjList[i].Score(), adjList[i]));
				}
				else
				{
					if (adjList[i].Score() < it2->second.Score())
					{
						open.erase(Find(adjList[i].pos, &open));
						open.insert(std::pair<float, HierNode>(adjList[i].Score(), adjList[i]));
					}
				}
			}
		}
		adjList.clear();
	}
	return 0;
}


float ModulePathfinding::SimpleAPathfinding(const iMPoint& origin, const iMPoint& destination, int limitpath)
{
	BROFILER_CATEGORY("A* Algorithm", Profiler::Color::Black);

	last_path.clear();

	std::multimap<float, PathNode> open;

	std::vector<PathNode> closed;
	open.insert(std::pair<float, PathNode>(0, PathNode(0, origin.OctileDistance(destination), origin, nullptr, 0, 0)));

	//Analize the current
	PathNode* curr = nullptr;
	std::multimap<float, PathNode>::iterator lowest;

	// Get New Nodes
	std::vector<PathNode> adjList;
	std::multimap<float, PathNode>::iterator it2;
	uint limit = 0;
	uint loopCounter = 0;

	while (open.empty() == false)
	{
		lowest = open.begin();
		closed.push_back(lowest->second);
		curr = &closed.back();

		curr->myDirection = (closed.size() - 1);
		open.erase(lowest);

		if (limitpath != -1)
		{
			loopCounter++;
			if (loopCounter > limitpath)
				return -1.f;
		}


		if (curr->pos == destination)
		{
			int dir;
			for (curr; curr->pos != origin; curr = &closed[dir])
			{
				last_path.push_back(curr->pos);
				dir = curr->parentDir;
			}
			last_path.push_back(origin);

			std::reverse(last_path.begin(), last_path.end());

			adjList.clear();

			return closed.back().g;
		}

		limit = curr->FindWalkableAdjacents(adjList);

		for (uint i = 0; i < limit; i++)
		{
			if (FindV(adjList[i].pos, &closed) == closed.size())
			{
				it2 = Find(adjList[i].pos, &open);
				adjList[i].CalculateF(destination);
				if (it2 == open.end())
				{
					open.insert(std::pair<float, PathNode>(adjList[i].Score(), adjList[i]));
				}
				else
				{
					if (adjList[i].g < it2->second.g)
					{
						open.erase(Find(adjList[i].pos, &open));
						open.insert(std::pair<float, PathNode>(adjList[i].Score(), adjList[i]));
					}
				}
			}
		}
		adjList.clear();
	}
	return -1.f;
}

bool ModulePathfinding::RequestPath(Entity* request, std::vector <iMPoint>* path)
{
	BROFILER_CATEGORY("RequestPath", Profiler::Color::Khaki);

	if (generatedPaths.size() < 1)
		return false;

	std::unordered_map<Entity*, generatedPath>::iterator it = generatedPaths.begin();

	int maxSize = generatedPaths.size();
	for (int i = 0; i < maxSize; i++)
	{
		if (it->first == request)
		{
			switch (it->second.type)
			{
			case PATH_TYPE::SIMPLE:
			{
				path->insert(path->end(), it->second.path.begin(), it->second.path.end());
				DeletePath(request);
				return true;
			}
			break;
			case PATH_TYPE::ABSTRACT:
			{
				return RefineAndSmoothPath(&it->second.path, it->second.lvl, path);;
			}
			break;
			}
		}
		it++;
	}

	return false;
}

iMPoint ModulePathfinding::GetDestination(Entity* request)
{
	BROFILER_CATEGORY("RequestPath", Profiler::Color::Khaki);

	if (generatedPaths.size() < 1)
		return { -1,-1 };

	std::unordered_map<Entity*, generatedPath>::iterator it = generatedPaths.begin();

	int maxSize = generatedPaths.size();
	for (int i = 0; i < maxSize; i++)
	{
		if (it->first == request)
		{
			if (it->second.path.size() > 0)
				return it->second.path.back();
		}
		it++;
	}

	return { INT_MIN,INT_MIN };
}


bool ModulePathfinding::RefineAndSmoothPath(std::vector<iMPoint>* absPath, int lvl, std::vector<iMPoint>* pathToFill)
{
	BROFILER_CATEGORY("Refine And Smooth Path", Profiler::Color::RosyBrown);

	std::vector <iMPoint>* generatedPath = nullptr;
	iMPoint currPos = { -1, -1 };
	iMPoint startPos = { -1, -1 };
	int from = -1;
	int maxPath = lvl * CLUSTER_SIZE_LVL;
	int pathSize = absPath->size();

	Cluster* fromC = nullptr;


	for (int i = 0; i < pathSize; i)
	{
		currPos = absPath->at(i);

		if (startPos.x == -1)
		{
			startPos = currPos;
			from = i;
			fromC = absGraph.determineCluster(startPos, lvl);
			i++;
			continue;
		}

		if (fromC != absGraph.determineCluster(currPos, lvl) || (i == pathSize - 1 && pathSize > 0))
		{
			if (LineRayCast(startPos, currPos) && !last_line.empty())
			{
				generatedPath = &last_line;
			}

			else if (SimpleAPathfinding(startPos, currPos) && !last_path.empty())
			{
				generatedPath = &last_path;
			}

			if (pathToFill->size() > 1)
				pathToFill->insert(pathToFill->end(), generatedPath->begin() + 1, generatedPath->end());
			else
				pathToFill->insert(pathToFill->end(), generatedPath->begin(), generatedPath->end());

			absPath->erase(absPath->begin() + from, absPath->begin() + i);
			break;
		}
		else
		{
			absPath->erase(absPath->begin() + 1);
			pathSize--;
			continue;
		}

	}

	if (generatedPath != nullptr)
		generatedPath->clear();

	return true;
}

bool ModulePathfinding::IsStraightPath(iMPoint from, iMPoint to)
{
	if (from.x == to.x || from.y == to.y || abs(from.x - to.x) == abs(from.y - to.y))
		return true;

	return false;
}

std::multimap<float, PathNode>::iterator ModulePathfinding::Find(iMPoint point, std::multimap<float, PathNode>* map)
{
	std::multimap<float, PathNode>::iterator iterator = map->begin();

	int size = map->size();

	for (int i = 0; i < size; i++)
	{
		if (iterator->second.pos == point)
		{
			return iterator;
		}
		iterator++;
	}

	return map->end();
}

std::multimap<float, HierNode>::iterator ModulePathfinding::Find(iMPoint point, std::multimap<float, HierNode>* map)
{
	std::multimap<float, HierNode>::iterator iterator = map->begin();

	int size = map->size();

	for (int i = 0; i < size; i++)
	{
		if (iterator->second.pos == point)
		{
			return iterator;
		}
		iterator++;
	}

	return map->end();
}



int ModulePathfinding::FindV(iMPoint point, std::vector<PathNode>* vec)
{
	int numElements = vec->size();

	for (int i = 0; i < numElements; i++)
	{
		if (vec->at(i).pos == point)
		{
			return i;
		}
	}

	return vec->size();
}

int ModulePathfinding::FindV(iMPoint point, std::vector<HierNode>* vec)
{
	int numElements = vec->size();

	for (int i = 0; i < numElements; i++)
	{
		if (vec->at(i).pos == point)
		{
			return i;
		}
	}

	return vec->size();
}

bool ModulePathfinding::DeletePath(Entity* request)
{
	BROFILER_CATEGORY("Destroy Path", Profiler::Color::Khaki);

	if (generatedPaths.size() < 1)
		return false;

	std::unordered_map<Entity*, generatedPath>::iterator it = generatedPaths.begin();

	int maxSize = generatedPaths.size();
	for (int i = 0; i < maxSize; i++)
	{
		if (it->first == request)
		{
			generatedPaths.erase(request);
			return true;
		}
		it++;
	}

	return false;
}
