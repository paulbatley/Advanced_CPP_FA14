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
	/*SDL_Window* set(SDL_Window*);*/
	void set();
	bool updateMiniMap();
	void showHideMap();

private:
	int mWindowID;
	bool isHidden = false;
};


#endif
