#include "Room.h"

string Room::FillNumber(int num){
	stringstream fill;
	fill << setfill('0') << std::setw(2) << num;
	return fill.str();
}

Room::Room(int x, int y){
	coord.x = x;
	coord.y = y;

	int wallLimit = (((2 * (height - 2)) + (2 * width)) / 4);

	filename = "map.map";
	stringstream subfilename;
	subfilename << setfill('0') << setw(2) << coord.x << setfill('0') << setw(2) << coord.y;
	filename.insert(3, subfilename.str());
	cout << filename << endl;

	ofstream file(filename);
	for (int i = 0; i < height; ++i)
	{
		int toLimit = 0;
		for (int j = 0; j < width; ++j)
		{
			if ((i == (height / 2) && (j == 0 || j == (width - 1))) || (j == (width / 2) && (i == 0 || i == (height - 1))))
			{
				file << FillNumber(1) << " ";
			}
			else if (i == 0 || i == (height - 1) || j == 0 || j == (width - 1) && (i != (height / 2) || j != (width / 2)))
			{
				file << FillNumber(0) << " ";
			}
			else
			{
				int num = rand() % TOTAL_TILE_SPRITES;
				if (num == 0 && toLimit != wallLimit)
				{
					file << FillNumber(num) << " ";
					++toLimit;
				}
				else if (num == 0 && toLimit == wallLimit)
				{
					num = (rand() % (TOTAL_TILE_SPRITES - 1)) + 1;
					file << FillNumber(num) << " ";
				}
				else
				{
					file << FillNumber(num) << " ";
				}
			}
		}
		file << "\n";
	}
	file.close();

	string line;
	ifstream file2(filename);
	if (file2.is_open())
	{
		while (getline(file2, line))
		{
			cout << line << '\n';
		}
		file2.close();
	}
}