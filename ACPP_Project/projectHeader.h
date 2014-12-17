#ifndef _PROJECTHEADER_H_
#define _PROJECTHEADER_H_

#include <SDL.h>
#include <SDL_image.h>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <fstream>
#include "GlobalVar.h"


const int TILE_UPDOOR = 0;
const int TILE_LEFTDOOR = 1;
const int TILE_RIGHTDOOR = 2;
const int TILE_BOTTOMDOOR = 3;
const int TILE_WALL = 4;
const int TILE_BOTTOMCEN = 5;
const int TILE_TOPRIGHT = 6;
const int TILE_CENRIGHT = 7;
const int TILE_BOTTOMRIGHT = 8;


//class predefinition
class FoeDot;
class Tile;

//Box collision detector
bool checkCollision(SDL_Rect a, SDL_Rect b);

//Sets tiles from tile map
bool setTiles(Tile *tiles[]);

//Checks collision box against set of tiles
int touchesWall(SDL_Rect box, Tile* tiles[]);



class boardMember
{
private:
	int level;
	//raw stats
	int HP;
	int ATK;
	int DFN;
	int POW;
	int maxHP;

public:
	boardMember();
	~boardMember(){};
	void statSet(int newHP, int newPOW, int newATK, int newDFN);
	int levelUP(){ return ++level; }
	void setLvl(int levelToSet){ level += levelToSet; }
	//unsigned int ATKcalc(boardMember&);
	int getLevel(){ return level; }
	int getHP(){ return HP; }
	bool decHP(int amount);
	void incHP(int amount);
	void fillHP(){ HP = maxHP; }
	int getATK(){ return ATK; }
	int getDEF(){ return DFN; }
	int getPOW(){ return POW; }
	int getMaxHP(){ return maxHP; }
	bool hpFull();
};

//	long int expr;
//	int expUP(int XPtoUP){ return expr += XPtoUP; }
//	int checkForLvlUp();
//
//
//	void setCoor(int x, int y){ xPos = (TILE_WIDTH); yPos = (TILE_HEIGHT); }
//	int XPgiven(Foe);

class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

	//Deallocates texture
	void free();

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

//used to tile the board
class Tile
{
public:
	//Initializes position and type
	Tile(int x, int y, int tileType);

	//Shows the tile
	void render();

	//Get the tile type
	int getType();

	//Get the collision box
	SDL_Rect getBox();
	void setType(int type){	mType = type;}
private:
	//The attributes of the tile
	SDL_Rect mBox;

	//The tile type
	int mType;
};

//The dot that will move around on the screen
class Dot : public boardMember
{
public:
	//The dimensions of the dot
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;
	
	//Maximum axis velocity of the dot
	const float DOT_VEL = 125;

	//Initializes the variables
	Dot();

	//Takes key presses and adjusts the dot's velocity
	void handleEvent(SDL_Event& e);

	//Moves the dot and check collision against tiles
	bool move(Tile *tiles[], float timeStep);

	//Shows the dot on the screen
	void render();

	void setBoxX(int);
	void setBoxY(int);
	int getBoxX();
	int getBoxY();
	SDL_Rect getBox(){ return mBox; }

private:
	//Collision box of the dot
	SDL_Rect mBox;
	//The velocity of the dot
	float mVelX, mVelY;
	float mPosX, mPosY;
};

//basic ai
class FoeDot : public boardMember
{
public:
	//The dimensions of the dot
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;

	//Maximum axis velocity of the dot
	const float DOT_VEL = 500;
	//Initializes the variables
	FoeDot();
	FoeDot(int, int);
	void handleEvent();
	//Shows the dot on the screen
	void render();
	void setBoxX(int);
	void setBoxY(int);
	int getBoxX();
	int getBoxY();
	void move(Tile *tiles[], float timeStep);
	SDL_Rect getBox(){ return mBox; }

private:
	//Collision box of the dot
	SDL_Rect mBox;
	//The velocity of the dot
	float mVelX, mVelY;
	float mPosX, mPosY;
	
};

//The application time based timer
class LTimer
{
public:
	//Initializes variables
	LTimer();

	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Gets the timer's time
	Uint32 getTicks();

	//Checks the status of the timer
	bool isStarted();
	bool isPaused();

private:
	//The clock time when the timer started
	Uint32 mStartTicks;

	//The ticks stored when the timer was paused
	Uint32 mPausedTicks;

	//The timer status
	bool mPaused;
	bool mStarted;
};


#endif