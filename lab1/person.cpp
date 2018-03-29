#include "person.h"

std::istream & operator>>(std::istream & in, Person & p)
{
	std::string zipA, zipB;
	
	std::getline(in, p.name);
	std::getline(in, p.id);
	std::getline(in, p.address.street, ',');

	in.ignore();	//Ignorera whitespace
	std::getline(in, zipA, ' ');	//Ta första delen av postnr
	std::getline(in, zipB, ' ');	//Ta andra delen av postnr
	p.address.zip = stoi(zipA + zipB);	//Slå ihop dem
	
	in.ignore(); //Ignorera whitespace
	std::getline(in, p.address.city, ' ');

	//Ignorera samtliga tecken till en ny rad
	in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	
	if(in.peek() == -1) in.ignore();


	return in;
}

std::vector<Person> read_file(const std::string & filename)
{
	std::ifstream file(filename.c_str());
	std::vector<Person> persons;

	if(!file.is_open())	//Om filen ej öppnats, avbryt meddetsamma
		return persons;

	Person p;

	do	//Plocka in alla personer stegvis ur filen in i vectorn	
	{
		file >> p;
		persons.push_back(p);
	}
	while(!file.eof());

	return persons;
}


size_t find_in_names(const std::vector<Person> & persons, std::string name_part)
{
	std::transform(name_part.begin(), name_part.end(), name_part.begin(), 
			::toupper);

	//Räkna bara de personer som har söksträngen som en substräng inom sig
	return	std::count_if(persons.begin(), persons.end(), [&](const Person & p)
	{
		std::string tmp = p.name;

		std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
		
		return tmp.find(name_part) != std::string::npos;
	});
}

std::vector<Person> find_person_from_city(const std::vector<Person> & haystack, 
		std::string city)
{
	std::vector<Person> persons_found;
	std::transform(city.begin(), city.end(), city.begin(), ::toupper);

	for(const auto & p : haystack)
	{
		std::string tmp = p.address.city;
	
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
		
		//Om nuvarande stad matchar vår sökterm, så ska denna person med
		if(tmp == city) persons_found.push_back(p);
	}

	return persons_found;
}
