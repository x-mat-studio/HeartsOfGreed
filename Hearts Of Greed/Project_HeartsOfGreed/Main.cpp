#include <stdlib.h>

#include "p2Defs.h"
#include "p2Log.h"
#include "App.h"

// This is needed here because SDL redefines main function
// do not add any other libraries here, instead put them in their modules
#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

//Brofiler
#include "Brofiler/Brofiler/Brofiler.h"
#pragma comment(lib, "Brofiler/Brofiler/ProfilerCore32.lib")
enum class MAIN_STATE
{
	CREATE = 1,
	AWAKE,
	START,
	LOOP,
	CLEAN,
	FAIL,
	EXIT
};

App* app = NULL;

int main(int argc, char* args[])
{

	MAIN_STATE state = MAIN_STATE::CREATE;
	int result = EXIT_FAILURE;


	while (state != MAIN_STATE::EXIT)
	{
		BROFILER_FRAME("Thread_Game")


			switch (state)
			{

				// Allocate the engine --------------------------------------------
			case MAIN_STATE::CREATE:

				app = new App(argc, args);


				if (app != NULL)
					state = MAIN_STATE::AWAKE;
				else
					state = MAIN_STATE::FAIL;


				break;

				// Awake all modules -----------------------------------------------
			case MAIN_STATE::AWAKE:


				if (app->Awake() == true)
					state = MAIN_STATE::START;
				else
				{
					state = MAIN_STATE::FAIL;
				}


				break;

				// Call all modules before first frame  ----------------------------
			case MAIN_STATE::START:


				if (app->Start() == true)
				{
					state = MAIN_STATE::LOOP;
				}
				else
				{
					state = MAIN_STATE::FAIL;
				}


				break;

				// Loop all modules until we are asked to leave ---------------------
			case MAIN_STATE::LOOP:


				if (app->Update() == false)
					state = MAIN_STATE::CLEAN;


				break;

				// Cleanup allocated memory -----------------------------------------
			case MAIN_STATE::CLEAN:


				if (app->CleanUp() == true)
				{
					RELEASE(app);
					result = EXIT_SUCCESS;
					state = MAIN_STATE::EXIT;
				}
				else
					state = MAIN_STATE::FAIL;


				break;

				// Exit with errors and shame ---------------------------------------
			case MAIN_STATE::FAIL:
				result = EXIT_FAILURE;
				state = MAIN_STATE::EXIT;
				break;
			}


	}


	LOG("... Bye! :)\n");

	// Dump memory leaks
	return result;
}