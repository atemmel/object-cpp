#include <iostream>
#include "person.h"

void printmenu();

int main(int argc, char ** argv)
{
	auto list = read_file("names.txt");

	uint8_t selection = 0;
	std::string input;

	while(selection != 3)
	{
		printmenu();

		do 
		{
			std::getline(std::cin, input);
		} 
		while(input.empty() || !isdigit(input.front()));

		selection = input.front() - '0';

		if(selection == 1)
		{
			system("clear");
			std::cout << "Skriv ett namn att söka efter: ";
			std::string searchstr;
			getline(std::cin, searchstr);

			auto count = find_in_names(list, searchstr);

			std::cout << "Hittade " << count << " personer med liknande namn.\n";
			std::cin.get();
		}
		else if(selection == 2)
		{
			system("clear");
			std::cout << "Skriv en ort att söka efter: ";
			std::string searchstr;
			getline(std::cin, searchstr);

			auto sublist = find_person_from_city(list, searchstr);

			if(!sublist.empty())
			{
				for(const auto & person : sublist)
				{
					std::cout << "Hittade " << person.name << " som bor i " 
						<< person.address.city << '\n';
				}
			}
			else 
			{
				std::cout << "Hittade inga personer som bor i " << 
					searchstr << '\n';
			}
			std::cin.get();

		}

	}

	return 0;
}

void printmenu()
{
	system("clear");
	std::cout << 	"1 - Sök del av personnamn."
			"\n2 - Sök städer."
			"\n3 - Avsluta."
			"\n: ";
}
