#include "maze.h"

int main()
{
	auto seed = std::random_device()();

	Maze maze(21, 21, seed);

	auto begin = std::chrono::system_clock::now();

	maze.generate();

	auto end = std::chrono::system_clock::now();

	auto time_build = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

	begin = std::chrono::system_clock::now();

	bool solveable = maze.find();

	end = std::chrono::system_clock::now();

	auto time_search = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

	std::cout << "Building took " << time_build.count() << "ms\n"
		<< "Searching took " << time_search.count() << "ms\n"
		<< "Solveable: " << std::boolalpha << solveable << '\n'
		<< maze;

	
	begin = std::chrono::system_clock::now();

	for(uint64_t i = 0; i < 10000; i++)
	{
		maze.generate();
		solveable = maze.find();
	}

	end = std::chrono::system_clock::now();

	
	auto time_gen = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

	std::cout << "Time it took to generate and search 10000 mazes " << time_gen.count() << "ms\n";

	return 0;
}
