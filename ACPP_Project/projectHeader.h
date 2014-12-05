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

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_WIDTH = 40;
const int TILE_HEIGHT = 40;
const int TOTAL_TILES = 192;
const int TOTAL_TILE_SPRITES = 9;


const int TILE_UPDOOR = 0;
const int TILE_LEFTDOOR = 1;
const int TILE_RIGHTDOOR = 2;
const int TILE_BOTTOMDOOR = 3;
const int TILE_WALK = 4;
const int TILE_BOTTOMCEN = 5;
const int TILE_TOPRIGHT = 6;
const int TILE_CENRIGHT = 7;
const int TILE_BOTTOMRIGHT = 8;


//Box collision detector
bool checkCollision(SDL_Rect a, SDL_Rect b);


class boardMember
{
private:
	int level;
	//raw stats
	int HP;
	int ATK;
	int DFN;
	int POW;
public:
	boardMember();
	~boardMember();
	void statSet(int newHP, int newPOW, int newATK, int newDFN);
	int levelUP(){ return ++level; }
	void setLvl(int levelToSet){ level += levelToSet; }
	unsigned int ATKcalc(boardMember);
	int getLevel(){ return level; }
	int getHP(){ return HP; }
	void decHP(int amount){ HP -= amount; }
	int getATK(){ return ATK; }
	int getDEF(){ return DFN; }
	int getPOW(){ return POW; }

};

class Allie : public boardMember
{
private:
	long int expr;
public:
	Allie();
	~Allie();
	int expUP(int XPtoUP){ return expr += XPtoUP; }
	int checkForLvlUp();

};

class Foe : public boardMember
{
private:
	int xPos;
	int yPos;
public:
	Foe();
	~Foe();
	void setCoor(int x, int y){ xPos = (TILE_WIDTH); yPos = (TILE_HEIGHT); }
	int XPgiven(Foe);

	int getXpos(){ return xPos; }
	int getYpos(){ return yPos; }
};

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

private:
	//The attributes of the tile
	SDL_Rect mBox;

	//The tile type
	int mType;
};


//Checks collision box against set of tiles
bool touchesWall(SDL_Rect box, Tile* tiles[]);

//Sets tiles from tile map
bool setTiles(Tile *tiles[]);

//The dot that will move around on the screen
class Dot
{
	public:
		//The dimensions of the dot
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		//Maximum axis velocity of the dot
		const float DOT_VEL = .99;

		//Initializes the variables
		Dot();

		//Takes key presses and adjusts the dot's velocity
		void handleEvent(SDL_Event& e);

		//Moves the dot and check collision against tiles
		void move(Tile *tiles[]);

		//Shows the dot on the screen
		void render();

	private:
		//Collision box of the dot
		SDL_Rect mBox;

		//The velocity of the dot
		float mVelX, mVelY;
};


#endif