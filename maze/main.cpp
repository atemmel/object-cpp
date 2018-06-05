#include "maze.h"
#include <getopt.h>

bool isInt(const std::string & str);
void help();
void version();

int main(int argc, char * argv[])
{
	char        c = 0;
	char *      cvalue = nullptr;
	std::string param  = "",
		inputFile  = "",
		outputFile = "";

	//v : Version
	//h : Help
	//s : Size
	//c : Columns
	//r : Rows
	//i : Input
	//o : Output
	//b : Check
	//
	//långa parametrar
	
	int32_t width  = -1,
		height = -1;

	auto error = [](const std::string & str)
	{
		std::cerr << "Error: " << str << '\n';
	};

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
					return EXIT_FAILURE;
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
					return EXIT_FAILURE;
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
					return EXIT_FAILURE;
				}
				break;
			case 'i':
				inputFile = optarg;
				width = height = 5;
				break;
			case 'o':
				outputFile = optarg;
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
		error("Ogiltiga dimensioner.\nLabyrinten får ej vara "
				"mindre än 5x5.");
		return EXIT_FAILURE;
	}
	if(width % 2 == 0 || height % 2 == 0)
	{
		error("Ogilitga dimensioner.\nLabyrinten får bara ha "
				"udda dimensioner");
		return EXIT_FAILURE;
	}
		
	std::random_device rng;
	auto seed = rng();

	Maze maze(width, height, seed);

	if(!inputFile.empty())
	{
		if(!outputFile.empty())
		{
			error("In och utfil definierad, "
					"otydligt vad som användarenönskas");
			return EXIT_FAILURE;
		}
		if(maze.open(inputFile))
		{
			bool found =maze.find();
			std::cout << maze;
		}
		else
		{
			error("Labyrinten gick ej att öppna");
			return EXIT_FAILURE;
		}

		return EXIT_SUCCESS;
	}
	
	maze.generate();

	//maze.find();

	std::cout << maze;

	if(!outputFile.empty())
	{
		std::ofstream file(outputFile.c_str());
		if(file.is_open())
		{
			file << maze;
		}
		else
		{
			error("Labyrinten gick ej att exportera");
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
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
		" --version      | -v Skriver ut versionsnummer.\n\t"
		" --help         | -h Skriver ut tillgängliga argument.\n\t"
		"(--size         | -s)N. Skapa en labyrint med storleken N.\n\t"
		"(--columns      | -c)W. Skapa en labyrint med bredden W.\n\t"
		"(--rows         | -r)H. Skapa en layrint med höjden H.\n\t"
		"(--input        | -i)file. Använd filen file som indata.\n\t"
		"(--output       | -o)file. Använd filen file som utdata. Annars cout.\n\t"
		" --check        | -b. Skriver bara ut om labyrinten går att lösa.\n";
}

void version()
{
	std::cout << "Version 1.0\n";
}
