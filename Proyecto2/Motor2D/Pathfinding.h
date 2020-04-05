#ifndef __j1PATHFINDING_H__
#define __j1PATHFINDING_H__

#include "Module.h"
#include "p2Point.h"

#define DEFAULT_PATH_LENGTH 50
#define INVALID_WALK_CODE 255

#include <list>
#include <vector>
#include <map>
#include <algorithm>

struct PathList;


struct PathNode
{
	PathNode();
	PathNode(float g, float h, const iMPoint& pos, PathNode* parent, int parentdir, int myDir, bool isdiagonal = false);
	PathNode(const PathNode& node);
	uint FindWalkableAdjacents(std::vector<PathNode>& list_to_fill);
	float Score() const;
	float CalculateF(const iMPoint& destination);
	float g;
	float h;
	bool is_Diagonal;
	iMPoint pos;

	PathNode* parent;

	int parentDir;
	int myDirection;

	bool operator==(PathNode* node)
	{
		if (g == node->g && h == node->h && pos == node->pos && parent == node->parent)
		{
			return true;
		}

		return false;
	}
};


class ModulePathfinding : public Module
{
public:

	ModulePathfinding();


	~ModulePathfinding();

	bool CleanUp();


	void SetMap(uint width, uint height, uchar* data);


	int CreatePath(const iMPoint& origin, const iMPoint& destination);


	const std::vector<iMPoint>* GetLastPath() const;


	bool CheckBoundaries(const iMPoint& pos) const;


	bool IsWalkable(const iMPoint& pos) const;


	uchar GetTileAt(const iMPoint& pos) const;

	void SavePath(std::vector<iMPoint>* path);

	std::multimap<int, PathNode>::iterator Find(iMPoint point, std::multimap<int, PathNode>& map);
	int FindV(iMPoint point, std::vector<PathNode>& vec);

private:
	uint width;
	uint height;
	uchar* map;
	std::vector<iMPoint> last_path;
};


#endif // __j1PATHFINDING_H__