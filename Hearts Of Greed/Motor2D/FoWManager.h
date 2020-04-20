#ifndef __FOW_MANAGER_H__
#define __FOW_MANAGER_H__
#include "SDL_image/include/SDL_image.h"
#include <vector>
#include <list>
#include <map>
#include "p2Point.h"
#include "FoWBitDefs.h"
#include "Module.h"
#include "FoWEntity.h"


//this struct holds the information of fog and shroud for every tile
struct FoWDataStruct
{
	unsigned short tileFogBits; //saves information about which type of fog are we in (useful for smooth edges)
	unsigned short tileShroudBits; //same as above but for shroud
};

struct MaskData
{
	int numberOfUsers=0;
	unsigned short* mask= nullptr;
};


class ModuleFoWManager :public Module
{
	//FUNCTIONS
public:
	ModuleFoWManager();
	~ModuleFoWManager();


	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	bool CleanUp();

	FoWEntity* CreateFoWEntity(fMPoint pos, bool providesVisibility,int visionRadius=3);
	void DeleteAllFoWEntites();


	//Resets the map to its shrouded state
	void ResetFoWMap();
	void CreateFoWMap(uint width, uint height);
	void DeleteFoWMap();
	//Updates the data on the FoWMap based on the FoWEntities position and mask shape
	void UpdateFoWMap();
	void DrawFoWMap();
	//Tell the map that it needs to be updated the next frame
	void MapNeedsUpdate();

	//Returns the visibility state of the chosen tile (given its map coordinates)
	FoWDataStruct* GetFoWTileState(iMPoint mapPos)const;
	//Returns true if the tile is inside the map boundaries, otherwise returns false
	bool CheckFoWTileBoundaries(iMPoint mapPos)const;
	//Returns true if the tile is visible (there's no FOG in it) otherwise returns false
	bool CheckTileVisibility(iMPoint mapPos)const;

	unsigned short* GetMaskFromRadius(int radius);
	void RequestMaskGeneration(int radius);
	void RequestMaskDeletion(int radius);

	bool InsideCircle(iMPoint center, iMPoint tile, float radius);

private:
	unsigned short* GenerateCircle(int radius);
	unsigned short* GenerateCircleBorders(int radius,unsigned short* mask);
	unsigned short* GenerateCircleJoints(int radius, unsigned short* mask);
	unsigned short CheckCornersFromNeighbours(iMPoint pos, int diameter, unsigned short* mask);
	unsigned short CheckJointsFromNeighbours(iMPoint pos, int diameter, unsigned short* mask);


	//VARIABLES
public:
	bool foWMapVisible;


private:
	//This is where the FoWEntites are stored
	std::vector<FoWEntity*> fowEntities;
	//A map where we will store all created masks using their radius as the key
	std::map<uint, MaskData> maskMap;
	//This is where we store our FoW information
	FoWDataStruct* fowMap;

	SDL_Texture* smoothFoWtexture;
	SDL_Texture* debugFoWtexture;

	//Map that we use to translate bits to Texture Id's
	std::map<unsigned short, uint> bitToTextureTable;

	uint width;
	uint height;
	bool debugMode;
	bool foWMapNeedsRefresh;
	float fowUpdateTimer;
};

#endif // !__FOW_MANAGER_H__