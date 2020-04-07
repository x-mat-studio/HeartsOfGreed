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
#include <unordered_map>

//HPA*-------------------------------------------
#define NODE_MIN_DISTANCE 5
#define CLUSTER_SIZE_LVL 5
#define MAX_LEVELS 1

class HierNode;
class Entity;

//This is always relative to c1
enum class ADJACENT_DIR
{
	DIR_NONE = -1,

	VERTICAL,
	LATERAL
};

enum class EDGE_TYPE
{
	TP_UNKNOWN = -1,

	TP_INTRA,
	TP_INTER

};

struct Cluster
{
	Cluster();
	Cluster(int width, int height, iMPoint& pos);
	Cluster(const Cluster& clust);

	iMPoint pos;
	int width, height;

	std::vector <HierNode*> clustNodes;
};

struct Entrance
{
	Entrance(iMPoint pos, int width, int height, ADJACENT_DIR dir, Cluster* from, Cluster* to);
	Entrance();

	iMPoint pos;
	int width, height;

	ADJACENT_DIR dir;

	Cluster* from;
	Cluster* to;
};

struct Edge
{
	Edge(HierNode* dest, int distanceTo, EDGE_TYPE type);

	HierNode* dest;
	float moveCost;

	EDGE_TYPE type;
};

struct graphLevel
{
	std::vector <std::vector<Cluster>> lvlClusters;

	std::vector <Entrance> entrances;

	void buildClusters(int lvl);
	void buildEntrances(int lvl);

	ADJACENT_DIR adjacents(Cluster* c1, Cluster* c2, int lvl);
	void createEntrance(Cluster* c1, Cluster* c2, ADJACENT_DIR adjDir, int lvl);

	HierNode* insertNode(iMPoint pos, int maxLvl, bool* toDelete = nullptr);
	void deleteNode(HierNode* toDelete, int maxLvl);
	Cluster* determineCluster(iMPoint nodePos, int lvl, Cluster* firstCheck = nullptr);
	void ConnectNodeToBorder(HierNode* node, Cluster* c, int lvl);

	HierNode* NodeExists(iMPoint pos, Cluster* lvl);
};

//Basic A*---------------------------------------

enum class PATH_TYPE
{
	NO_TYPE = -1,

	ABSTRACT,
	SIMPLE

};

struct generatedPath
{
	generatedPath(std::vector <iMPoint> vector, PATH_TYPE type, int lvl);

	std::vector<iMPoint> path;
	PATH_TYPE type;
	int lvl;
};


class PathNode
{
public:
	PathNode();
	PathNode(float g, float h, const iMPoint& pos, PathNode* parent, int parentdir, int myDir, bool isdiagonal = false);
	PathNode(const PathNode& node);
	virtual uint FindWalkableAdjacents(std::vector<PathNode>& list_to_fill);
	float Score() const;
	virtual float CalculateF(const iMPoint& destination);

	float g;
	float h;
	iMPoint pos;

	PathNode* parent;

	int parentDir;
	int myDirection;
	bool is_Diagonal;

};

class HierNode : public PathNode
{
public:
	HierNode(iMPoint pos);
	HierNode(iMPoint pos, bool tmp);
	HierNode(float g, const iMPoint& pos, PathNode* parent, int myDir, int parentdir, std::vector<Edge*> edges);

	float CalculateF(const iMPoint& destination);
	std::vector <Edge*> edges;
	uint FindWalkableAdjacents(std::vector<HierNode>& list_to_fill);
};


class ModulePathfinding : public Module
{
public:

	ModulePathfinding();


	~ModulePathfinding();

	bool CleanUp();


	void SetMap(uint width, uint height, uchar* data);

	PATH_TYPE CreatePath(const iMPoint& origin, const iMPoint& destination, int maxLvl, Entity* pathRequest);

	bool CheckBoundaries(const iMPoint& pos) const;

	bool IsWalkable(const iMPoint& pos) const;

	uchar GetTileAt(const iMPoint& pos) const;

	bool RequestPath(Entity* request, std::vector <iMPoint>* path);

private:

	float SimpleAPathfinding(const iMPoint& origin, const iMPoint& destination);

	int HPAPathfinding(const HierNode& origin, const iMPoint& destination, int lvl);

	std::multimap<int, PathNode>::iterator Find(iMPoint point, std::multimap<int, PathNode>* map);
	std::multimap<int, HierNode>::iterator Find(iMPoint point, std::multimap<int, HierNode>* map);
	int FindV(iMPoint point, std::vector<PathNode>* vec);
	int FindV(iMPoint point, std::vector<HierNode>* vec);

	void preProcessing(int maxLevel);
	void buildGraph();
	void abstractMaze();

	bool RefineAndSmoothPath(std::vector<iMPoint>* absPath, int lvl, std::vector<iMPoint>* refinedPath);
	bool IsStraightPath(iMPoint from, iMPoint to);

public:
	uint width;
	uint height;

private:

	uchar* walkabilityMap;
	graphLevel absGraph;

	std::vector<iMPoint> last_path;

	std::unordered_map <Entity*, generatedPath> generatedPaths;
};

#endif // __j1PATHFINDING_H__