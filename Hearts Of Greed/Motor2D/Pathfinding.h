#ifndef __j1PATHFINDING_H__
#define __j1PATHFINDING_H__

#include "Module.h"
#include "p2Point.h"

#define DEFAULT_PATH_LENGTH 50
#define INVALID_WALK_CODE 255
#define NEARBY_TILES_CHECK 8

#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include "SDL/include/SDL.h"

//HPA*-------------------------------------------
#define NODE_MIN_DISTANCE 4
#define CLUSTER_SIZE_LVL 9
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
	~Cluster() { clustNodes.clear(); }

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
	Edge(HierNode* dest, float distanceTo, EDGE_TYPE type);

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

	void ReCreateIntraEdges(Cluster* c);
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

	~generatedPath()
	{
		path.clear();
	}

	std::vector<iMPoint> path;
	PATH_TYPE type;
	int lvl;
};

struct pendentPath
{
	pendentPath(iMPoint origin, iMPoint destination, int lvl, PATH_TYPE type = PATH_TYPE::ABSTRACT);


	PATH_TYPE type;
	int lvl;

	iMPoint origin, destination;
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

	bool active;
};


class ModulePathfinding : public Module
{
public:

	ModulePathfinding();


	~ModulePathfinding();

	bool CleanUp();

	bool PreUpdate(float dt);

	void SetMap(uint width, uint height, uchar* data);

	PATH_TYPE GeneratePath(iMPoint& origin, iMPoint& destination, int maxLvl, Entity* pathRequest);


	iMPoint CheckNearbyTiles(const iMPoint& origin, const iMPoint& destination);

	bool CheckBoundaries(const iMPoint& pos) const;

	bool IsWalkable(const iMPoint& pos) const;

	uchar GetTileAt(const iMPoint& pos) const;

	bool RequestPath(Entity* request, std::vector <iMPoint>* path);
	iMPoint GetDestination(Entity* request);
	bool DeletePath(Entity* request);

	bool LineRayCast(iMPoint& p0, iMPoint& p1);
	std::vector<iMPoint> CreateLine(const iMPoint& p0, const iMPoint& p1, int maxDistance = INT_MAX);
	std::vector<iMPoint>* GetLastLine();

	float SimpleAPathfinding(const iMPoint& origin, const iMPoint& destination, int limitpath = -1);

	void SetWalkabilityMap(bool state, iMPoint& position, int width = 1, int height = 1);

	bool IsAreaWalkable(iMPoint& position, int width, int height);

private:

	PATH_TYPE CreatePath(iMPoint& origin, iMPoint& destination, int maxLvl, Entity* pathRequest);

	int HPAPathfinding(const HierNode& origin, const iMPoint& destination, int lvl);

	std::multimap<float, PathNode>::iterator Find(iMPoint point, std::multimap<float, PathNode>* map);
	std::multimap<float, HierNode>::iterator Find(iMPoint point, std::multimap<float, HierNode>* map);
	int FindV(iMPoint point, std::vector<PathNode>* vec);
	int FindV(iMPoint point, std::vector<HierNode>* vec);

	void preProcessing(int maxLevel);
	void buildGraph();
	void abstractMaze();

	bool RefineAndSmoothPath(std::vector<iMPoint>* absPath, int lvl, std::vector<iMPoint>* refinedPath);
	bool IsStraightPath(iMPoint from, iMPoint to);

	bool PathAlreadyExists(iMPoint destination, Entity* requester);

public:
	uint width;
	uint height;

private:

	uchar* walkabilityMap;
	graphLevel absGraph;

	std::vector<iMPoint> last_path;
	std::vector<iMPoint> last_line;

	std::unordered_map <Entity*, generatedPath> generatedPaths;

	std::unordered_map <Entity*, pendentPath> pendentPaths;
};

#endif // __j1PATHFINDING_H__