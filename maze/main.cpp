#include "maze.h"

int main()
{
	auto seed = std::random_device()();

	Maze maze(25, 25, seed);

	maze.generate();

	return 0;
}
