#include "projectHeader.h"


/***********\
||SDL stuff||
\***********/

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia(Tile* tiles[]);

LTexture gTileTexture;

SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

//Frees media and shuts down SDL
void close(Tile* tiles[]);

//The window renderer
SDL_Renderer* gRenderer = NULL;
//The window we'll be rendering to
SDL_Window* gWindow = NULL;


LTexture gDotTexture;
//contains sprites
SDL_Rect gSpriteClips[4];

LTexture gSpriteSheetTexture;

//Loads individual image
SDL_Surface* loadSurface(std::string path);

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;


//Current displayed image
SDL_Surface* gCurrentSurface = NULL;


//The images that correspond to a keypress
SDL_Surface* gKeyPressSurfaces[1];



int boardPositionX = SCREEN_WIDTH;
int boardPositionY = SCREEN_HEIGHT;



bool hitSuccess(boardMember Attacker);

int main(int argc, char* args[])
{

	if (!init())
	{
		std::cout << "Initialization failed!" << std::endl;
	}
	else
	{
		Tile* tileSet[TOTAL_TILES];

		if (!loadMedia( tileSet))
		{
			std::cout << "Failed to load media!" << std::endl;
		}
		else
		{
			bool quit = false;

			SDL_Event e;

			//The dot that will be moving around on the screen
			Dot dot;

			while (!quit)
			{
				//event que
				while (SDL_PollEvent(&e) != 0)
				{

					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					//key press event

					dot.handleEvent(e);

				}
				dot.move(tileSet);

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render level
				for (int i = 0; i < TOTAL_TILES; ++i)
				{
					tileSet[i]->render();
				}

				dot.render();

				SDL_RenderPresent(gRenderer);

			}
		}


		close(tileSet);
		//system("pause");
		return 0;
	}
}

bool hitSuccess(boardMember Attacker)
{
	bool hit = false;
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, 3);
	if (distribution(generator) != Attacker.getLevel() % 4)
		hit = true;

	return hit;

}


bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL Error" << SDL_GetError() << std::endl;
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			std::cout << "Window could not be created! SDL Error:" << SDL_GetError() << std::endl;
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
		//	gScreenSurface = SDL_GetWindowSurface(gWindow);
			if (gRenderer == NULL)
			{
				std::cout << "Renderer could not be created! SDL Error:" << SDL_GetError() << std::endl;
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					std::cout << "SDL_image could not initialize! SDL_image Error:" << IMG_GetError() << std::endl;
					success = false;
				}
				gSpriteSheetTexture.render(SCREEN_WIDTH - gSpriteClips[0].w, SCREEN_HEIGHT - gSpriteClips[0].h, &gSpriteClips[0]);
			}
		}
	}
	
	return success;
}

bool loadMedia(Tile* tiles[])
{
	//Loading success flag
	bool success = true;

	//Load dot texture
	if (!gDotTexture.loadFromFile("dot.bmp"))
	{
		printf("Failed to load dot texture!\n");
		success = false;
	}

	//Load tile texture
	if (!gTileTexture.loadFromFile("tiles1.png"))
	{
		printf("Failed to load tile set texture!\n");
		success = false;
	}

	if (!gSpriteSheetTexture.loadFromFile("sprites.png"))
	{
		printf("Failed to load sprite sheet texture!\n");
		success = false;
	}
	else
	{
		//Set  first sprite
		gSpriteClips[0].x = 0;
		gSpriteClips[0].y = 0;
		gSpriteClips[0].w = TILE_WIDTH;
		gSpriteClips[0].h = TILE_HEIGHT;

		//Set second sprite
		gSpriteClips[1].x = TILE_WIDTH;
		gSpriteClips[1].y = 0;
		gSpriteClips[1].w = TILE_WIDTH;
		gSpriteClips[1].h = TILE_HEIGHT;

		//Set third sprite
		gSpriteClips[2].x = 0;
		gSpriteClips[2].y = TILE_HEIGHT;
		gSpriteClips[2].w = TILE_WIDTH;
		gSpriteClips[2].h = TILE_HEIGHT;

		//set the Foe sprite
		gSpriteClips[3].x = TILE_WIDTH;
		gSpriteClips[3].y = TILE_HEIGHT;
		gSpriteClips[3].w = TILE_WIDTH;
		gSpriteClips[3].h = TILE_HEIGHT;


	}
	//Load tile map
	if (!setTiles(tiles))
	{
		printf("Failed to load tile set!\n");
		success = false;
	}
	//Load default surface
	/*gKeyPressSurfaces[0] = loadSurface("room1.jpg");
	if (gKeyPressSurfaces[0] == NULL)
	{
		printf("Failed to load default image!\n");
		success = false;
	}*/
	//sprites to load
	return success;
}

void close(Tile* tiles[])
{
	//Deallocate tiles
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		if (tiles[i] == NULL)
		{
			delete tiles[i];
			tiles[i] = NULL;
		}
	}

	//Free loaded images
	gDotTexture.free();
	gTileTexture.free();


	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool LTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		std::cout << "Unable to load image %s! SDL_image Error: " << path.c_str() << " " << IMG_GetError() << std::endl;
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			std::cout << "Unable to create texture from" << path.c_str() << " !SDL Error: " << SDL_GetError() << std::endl;
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

