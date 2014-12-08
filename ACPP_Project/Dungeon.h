#ifndef DUNGEON_H
#define DUNGEON_H

#include "Room.h"
#include <memory>		//smart pointers
#include <iostream>		//cout
//#include <cstdlib>		//rand()
//#include <ctime>		//to seed srand()
#include <random>		//random device
#include <algorithm>	//mt19937
#include <array>
#include <vector>
using namespace std;

const int dungeonWidth = 10;
const int dungeonHeight = 5;
const int maxRooms = 12;
const float CHANCE_OF_OPEN_DOOR = 0.7;


class Dungeon
{
public:
	Dungeon();
	int firstRoom;
	array<unique_ptr<Room>, (dungeonHeight*dungeonWidth)> map;
	int findX(int);
	int findY(int);
	void printMap();		//prints rooms created
	void printRoomsMap();	//prints rooms with open doors

private:
	void generateRoom(int);
	int totalRooms = 0;
};


#endif