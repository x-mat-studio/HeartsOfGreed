#include "App.h"
#include "AssetManager.h"


ModuleAssetManager::ModuleAssetManager() : Module()
{
	name = ("assetManager");

	// Initialize the PhysicsFS library
	// This must be called before any other PhysicsFS function
	// This should be called prior to any attempts to change your process's current working directory
	PHYSFS_init(nullptr);

	// We only need this when compiling in debug. In Release we don't need it.
	PHYSFS_mount(".", nullptr, 1);
}


ModuleAssetManager::~ModuleAssetManager()
{
	// Deinitialize the PhysicsFS library.
	// This closes any files opened via PhysicsFS, blanks the search/write paths, frees memory, and invalidates all of your file handles.
	// NOTE: This call can FAIL if there's a file open for writing that refuses to close
	PHYSFS_deinit();
}

bool ModuleAssetManager::Awake(pugi::xml_node& config)
{
	// Determine if the PhysicsFS library is initialized, we can check it for avoid errors.
	if (PHYSFS_isInit())
		LOG("Asset Manager is succefully loaded");
	else
		LOG("Failed loading Asset Manager");


	// Add an archive or directory to the search path.
	// If this is a duplicate, the entry is not added again, even though the function succeeds.
	// When you mount an archive, it is added to a virtual file system...
	// all files in all of the archives are interpolated into a single hierachical file tree.
	PHYSFS_mount("Assets.zip", nullptr, 1);

	return true;
}


bool ModuleAssetManager::CleanUp()
{
	return false;
}

SDL_RWops* ModuleAssetManager::Load(const char* path) const
{
	char* buffer;
	uint bytes = Load(path, &buffer);


	// Read-only memory buffer for use with RWops, retruns a pointer to a new SDL_RWops structure
	SDL_RWops* ret = SDL_RWFromConstMem(buffer, bytes);

	return ret;

}


uint ModuleAssetManager::Load(const char* path, char** buffer) const
{
	uint ret;


	// The reading offset is set to the first byte of the file.
	// Returns a filehandle on success that we will need for the PHYSFS_fileLength
	PHYSFS_file* file = PHYSFS_openRead(path);

	// Check for end-of-file state on a PhysicsFS filehandle.
	if (!PHYSFS_eof(file))
	{
		// Get total length of a file in bytes
		uint lenght = PHYSFS_fileLength(file);
		*buffer = new char[lenght];

		// Read data from a PhysicsFS firehandle. Returns a number of bytes read.
		uint bytes = PHYSFS_readBytes(file, *buffer, lenght);

		if (bytes != lenght)
		{
			LOG("%s", path, "ERROR: %s", PHYSFS_getLastError());
			RELEASE_ARRAY(buffer);
		}
		else
			ret = bytes;
	}
	else
		LOG("%s", path, "ERROR: %s", PHYSFS_getLastError());


	// Close a PhysicsFS firehandle
	PHYSFS_close(file);

	return ret;
}