#include "Dungeon.h"


enum Corner { UPPER_LEFT, UPPER_RIGHT, LOWER_LEFT, LOWER_RIGHT };

//prints map showing only which rooms have been created
void Dungeon::printMap(){
	for (int row = 0; row < (DUNGEON_HEIGHT*DUNGEON_WIDTH); row++){
		cout << (map[row] == NULL ? "0" : "1");
		if (row != 0 && (row + 1) % DUNGEON_WIDTH == 0) cout << "\n";
	}
}


//prints map showing open doors in rooms
void Dungeon::printRoomsMap(){
	for (int i = 0; i < DUNGEON_HEIGHT; i++){
		for (int level = 0; level < 3; level++){
			switch (level){
			case 0:
				for (int j = 0; j < DUNGEON_WIDTH; j++){
					if (map[(i*DUNGEON_WIDTH) + j] != NULL){
						cout << " ";
						cout << (map[(i*DUNGEON_WIDTH) + j]->up ? "o" : "-");
						cout << " ";
					}
					else{
						cout << " - ";
					}
				}
				cout << endl;
				break;
			case 1:
				for (int j = 0; j < DUNGEON_WIDTH; j++){
					if (map[(i*DUNGEON_WIDTH) + j] != NULL){
						cout << (map[(i*DUNGEON_WIDTH) + j]->left ? "o" : "|");
						cout << " " << (map[(i*DUNGEON_WIDTH) + j]->right ? "o" : "|");
					}
					else{
						cout << "| |";
					}
				}
				cout << endl;
				break;
			case 2:
				for (int j = 0; j < DUNGEON_WIDTH; j++){
					if (map[(i*DUNGEON_WIDTH) + j] != NULL){
						cout << " ";
						cout << (map[(i*DUNGEON_WIDTH) + j]->down ? "o" : "-");
						cout << " ";
					}
					else{
						cout << " - ";
					}
				}
				cout << endl;
				break;
				cout << endl;
			}

		}
	}
}


//returns x position from given index of 2D array
int Dungeon::findX(int index){
	return index / DUNGEON_WIDTH;
}


//returns x position from given index of 2D array
int Dungeon::findY(int index){
	return index % DUNGEON_WIDTH; 
}


/* Creates room at the given index.  Opens room's doors if there are rooms already next to it,
   randomly tries to open doors if not and creates new rooms through those randomly opened doors. 
   Recursive.  */
void Dungeon::generateRoom(int index)
{
	//DELETE
	cout << "total rooms: " << totalRooms << endl;
	//room out of bounds
	if (index >= (DUNGEON_HEIGHT*DUNGEON_WIDTH))
		return;

	//make the room at its coordinates
	map[index] = make_unique<Room>(findX(index), findY(index));


	//random engine for shuffling doors
	random_device rd;
	mt19937 randomize(rd());

	//shuffle case switches which control which doors to open first
	vector<int> shuffled_doors = { 1, 2, 3, 4 };	
	shuffle(shuffled_doors.begin(), shuffled_doors.end(), randomize);

	//a random generator w/ a % chance of returning true
	tr1::bernoulli_distribution randOpen(CHANCE_OF_OPEN_DOOR);


	//go through each door and randomly try to open each
	for (int i = 0; i < 4; i++){
		switch (shuffled_doors[i]){
			//left
			case 1:
				if ((index % DUNGEON_WIDTH) != 0){
					if (map[index - 1] == NULL && totalRooms < MAX_ROOMS){
							map[index]->left = randOpen(randomize);
							//door opened, make a new room on the left
							if (map[index]->left){
								totalRooms++;
								generateRoom(index - 1);
							}
					}
				}
				break;

			//right
			case 2:
				if ((index + 1) < (DUNGEON_HEIGHT*DUNGEON_WIDTH) && ((index + 1) % DUNGEON_WIDTH) != 0){
					if (map[index + 1] == NULL && totalRooms < MAX_ROOMS){
							map[index]->right = randOpen(randomize);
							//door opened, make a new room on the right
							if (map[index]->right){
								totalRooms++;
								generateRoom(index + 1);
							}
					}
				}
				break;

			//up
			case 3:
				if ((index - DUNGEON_WIDTH) >= 0){
					if (map[index - DUNGEON_WIDTH] == NULL && totalRooms < MAX_ROOMS){
							map[index]->up = randOpen(randomize);
							//door opened, make a new room on the up
							if (map[index]->up){
								totalRooms++;
								generateRoom(index - DUNGEON_WIDTH);
							}
					}
				}
				break;

			//down
			case 4:
				if ((index + DUNGEON_WIDTH) < (DUNGEON_HEIGHT*DUNGEON_WIDTH)){
					if (map[index + DUNGEON_WIDTH] == NULL && totalRooms < MAX_ROOMS){
							map[index]->down = randOpen(randomize);
							//door opened, make a new room on the down
							if (map[index]->down){
								totalRooms++;
								generateRoom(index + DUNGEON_WIDTH);
							}
					}
				}
				break;
		}
	}

}





Dungeon::Dungeon()
{
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> dist(0, (DUNGEON_HEIGHT*DUNGEON_WIDTH) - 1);

	//make first room at a random place in the map
	++totalRooms;
	firstRoom = dist(mt);
	generateRoom(firstRoom);		//makes subsequent rooms from first room

	//goes through entire map and opens doors leading to adjacent rooms
	for (int i = 0; i < (DUNGEON_HEIGHT*DUNGEON_WIDTH); i++){
		if (map[i] != NULL){
			if ((i % DUNGEON_WIDTH) != 0 && map[i - 1] != NULL)
				map[i]->left = true;
			//right
			if (((i + 1) < (DUNGEON_HEIGHT*DUNGEON_WIDTH) && ((i + 1) % DUNGEON_WIDTH) != 0) && map[i + 1] != NULL)
				map[i]->right = true;
			//up
			if ((i - DUNGEON_WIDTH) >= 0 && map[i - DUNGEON_WIDTH] != NULL)
				map[i]->up = true;
			//down
			if ((i + DUNGEON_WIDTH) < (DUNGEON_HEIGHT*DUNGEON_WIDTH) && map[i + DUNGEON_WIDTH] != NULL)
				map[i]->down = true;
		}
	}

	//DELETE
	cout << endl;

	printMap();

	cout << endl;

	printRoomsMap();

}





