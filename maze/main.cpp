#include "maze.h"
#include <getopt.h>

void error(const std::string & str);
bool isInt(const std::string & str);
void help();
void version();

int main(int argc, char * argv[])
{
	char        c = 0;
	char *      cvalue = nullptr;
	std::string param;

	//v : Version
	//h : Help
	//s : Size
	//c : Columns
	//r : Rows
	//i : Input
	//o : Output
	//b : Check
	
	int32_t width  = -1,
		height = -1;

	while((c = getopt(argc, argv, "vhs:c:r:i:o:b")) != -1)
	{
		switch(c)
		{
			case 'v':
				version();
				return 0;
			case 'h':
				help();
				return 0;
			case 's':
				param = optarg;
				if(isInt(param))
				{
					width = height = stoi(param);
				}
				else
				{
					error("Size (s) måste följas av ett heltal");
				}
				break;
			case 'c':
				param = optarg;
				if(isInt(param))
				{
					width = stoi(param);
				}
				else
				{
					error("Columns (c) måste följas av ett heltal");
				}
				break;
			case 'r':
				param = optarg;
				if(isInt(param))
				{
					height = stoi(param);
				}
				else
				{
					error("Rows (r) måste följas av ett heltal");
				}
				break;
			case 'i':
				puts("Input");
				cvalue = optarg;
				puts(cvalue);
				break;
			case 'o':
				puts("Output");
				cvalue = optarg;
				puts(cvalue);
				break;
			case 'b':
				puts("Check");
				break;
			case '?':
			default:
				return 0;
		}
	}

	if(width < 5 || height < 5)
	{
		std::cerr << "Ogiltiga dimensioner. "
			"Labyrinten får ej vara mindre än 5x5.\n"; 
		return -1;
	}
	if(width % 2 == 0 || height % 2 == 0)
	{
		std::cerr << "Ogiltiga dimensioner. "
			"Labyrinten får bara ha udda dimensioner.\n";
		return -1;
	}
			
	auto seed = std::random_device()();

	Maze maze(width, height, seed);

	auto begin = std::chrono::system_clock::now();

	maze.generate();

	auto end = std::chrono::system_clock::now();

	maze.find();

	std::cout << maze;

	auto time_build = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

/*	begin = std::chrono::system_clock::now();

	bool solveable = maze.find();

	end = std::chrono::system_clock::now();

	auto time_search = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);*/

	std::cout << "Building took " << time_build.count() << "ms\n"
//		<< "Searching took " << time_search.count() << "ms\n"
//		<< "Solveable: " << std::boolalpha << solveable << '\n'
	;//		<< maze;

	std::ofstream file("nej");

	file << maze;
	
/*	begin = std::chrono::system_clock::now();

	for(uint64_t i = 0; i < 10000; i++)
	{
		maze.generate();
		solveable = maze.find();
	}

	end = std::chrono::system_clock::now();

	
	auto time_gen = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

	std::cout << "Time it took to generate and search 10000 mazes " << time_gen.count() << "ms\n";*/

	return 0;
}

void error(const std::string & str)
{
	std::cerr << "Error: " << str << '\n';
	exit(1);
}

bool isInt(const std::string & str)
{
	for(const char & c : str)
	{
		if(!isdigit(c))
		{
			return false;
		}
	}

	return true;
}

void help()
{
	std::cout <<
		"\t"
		"--version      | -v Skriver ut versionsnummer.\n\t"
		"--help         | -h Skriver ut tillgängliga argument.\n\t"
		"(--size        | -s)N. Skapa en labyrint med storleken N.\n\t"
		"(--columns     | -c)W. Skapa en labyrint med bredden W.\n\t"
		"(--rows        | -r)H. Skapa en layrint med höjden H.\n\t"
		"(--input       | -i)file. Använd filen file som indata.\n\t"
		"(--output      | -o)file. Använd filen file som utdata. Annars cout.\n\t"
		"--check        | -b. Skriver bara ut om labyrinten går att lösa.\n";
}

void version()
{
	std::cout << "Version 1.0\n";
}
