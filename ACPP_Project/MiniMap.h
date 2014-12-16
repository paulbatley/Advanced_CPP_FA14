#ifndef MINIMAP_H
#define MINIMAP_H

#include <iostream>


class MiniMap{
public:
	MiniMap() {};
	bool init();
	bool loadMedia();
	void close();
	int set();
private:
	int mWindowID;
};


#endif
