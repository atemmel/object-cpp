#include "maze.h"
#include <getopt.h>

//Returnerar true om strängen är ett heltal
bool isInt(const std::string & str);
void help();
void version();

int main(int argc, char * argv[])
{

	char        arg = 0;
	std::string param  = "",
		inputFile  = "",	//Infil
		outputFile = "";	//Utfil

	bool 	check = false;		//Om bara lösningen skall
					//kontrolleras

	int32_t width  = -1,		//Bredd
		height = -1;		//Höjd

	//Funktionslambda för felmeddelanden
	auto error = [](const std::string & str)
	{
		std::cerr << "Error: " << str << '\n';
	};

	//Alternativ
	const char * short_opts = "vhs:c:r:i:o:b";

	//Långa alternativ
	const struct option long_opts[] =
	{
		{"version", 0, NULL, 'v'},
		{"help",    0, NULL, 'h'},
		{"size",    1, NULL, 's'},
		{"columns", 1, NULL, 'c'},
		{"rows",    1, NULL, 'r'},
		{"input",   1, NULL, 'i'},
		{"output",  1, NULL, 'o'},
		{"check",   0, NULL, 'b'}
	};

	while((arg = getopt_long(argc, argv, short_opts, long_opts, nullptr)) != -1)
	{
		switch(arg)
		{
			case 'v':	//Version
				version();
				return EXIT_SUCCESS;
			case 'h':	//Help
				help();
				return EXIT_SUCCESS;
			case 's':	//Size
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
			case 'c':	//Columns
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
			case 'r':	//Rows
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
			case 'i':	//Input
				inputFile = optarg;

				//Width och height sätts så att flödet kan passera en senare kontroll
				//Om input specificerats så kommer inte programmet bygga en labyrint 
				//under samma körning

				width = height = 5;
				break;
			case 'o':	//Output
				outputFile = optarg;
				break;
			case 'b':	//Check
				check = true;
				break;
			case '?':
			default:
				return EXIT_FAILURE;
		}
	}
	
	//Felhantering
	if(width < 5 || height < 5)
	{
		error("Ogiltiga eller opsecifierade dimensioner.\nLabyrinten får ej vara "
				"mindre än 5x5.\n"
				"För hjälp skriv -h.");
		return EXIT_FAILURE;
	}
	if(width % 2 == 0 || height % 2 == 0)
	{
		error("Ogilitga dimensioner.\nLabyrinten får bara ha "
				"udda dimensioner");
		return EXIT_FAILURE;
	}
		
	std::random_device rng;
	auto seed = rng();	//Hämta ett seed

	Maze maze(width, height, seed);

	if(!inputFile.empty())	//Om en fil skall läsas
	{
		if(!outputFile.empty())	//Om en fil skall skrivas
		{
			error("In och utfil definierad, "
					"otydligt vad användaren önskar");
			return EXIT_FAILURE;
		}


		if(maze.open(inputFile))	//Om filen gick att öppna
		{
			bool found = maze.find();
			if(check)
			{
				std::cout << "Labyrinten gick" << 
					(found ? " " : " inte ") <<
					"att lösa\n";
			}
			else if(found)
				std::cout << maze;
		}
		else
		{
			error("Labyrinten gick ej att öppna.\n"
					"Finns filen? "
					"Om filen är öppen, stäng den.");
			return EXIT_FAILURE;
		}

		return EXIT_SUCCESS;
	}
	
	//Annars ska ingen fil läsas, utan någonting skall genereras

	maze.generate();

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
