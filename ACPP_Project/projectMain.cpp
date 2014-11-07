#include "projectHeader.h"


/***********\
||SDL stuff||
\***********/

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window renderer
SDL_Renderer* gRenderer = NULL;
//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//contains sprites
SDL_Rect gSpriteClips[4];

LTexture gSpriteSheetTexture;


int boardPositionX = SCREEN_WIDTH;
int boardPositionY = SCREEN_HEIGHT;

bool hitSuccess(boardMember Attacker);

int main(int argc, char* args[])
{
	Allie A1;
	std::cout << A1.expUP(200) << std::endl;
	std::cout << A1.checkForLvlUp() << std::endl;
	std::cout << A1.expUP(200) << std::endl;
	std::cout << A1.checkForLvlUp() << std::endl;
	A1.statSet(10, 3, 2, 2);
	std::cout << A1.getHP() << std::endl;

	Foe F1;
	//x tile #, y tile #
	F1.statSet(5, 2, 1, 1);
	F1.setCoor(5, 7);
	F1.setLvl(5);
	if (!init())
	{
		std::cout << "Initialization failed!" << std::endl;

	}
	else
	{
		if (!loadMedia())
		{
			std::cout << "Failed to load media!" << std::endl;

		}
		else
		{
			bool quit = false;
			SDL_Event evnt;

			while (!quit)
			{
				//event que
				while (SDL_PollEvent(&evnt) != 0)
				{

					if (evnt.type == SDL_QUIT)
					{
						quit = true;
					}
					//key press event
					else if (evnt.type == SDL_KEYDOWN)
					{
						//set board to black
						SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xCC);
						SDL_RenderClear(gRenderer);

						//Select surfaces based on key press
						switch (evnt.key.keysym.sym)
						{
						case SDLK_UP:
							//move UP
							if (boardPositionY <= tileSize)
								boardPositionY = tileSize * 2;
							gSpriteSheetTexture.render(boardPositionX - gSpriteClips[0].w, (boardPositionY -= tileSize) - gSpriteClips[0].h, &gSpriteClips[0]);
							break;

						case SDLK_DOWN:
							//move DOWN
							if (boardPositionY >= (SCREEN_HEIGHT - tileSize))
								boardPositionY = SCREEN_HEIGHT - tileSize;
							gSpriteSheetTexture.render(boardPositionX - gSpriteClips[0].w, (boardPositionY += tileSize) - gSpriteClips[1].h, &gSpriteClips[1]);
							break;

						case SDLK_LEFT:
							//move LEFT
							if (boardPositionX <= tileSize)
								boardPositionX = tileSize * 2;
							gSpriteSheetTexture.render((boardPositionX -= tileSize) - gSpriteClips[0].w, boardPositionY - gSpriteClips[2].h, &gSpriteClips[2]);
							break;

						case SDLK_RIGHT:
							//move RIGHT
							if (boardPositionX >= (SCREEN_WIDTH - tileSize))
								boardPositionX = SCREEN_WIDTH - tileSize;
							gSpriteSheetTexture.render((boardPositionX += tileSize) - gSpriteClips[0].w, boardPositionY - gSpriteClips[0].h, &gSpriteClips[0]);
							break;

						default:
							//do nothing				

							break;
						}
						if (boardPositionX == F1.getXpos() && boardPositionY == F1.getYpos())
						{
							if (hitSuccess(F1))
							{
								std::cout << "Allie Attacked!\n";
								A1.decHP(F1.ATKcalc(A1));
								std::cout << A1.getHP() << std::endl;
								if (A1.getHP() <= 0)
								{
									std::cout << "You are dead\n";
									quit = true;
								}
							}

							gSpriteSheetTexture.render(F1.getXpos() - gSpriteClips[3].w, F1.getYpos() - gSpriteClips[3].h, &gSpriteClips[3]);
							SDL_RenderPresent(gRenderer);
						}

					}

				}




				//set color to white for grid
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				//draw a grid of tiles: spacing determined by the tileSize variable
				for (int i = 0; i < SCREEN_HEIGHT; i += tileSize)
				{
					SDL_RenderDrawLine(gRenderer, 0, i, SCREEN_WIDTH, i);
				}
				for (int i = 0; i < SCREEN_WIDTH; i += tileSize)
				{
					SDL_RenderDrawLine(gRenderer, i, 0, i, SCREEN_HEIGHT);
				}


				SDL_RenderPresent(gRenderer);

			}
		}

	}
	close();
	//system("pause");
	return 0;
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

bool loadMedia()
{
	//Loading success flag
	bool success = true;

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
		gSpriteClips[0].w = tileSize;
		gSpriteClips[0].h = tileSize;

		//Set second sprite
		gSpriteClips[1].x = tileSize;
		gSpriteClips[1].y = 0;
		gSpriteClips[1].w = tileSize;
		gSpriteClips[1].h = tileSize;

		//Set third sprite
		gSpriteClips[2].x = 0;
		gSpriteClips[2].y = tileSize;
		gSpriteClips[2].w = tileSize;
		gSpriteClips[2].h = tileSize;

		//set the Foe sprite
		gSpriteClips[3].x = tileSize;
		gSpriteClips[3].y = tileSize;
		gSpriteClips[3].w = tileSize;
		gSpriteClips[3].h = tileSize;


	}
	//sprites to load
	return success;
}

void close()
{

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

