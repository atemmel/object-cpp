#include <iostream>
#include <vector>
#include <string>

const uint64_t ki = 1024;

#ifdef _WIN32
#define CLEARSTR "cls"
#else 
#define CLEARSTR "clear"
#endif

struct Kibibyte
{
	int8_t i[ki] = {0};
};

struct Mebibyte
{
	Kibibyte k[ki];
};

struct Gibibyte
{
	Mebibyte m[ki];
};

int main()
{
	std::vector<Gibibyte*> gibiboys;
	std::vector<Mebibyte*> mebiboys;
	std::vector<Kibibyte*> kibiboys;

	int choice = -1;
	std::string input;
	
	try
	{
		while (choice != 4)
		{
			uint64_t n_bytes = sizeof(Kibibyte) * kibiboys.size() + sizeof(Mebibyte) * mebiboys.size() +
				sizeof(Gibibyte) * gibiboys.size();

			double n_kib = n_bytes / ki, n_mib = n_kib / ki, n_gib = n_mib / ki;

			system(CLEARSTR);

			std::cout <<  n_bytes << " bytes allocated\n" <<
				n_kib << " KiB allocated\n" <<
				n_mib << " MiB allocated\n" <<
				n_gib << " GiB allocated\n\n" <<
				"1. Allocate one KiB\n"
				"2. Allocate one MiB\n"
				"3. Allocate one GiB\n"
				"4. Exit and free memory\n";

			std::getline(std::cin, input);
			if (!input.empty()) choice = input.front() - '0';
			else choice = -1;

			if (choice == 1)
				kibiboys.emplace_back(new Kibibyte);
			else if (choice == 2)
				mebiboys.emplace_back(new Mebibyte);
			else if (choice == 3)
				gibiboys.emplace_back(new Gibibyte);
		}
	}
	catch (std::bad_alloc e)
	{
		std::cerr << "Caught error: " << e.what() << '\n' << 
			"Pressing any key will free memory\n";
		std::cin.get();
	}

	std::cout << "Freeing memory!" << '\n';

	for (int i = 0; i < gibiboys.size(); i++)
		delete gibiboys[i];
	for (int i = 0; i < mebiboys.size(); i++)
		delete mebiboys[i];
	for (int i = 0; i < kibiboys.size(); i++)
		delete kibiboys[i];

	return 0;
}
