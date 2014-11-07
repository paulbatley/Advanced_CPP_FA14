#ifndef _PROJECTHEADER_H_
#define _PROJECTHEADER_H_

#include <SDL.h>
#include <SDL_image.h>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <random>

const int tileSize = 20;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;



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
	void setCoor(int x, int y){ xPos = (x*tileSize); yPos = (y*tileSize); }
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




#endif