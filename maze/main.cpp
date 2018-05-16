#include "maze.h"

int main()
{
	auto seed = std::random_device()();

	Maze maze(51, 51, seed);

	maze.generate();

	return 0;
}
