#include "maze.h"

int main()
{
	auto seed = std::random_device()();

	Maze maze(21, 21, seed);

	maze.generate();

	return 0;
}
