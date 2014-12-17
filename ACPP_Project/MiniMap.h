#ifndef MINIMAP_H
#define MINIMAP_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include "GlobalVar.h"
using namespace std;


class MiniMap{
public:
	MiniMap() {};
	bool init();
	bool loadMedia();
	void close();
	SDL_Window* set();
	bool updateMiniMap();

private:
	int mWindowID;
};


#endif
