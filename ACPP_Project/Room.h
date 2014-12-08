#ifndef ROOM_H
#define ROOM_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include "GlobalVar.h"
using namespace std;

//DELETE
//const int TOTAL_TILE_SPRITES = 4;
//const int dungeonWidth = 10;
//const int dungeonHeight = 5;

struct Coordinates{
	int x;
	int y;
};


class Room
{
public:
	Room() { };
	Room(int, int);
	Coordinates coord;
	bool left = false, right = false;
	bool up = false, down = false;
	string filename;
	~Room() { remove(filename.c_str()); }

private:
	string FillNumber(int);
	//int height = 12;
	//int width = 10;
	int height = SCREEN_HEIGHT / TILE_HEIGHT;
	int width = SCREEN_WIDTH / TILE_WIDTH;
};

#endif