//SDL_Surface* loadSurface(std::string path)
//{
//	//Load image at specified path
//	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
//	if (loadedSurface == NULL)
//	{
//		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
//	}
//
//	return loadedSurface;
//}

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render(int x, int y, SDL_Rect* clip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

void Dot::render()
{
	//Show the dot
	gDotTexture.render(mBox.x, mBox.y);

}

void Dot::move(Tile *tiles[])
{
	//Move the dot left or right
	mBox.x += mVelX;

	//If the dot went too far to the left or right or touched a wall
	if ((mBox.x < 0) || (mBox.x + DOT_WIDTH > SCREEN_WIDTH) || touchesWall(mBox, tiles))
	{
		//move back
		mBox.x -= mVelX;
	}

	//Move the dot up or down
	mBox.y += mVelY;

	//If the dot went too far up or down or touched a wall
	if ((mBox.y < 0) || (mBox.y + DOT_HEIGHT > SCREEN_HEIGHT) || touchesWall(mBox, tiles))
	{
		//move back
		mBox.y -= mVelY;
	}
}

void Tile::render()
{
	//Show the tile
	gTileTexture.render(mBox.x, mBox.y, &gTileClips[mType]);

}

bool checkCollision(SDL_Rect a, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}


bool touchesWall(SDL_Rect box, Tile* tiles[])
{
	//Go through the tiles
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		//If the tile is a wall type tile
		if ((tiles[i]->getType() > TILE_WALK) && (tiles[i]->getType() <= TILE_BOTTOMRIGHT))
		{
			//If the collision box touches the wall tile
			if (checkCollision(box, tiles[i]->getBox()))
			{
				return true;
			}
		}
	}

	//If no wall tiles were touched
	return false;
}


bool setTiles(Tile* tiles[])
{
	//Success flag
	bool tilesLoaded = true;

	//The tile offsets
	int x = 0, y = 0;

	//Open the map
	std::ifstream map("lazy.map");

	//If the map couldn't be loaded
	if (!map.is_open())
	{
		printf("Unable to load map file!\n");
		tilesLoaded = false;
	}
	else
	{
		//Initialize the tiles
		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			//Determines what kind of tile will be made
			int tileType = -1;

			//Read tile from map file
			map >> tileType;

			//If the was a problem in reading the map
			if (map.fail())
			{
				//Stop loading map
				printf("Error loading map: Unexpected end of file!\n");
				tilesLoaded = false;
				break;
			}

			//If the number is a valid tile number
			if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
			{
				tiles[i] = new Tile(x, y, tileType);
			}
			//If we don't recognize the tile type
			else
			{
				//Stop loading map
				printf("Error loading map: Invalid tile type at %d!\n", i);
				tilesLoaded = false;
				break;
			}

			//Move to next tile spot
			x += TILE_WIDTH;

			//If we've gone too far
			if (x >= SCREEN_WIDTH)
			{
				//Move back
				x = 0;

				//Move to the next row
				y += TILE_HEIGHT;
			}
		}

		//Clip the sprite sheet
		if (tilesLoaded)
		{
			gTileClips[TILE_UPDOOR].x = 0;
			gTileClips[TILE_UPDOOR].y = 0;
			gTileClips[TILE_UPDOOR].w = TILE_WIDTH;
			gTileClips[TILE_UPDOOR].h = TILE_HEIGHT;

			gTileClips[TILE_LEFTDOOR].x = 0;
			gTileClips[TILE_LEFTDOOR].y = 40;
			gTileClips[TILE_LEFTDOOR].w = TILE_WIDTH;
			gTileClips[TILE_LEFTDOOR].h = TILE_HEIGHT;

			gTileClips[TILE_RIGHTDOOR].x = 0;
			gTileClips[TILE_RIGHTDOOR].y = 80;
			gTileClips[TILE_RIGHTDOOR].w = TILE_WIDTH;
			gTileClips[TILE_RIGHTDOOR].h = TILE_HEIGHT;
			
			gTileClips[TILE_BOTTOMDOOR].x = 40;
			gTileClips[TILE_BOTTOMDOOR].y = 0;
			gTileClips[TILE_BOTTOMDOOR].w = TILE_WIDTH;
			gTileClips[TILE_BOTTOMDOOR].h = TILE_HEIGHT;

			gTileClips[TILE_WALK].x = 40;
			gTileClips[TILE_WALK].y = 40;
			gTileClips[TILE_WALK].w = TILE_WIDTH;
			gTileClips[TILE_WALK].h = TILE_HEIGHT;

			gTileClips[TILE_BOTTOMCEN].x = 40;
			gTileClips[TILE_BOTTOMCEN].y = 80;
			gTileClips[TILE_BOTTOMCEN].w = TILE_WIDTH;
			gTileClips[TILE_BOTTOMCEN].h = TILE_HEIGHT;

			gTileClips[TILE_TOPRIGHT].x = 80;
			gTileClips[TILE_TOPRIGHT].y = 0;
			gTileClips[TILE_TOPRIGHT].w = TILE_WIDTH;
			gTileClips[TILE_TOPRIGHT].h = TILE_HEIGHT;

			gTileClips[TILE_CENRIGHT].x = 80;
			gTileClips[TILE_CENRIGHT].y = 40;
			gTileClips[TILE_CENRIGHT].w = TILE_WIDTH;
			gTileClips[TILE_CENRIGHT].h = TILE_HEIGHT;

			gTileClips[TILE_BOTTOMRIGHT].x = 80;
			gTileClips[TILE_BOTTOMRIGHT].y = 80;
			gTileClips[TILE_BOTTOMRIGHT].w = TILE_WIDTH;
			gTileClips[TILE_BOTTOMRIGHT].h = TILE_HEIGHT;
		}
	}

	//Close the file
	map.close();

	//If the map was loaded fine
	return tilesLoaded;
}
