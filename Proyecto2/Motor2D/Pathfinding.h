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
	PathNode(float g, float h, const iMPoint& pos, PathNode* parent);
	PathNode(const PathNode& node);
	uint FindWalkableAdjacents(PathList& list_to_fill);
	float Score() const;
	float CalculateF(const iMPoint& destination);
	float g;
	float h;
	iMPoint pos;
	PathNode* parent;

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

private:
	uint width;
	uint height;
	uchar* map;
	std::vector<iMPoint> last_path;
};


struct PathList
{
	std::list<PathNode>::pointer Find(const iMPoint& point);

	std::list<PathNode> list;
};



#endif // __j1PATHFINDING_H__