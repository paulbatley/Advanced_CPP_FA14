#include "projectHeader.h"
#include "Room.h"
#include "Dungeon.h"
#include <Windows.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "MiniMap.h"

/***********\
||SDL stuff||
\***********/
int ROOM_WIDTH = SCREEN_WIDTH / TILE_WIDTH, ROOM_HEIGHT = SCREEN_HEIGHT / TILE_HEIGHT;// x and y for room width and height
bool setUp = false, setDown = false, setLeft = false, setRight = false;// bools for making doors lock and unlock
bool doorOpen = false;
void turnLock(Tile* tiles[]);
void lockDoor(Tile* tiles[]);
std::mutex bool_mu;

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
int mainWindowID;
SDL_Window* gWindow = NULL;
LTexture gDotTexture;
LTexture gFoeDotTexture;

//contains sprites
LTexture gSpriteSheetTexture;

//Loads individual image
SDL_Surface* loadSurface(std::string path);

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//Current displayed image
SDL_Surface* gCurrentSurface = NULL;

//The images that correspond to a keypress
SDL_Surface* gKeyPressSurfaces[1];

Dot dot;
FoeDot foe;
FoeDot foe1(250, 100);
Dungeon dungeonLevel;
int roomIndex = dungeonLevel.firstRoom;
int boardPositionX = SCREEN_WIDTH;
int boardPositionY = SCREEN_HEIGHT;
//bool hitSuccess(boardMember Attacker);

MiniMap mini;


void makeMiniMap(){
	Sleep(200);
	mini.set();


}

//bool hitSuccess(boardMember Attacker);

