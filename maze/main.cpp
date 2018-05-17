#include "maze.h"

int main()
{
	auto seed = std::random_device()();

	Maze maze(21, 21, seed);

	auto begin = std::chrono::system_clock::now();

	maze.generate();

	auto end = std::chrono::system_clock::now();

	auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

	std::cout << "Took " << time.count() << "ms\n";

	return 0;
}
