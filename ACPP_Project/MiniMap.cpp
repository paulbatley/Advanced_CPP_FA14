
/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include "MiniMap.h"

//Screen dimension constants
const int mSCREEN_WIDTH = 10*40;
const int mSCREEN_HEIGHT = 5*40;

////Starts up SDL and creates window
//bool init();
//
////Loads media
//bool loadMedia();
//
////Frees media and shuts down SDL
//void close();

//The window we'll be rendering to
SDL_Window* mgWindow = NULL;

//The surface contained by the window
SDL_Surface* mgScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gHelloWorld = NULL;





bool MiniMap::init()
{
	//Initialization flag
	bool success = true;

	////Initialize SDL
	//if (SDL_Init(SDL_INIT_VIDEO) < 0)
	//{
	//	printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	//	success = false;
	//}
	//else
	//{
	//Create window
	mgWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mSCREEN_WIDTH, mSCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (mgWindow == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Get window surface
		mgScreenSurface = SDL_GetWindowSurface(mgWindow);

		//Grab window identifier 
		mWindowID = SDL_GetWindowID( mgWindow );
	}
	//	}

	return success;
}

bool MiniMap::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load splash image
	gHelloWorld = SDL_LoadBMP("hello_world.bmp");
	if (gHelloWorld == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", "02_getting_an_image_on_the_screen/hello_world.bmp", SDL_GetError());
		success = false;
	}

	return success;
}

void MiniMap::close()
{
	//Deallocate surface
	SDL_FreeSurface(gHelloWorld);
	gHelloWorld = NULL;

	//Destroy window
	SDL_DestroyWindow(mgWindow);
	mgWindow = NULL;

	//Quit SDL subsystems
	//SDL_Quit();
}

SDL_Window* MiniMap::set()
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//	//Main loop flag
			//	bool quit = false;

			//	//Event handler
			//	SDL_Event e;

			//	//While application is running
			//	while (!quit)
			//	{
			//		//Handle events on queue
			//		while (SDL_PollEvent(&e) != 0)
			//		{
			//			//User requests quit
			//			if (e.type == SDL_WINDOWEVENT && e.window.windowID == mWindowID){
			//				if (e.window.event == SDL_WINDOWEVENT_CLOSE)
			//				{
			//					quit = true;
			//				}
			//			}

			//		}

			//		//Apply the image
			//		SDL_BlitSurface(gHelloWorld, NULL, mgScreenSurface, NULL);

			//		//Update the surface
			//		SDL_UpdateWindowSurface(mgWindow);

			//		//Wait two seconds
			//		SDL_Delay(2000);
			//	}
			//}

			//Apply the image
			SDL_BlitSurface(gHelloWorld, NULL, mgScreenSurface, NULL);

			//Update the surface
			SDL_UpdateWindowSurface(mgWindow);

			//Wait two seconds
			//SDL_Delay(2000);
			////Free resources and close SDL
			//close();

			return mgWindow;
		}
	}
	return mgWindow;
}


bool MiniMap::updateMiniMap(){
	SDL_Event e;
	if(SDL_PollEvent(&e) != 0)
	{
		//User requests quit
		if (e.type == SDL_WINDOWEVENT && e.window.windowID == mWindowID){
			if (e.window.event == SDL_WINDOWEVENT_CLOSE)
			{
				close();
				return true;
			}
		}

	}

	//Apply the image
	SDL_BlitSurface(gHelloWorld, NULL, mgScreenSurface, NULL);

	//Update the surface
	SDL_UpdateWindowSurface(mgWindow);

	//Wait two seconds
	//SDL_Delay(2000);
	return false;
}







	//if (e.type == SDL_WINDOWEVENT && e.window.windowID == mWindowID){
	//	if (e.window.event == SDL_WINDOWEVENT_CLOSE)
	//	{
	//		close();
	//	}
	//}
	//else{
	//		//Apply the image
	//		SDL_BlitSurface(gHelloWorld, NULL, mgScreenSurface, NULL);

	//		//Update the surface
	//		SDL_UpdateWindowSurface(mgWindow);

	//		//Wait two seconds
	//		SDL_Delay(2000);
	//	
	//}

//}