int main(int argc, char* args[])
{
	thread makeMap(makeMiniMap);

	if (!init())
	{
		std::cout << "Initialization failed!" << std::endl;
	}
	else
	{
		Tile* tileSet[TOTAL_TILES];
		if (!loadMedia(tileSet))
		{
			std::cout << "Failed to load media!" << std::endl;
		}
		else
		{
			bool quit = false;
			bool dead = false;
			SDL_Event e;
			//MAKE THREAD LOCK FOR MAP GENERATION
			LTimer stepTimer;
			while (!quit)
			{
				//event que
				while (SDL_PollEvent(&e) != 0)
				{
					//if (e.type == SDL_WINDOWEVENT && e.window.windowID == mainWindowID){
					//	if (e.window.event == SDL_WINDOWEVENT_CLOSE)
					//	{
					//		quit = true;
					//	}
					//}
					//else{
					//	//key press event
					//	dot.handleEvent(e);
					//}

					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					//key press event
					dot.handleEvent(e);
				}

				float timeStep = stepTimer.getTicks() / 1000.f;
				dead = dot.move(tileSet, timeStep);
				foe.move(tileSet, timeStep);
				foe1.move(tileSet, timeStep);
				stepTimer.start();
				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);
				//Render level
				for (int i = 0; i < TOTAL_TILES; ++i)
				{
					tileSet[i]->render();
				}
				dot.render();
				foe.render();
				foe1.render();
				SDL_RenderPresent(gRenderer);
				if (quit)
					;//dungeonLevel.destroy();
				else
					quit = dead;
			}
		}
		close(tileSet);
	}

	//system("pause");
	return 0;
}
//
//bool hitSuccess(boardMember Attacker)
//{
// bool hit = false;
// std::default_random_engine generator;
// std::uniform_int_distribution<int> distribution(0, 3);
// if (distribution(generator) != Attacker.getLevel() % 4)
// hit = true;
//
// return hit;
//
//}
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
			// gScreenSurface = SDL_GetWindowSurface(gWindow);
			if (gRenderer == NULL)
			{
				std::cout << "Renderer could not be created! SDL Error:" << SDL_GetError() << std::endl;
				success = false;
			}
			else
			{
				mainWindowID = SDL_GetWindowID(gWindow);

				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					std::cout << "SDL_image could not initialize! SDL_image Error:" << IMG_GetError() << std::endl;
					success = false;
				}
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
	if (!gFoeDotTexture.loadFromFile("dot1.bmp"))
	{
		printf("Failed to load foedot texture!\n");
		success = false;
	}
	//Load tile texture
	if (!gTileTexture.loadFromFile("tiles1.png"))
	{
		printf("Failed to load tile set texture!\n");
		success = false;
	}
	//Load tile map
	if (!setTiles(tiles))
	{
		printf("Failed to load tile set!\n");
		success = false;
	}
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
	gFoeDotTexture.free();
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
bool Dot::move(Tile *tiles[], float timeStep)
{
	bool hitsFoe = checkCollision(foe.getBox(), mBox);
	if (hitsFoe)
	if (dot.getHP() > 0)
		dot.decHP(1);
	else
		return true;
	bool hitsFoe1 = checkCollision(foe1.getBox(), mBox);
	if (hitsFoe1)
	if (dot.getHP() > 0)
		dot.decHP(1);
	else
		return true;
	int wallTouched;
	//Move the dot left or right
	mPosX += mVelX * timeStep;
	mBox.x = (int)mPosX;
	wallTouched = touchesWall(mBox, tiles);
	//cout << wallTouched << endl;
	//If the dot went too far to the left or right or touched a wall
	if ((mPosX < 0) || (mPosX + DOT_WIDTH > SCREEN_WIDTH) || wallTouched == TILE_WALL)
	{
		mPosX -= mVelX * timeStep;
	}
	mBox.x = (int)mPosX;
	//Move the dot up or down
	mPosY += mVelY * timeStep;
	mBox.y = (int)mPosY;
	wallTouched = touchesWall(mBox, tiles);
	//If the dot went too far up or down or touched a wall
	if ((mPosY < 0) || (mPosY + DOT_HEIGHT > SCREEN_HEIGHT) || wallTouched == TILE_WALL)
	{
		mPosY -= mVelY * timeStep;
	}
	mBox.y = (int)mPosY;
	return false;
}
void FoeDot::render()
{
	//Show the dot
	gFoeDotTexture.render(mBox.x, mBox.y);
}
void FoeDot::move(Tile *tiles[], float timeStep)
{
	mBox.x = (int)mPosX;
	mBox.y = (int)mPosY;
	bool hitsDot = checkCollision(dot.getBox(), mBox);
	if (hitsDot)
		turnLock(tiles);
	mPosX += mVelX * timeStep;
	//If the dot went too far to the left or right
	if (mPosX < 0 || mPosX > SCREEN_WIDTH - DOT_WIDTH)
	{
		mVelX -= mVelX * 2;
		mPosX += mVelX * timeStep;
	}
	else if (hitsDot)
	{
		mVelX -= mVelX * 2;
		//adjustment to prevent multiple collisions
		if (mVelX > 0)
			mPosX += mVelX * timeStep + 5;
		else
			mPosX += mVelX * timeStep - 5;
	}
	mBox.x = (int)mPosX;
	//Move the dot up or down
	mPosY += mVelY * timeStep;
	//If the dot went too far up or down
	if (mPosY < 0 || mPosY > SCREEN_HEIGHT - DOT_HEIGHT)
	{
		mVelY -= mVelY * 2;
		mPosY += mVelY * timeStep;
	}
	else if (hitsDot)
	{
		mVelY -= mVelY * 2;
		//adjustment to prevent multiple collisions
		if (mVelY > 0)
			mPosY += mVelY * timeStep + 5;
		else
			mPosY += mVelY * timeStep - 5;
	}
	mBox.y = (int)mPosY;
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
int touchesWall(SDL_Rect box, Tile* tiles[])
{
	//Go through the tiles
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		//If the tile is a wall type tile
		if (tiles[i]->getType() < TILE_BOTTOMCEN)
		{
			//If the collision box touches the wall tile
			if (checkCollision(box, tiles[i]->getBox()))
			{
				switch (tiles[i]->getType()){
				case(TILE_UPDOOR) :
					if (dungeonLevel.map[roomIndex]->up && dot.getBoxY() < 2 && doorOpen){
						roomIndex = roomIndex - DUNGEON_WIDTH;
						Sleep(500);
						setTiles(tiles);
						dot.setBoxY(SCREEN_HEIGHT - TILE_HEIGHT - 12);
						foe.setBoxX(120);
						foe.setBoxY(40);
						lockDoor(tiles);
					}
					return TILE_UPDOOR;
					break;
				case(TILE_LEFTDOOR) :
					if (dungeonLevel.map[roomIndex]->left && dot.getBoxX() < 2 && doorOpen){
						roomIndex--;
						Sleep(500);
						setTiles(tiles);
						dot.setBoxX(SCREEN_WIDTH - TILE_WIDTH - 6);
						foe.setBoxX(120);
						foe.setBoxY(40);
						lockDoor(tiles);
					}
					return TILE_LEFTDOOR;
					break;
				case(TILE_RIGHTDOOR) :
					if (dungeonLevel.map[roomIndex]->right && dot.getBoxX() > (SCREEN_WIDTH - 30) && doorOpen){
						roomIndex++;
						Sleep(500);
						setTiles(tiles);
						dot.setBoxX(TILE_WIDTH + 2);
						foe.setBoxX(120);
						foe.setBoxY(40);
						lockDoor(tiles);
					}
					return TILE_RIGHTDOOR;
					break;
				case(TILE_BOTTOMDOOR) :
					if (dungeonLevel.map[roomIndex]->down && dot.getBoxY() > (SCREEN_HEIGHT - 22) && doorOpen){
						roomIndex = roomIndex + DUNGEON_WIDTH;
						Sleep(500);
						setTiles(tiles);
						dot.setBoxY(TILE_HEIGHT + 1);
						foe.setBoxX(120);
						foe.setBoxY(40);
						lockDoor(tiles);
					}
					return TILE_BOTTOMDOOR;
					break;
				}
				return TILE_WALL;
			}
		}
		else if (tiles[i]->getType() == TILE_CENRIGHT)
		{
			if (dot.hpFull())
				;
			else
			{
				dot.incHP(1);
				std::cout << "Health Increased to: " << dot.getHP() << std::endl;
				return TILE_BOTTOMCEN;
			}
		}
	}
	//If no wall tiles were touched
	return TILE_BOTTOMCEN;
}
bool setTiles(Tile* tiles[])
{
	cout << "UP:" << dungeonLevel.map[roomIndex]->up;
	cout << " DOWN:" << dungeonLevel.map[roomIndex]->down;
	cout << " LEFT:" << dungeonLevel.map[roomIndex]->left;
	cout << " RIGHT:" << dungeonLevel.map[roomIndex]->right << endl;
	cout << "( " << dungeonLevel.map[roomIndex]->coord.x << ", " << dungeonLevel.map[roomIndex]->coord.y << " )" << endl;
	cout << roomIndex << endl;
	//Success flag
	bool tilesLoaded = true;
	//The tile offsets
	int x = 0, y = 0;
	//Open the map
	std::ifstream map(dungeonLevel.map[roomIndex]->filename);
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
			gTileClips[TILE_WALL].x = 40;
			gTileClips[TILE_WALL].y = 40;
			gTileClips[TILE_WALL].w = TILE_WIDTH;
			gTileClips[TILE_WALL].h = TILE_HEIGHT;
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
void turnLock(Tile* tiles[])
{
	std::lock_guard<mutex> lock(bool_mu);
	if (doorOpen)
	{
		doorOpen = false;
		gTileClips[TILE_WALL];
		std::cout << "locked" << std::endl;
		if (tiles[(ROOM_WIDTH*ROOM_HEIGHT) / (2 * ROOM_HEIGHT)]->getType() == 0)
		{
			tiles[(ROOM_WIDTH*ROOM_HEIGHT) / (2 * ROOM_HEIGHT)]->setType(4);
			setUp = true;
		}
		if (tiles[(ROOM_WIDTH*ROOM_HEIGHT) / 2]->getType() == 1)
		{
			tiles[(ROOM_WIDTH*ROOM_HEIGHT) / 2]->setType(4);
			setLeft = true;
		}
		if (tiles[((ROOM_WIDTH*ROOM_HEIGHT) / 2) + (ROOM_WIDTH - 1)]->getType() == 2)
		{
			tiles[((ROOM_WIDTH*ROOM_HEIGHT) / 2) + (ROOM_WIDTH - 1)]->setType(4);
			setRight = true;
		}
		if (tiles[((ROOM_WIDTH*ROOM_HEIGHT) - (ROOM_WIDTH*ROOM_HEIGHT) / (2 * ROOM_HEIGHT))]->getType() == 3)
		{
			tiles[((ROOM_WIDTH*ROOM_HEIGHT) - (ROOM_WIDTH*ROOM_HEIGHT) / (2 * ROOM_HEIGHT))]->setType(4);
			setDown = true;
		}
	}
	else
	{
		doorOpen = true;
		std::cout << "unlocked" << std::endl;
		if (setUp == true)
		{
			tiles[(ROOM_WIDTH*ROOM_HEIGHT) / (2 * ROOM_HEIGHT)]->setType(0);
			setUp = false;
		}
		if (setLeft == true)
		{
			tiles[(ROOM_WIDTH*ROOM_HEIGHT) / 2]->setType(1);
			setLeft = false;
		}
		if (setRight == true)
		{
			tiles[((ROOM_WIDTH*ROOM_HEIGHT) / 2) + (ROOM_WIDTH - 1)]->setType(2);
			setRight = false;
		}
		if (setDown == true)
		{
			tiles[((ROOM_WIDTH*ROOM_HEIGHT) - (ROOM_WIDTH*ROOM_HEIGHT) / (2 * ROOM_HEIGHT))]->setType(3);
			setDown = false;
		}
	}
}
void lockDoor(Tile* tiles[])
{
	if (doorOpen)
	{
		doorOpen = false;
		std::cout << "locked" << std::endl;
		if (tiles[(ROOM_WIDTH*ROOM_HEIGHT) / (2 * ROOM_HEIGHT)]->getType() == 0)
		{
			tiles[(ROOM_WIDTH*ROOM_HEIGHT) / (2 * ROOM_HEIGHT)]->setType(4);
			setUp = true;
		}
		if (tiles[(ROOM_WIDTH*ROOM_HEIGHT) / 2]->getType() == 1)
		{
			tiles[(ROOM_WIDTH*ROOM_HEIGHT) / 2]->setType(4);
			setLeft = true;
		}
		if (tiles[((ROOM_WIDTH*ROOM_HEIGHT) / 2) + (ROOM_WIDTH - 1)]->getType() == 2)
		{
			tiles[((ROOM_WIDTH*ROOM_HEIGHT) / 2) + (ROOM_WIDTH - 1)]->setType(4);
			setRight = true;
		}
		if (tiles[((ROOM_WIDTH*ROOM_HEIGHT) - (ROOM_WIDTH*ROOM_HEIGHT) / (2 * ROOM_HEIGHT))]->getType() == 3)
		{
			tiles[((ROOM_WIDTH*ROOM_HEIGHT) - (ROOM_WIDTH*ROOM_HEIGHT) / (2 * ROOM_HEIGHT))]->setType(4);
			setDown = true;
		}
	}
}
bool boardMember::decHP(int amount)
{
	HP -= amount;
	std::cout << getHP() << std::endl;
	if (HP > 1)
		return false;
	else
		return true;
}
void boardMember::incHP(int amount)
{
	HP += amount;
}
bool boardMember::hpFull()
{
	if (getHP() == getMaxHP())
		return true;
	else
		return false;
}
