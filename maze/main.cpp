#include "maze.h"

int main()
{
	auto seed = std::random_device()();

	Maze maze(7, 7, seed);

	maze.generate();

	for(int i = 0; i < 7; i++)
	{
		for(int j = 0; j < 7; j++)
			std::cout << maze(j, i);
		puts("");
	}

	return 0;
}
