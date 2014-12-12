#ifndef ROOM_H
#define ROOM_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include "GlobalVar.h"
#include "Dungeon.h"
using namespace std;


struct Coordinates{
	int x;
	int y;
};


class Room
{
public:
	Room() { };
	Room(int, int, bool, bool, bool, bool);
	int findX(int);
	int findY(int);
	Coordinates coord;
	bool left = false, right = false;
	bool up = false, down = false;
	string filename;
	~Room() { remove(filename.c_str()); }

private:
	string FillNumber(int);
	int height = SCREEN_HEIGHT / TILE_HEIGHT;
	int width = SCREEN_WIDTH / TILE_WIDTH;
};

#endif