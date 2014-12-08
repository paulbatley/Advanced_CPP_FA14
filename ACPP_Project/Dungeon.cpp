#include "Dungeon.h"


enum Corner { UPPER_LEFT, UPPER_RIGHT, LOWER_LEFT, LOWER_RIGHT };

//prints map showing only which rooms have been created
void Dungeon::printMap(){
	for (int row = 0; row < (dungeonHeight*dungeonWidth); row++){
		cout << (map[row] == NULL ? "0" : "1");
		if (row != 0 && (row + 1) % dungeonWidth == 0) cout << "\n";
	}
}


//prints map showing open doors in rooms
void Dungeon::printRoomsMap(){
	for (int i = 0; i < dungeonHeight; i++){
		for (int level = 0; level < 3; level++){
			switch (level){
			case 0:
				for (int j = 0; j < dungeonWidth; j++){
					if (map[(i*dungeonWidth) + j] != NULL){
						cout << " ";
						cout << (map[(i*dungeonWidth) + j]->up ? "o" : "-");
						cout << " ";
					}
					else{
						cout << " - ";
					}
				}
				cout << endl;
				break;
			case 1:
				for (int j = 0; j < dungeonWidth; j++){
					if (map[(i*dungeonWidth) + j] != NULL){
						cout << (map[(i*dungeonWidth) + j]->left ? "o" : "|");
						cout << " " << (map[(i*dungeonWidth) + j]->right ? "o" : "|");
					}
					else{
						cout << "| |";
					}
				}
				cout << endl;
				break;
			case 2:
				for (int j = 0; j < dungeonWidth; j++){
					if (map[(i*dungeonWidth) + j] != NULL){
						cout << " ";
						cout << (map[(i*dungeonWidth) + j]->down ? "o" : "-");
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
	return index / dungeonWidth;
}


//returns x position from given index of 2D array
int Dungeon::findY(int index){
	return index % dungeonWidth; 
}


/* Creates room at the given index.  Opens room's doors if there are rooms already next to it,
   randomly tries to open doors if not and creates new rooms through those randomly opened doors. 
   Recursive.  */
void Dungeon::generateRoom(int index)
{
	//DELETE
	cout << "total rooms: " << totalRooms << endl;
	//room out of bounds
	if (index >= (dungeonHeight*dungeonWidth))
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
				if ((index % dungeonWidth) != 0){
					if (map[index - 1] == NULL && totalRooms < maxRooms){
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
				if ((index + 1) < (dungeonHeight*dungeonWidth) && ((index + 1) % dungeonWidth) != 0){
					if (map[index + 1] == NULL && totalRooms < maxRooms){
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
				if ((index - dungeonWidth) >= 0){
					if (map[index - dungeonWidth] == NULL && totalRooms < maxRooms){
							map[index]->up = randOpen(randomize);
							//door opened, make a new room on the up
							if (map[index]->up){
								totalRooms++;
								generateRoom(index - dungeonWidth);
							}
					}
				}
				break;

			//down
			case 4:
				if ((index + dungeonWidth) < (dungeonHeight*dungeonWidth)){
					if (map[index + dungeonWidth] == NULL && totalRooms < maxRooms){
							map[index]->down = randOpen(randomize);
							//door opened, make a new room on the down
							if (map[index]->down){
								totalRooms++;
								generateRoom(index + dungeonWidth);
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
	uniform_int_distribution<int> dist(0, (dungeonHeight*dungeonWidth) - 1);

	//make first room at a random place in the map
	++totalRooms;
	firstRoom = dist(mt);
	generateRoom(firstRoom);		//makes subsequent rooms from first room

	//goes through entire map and opens doors leading to adjacent rooms
	for (int i = 0; i < (dungeonHeight*dungeonWidth); i++){
		if (map[i] != NULL){
			if ((i % dungeonWidth) != 0 && map[i - 1] != NULL)
				map[i]->left = true;
			//right
			if (((i + 1) < (dungeonHeight*dungeonWidth) && ((i + 1) % dungeonWidth) != 0) && map[i + 1] != NULL)
				map[i]->right = true;
			//up
			if ((i - dungeonWidth) >= 0 && map[i - dungeonWidth] != NULL)
				map[i]->up = true;
			//down
			if ((i + dungeonWidth) < (dungeonHeight*dungeonWidth) && map[i + dungeonWidth] != NULL)
				map[i]->down = true;
		}
	}

	//DELETE
	cout << endl;

	printMap();

	cout << endl;

	printRoomsMap();

}





