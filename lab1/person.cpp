#include "person.h"

std::istream & operator>>(std::istream & in, Person & p)
{
	if(in.peek() == -1) //Om filen är vid sitt slut
	{
		return in;  //Avbryt
	}

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

	if(in.peek() != ' ') //Om nästa tecken inte är mellanslag
	{
		while(in.peek() != ' ')
		{
			//Plocka strängen
			std::string citySecond;
			std::getline(in, citySecond, ' ');
			
			//Lägg på den inlästa strängen till den fullständiga
			//strängen
			p.address.city += ' ' + citySecond;
		}
	}
	else
	{
		//Ignorera samtliga tecken till nästa radbrytning
		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	return in;
}

std::string toUpper(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);

	while(str.find("Å") != std::string::npos)
		str.replace(str.find("Å"), 2, "å");
	while(str.find("Ä") != std::string::npos)
		str.replace(str.find("Ä"), 2, "ä");
	while(str.find("Ö") != std::string::npos)
		str.replace(str.find("Ö"), 2, "ö");

	return str;
}

std::vector<Person> read_file(const std::string & filename)
{
	std::ifstream file(filename.c_str());
	std::vector<Person> persons;

	if(!file.is_open())	//Om filen ej öppnats, avbryt meddetsamma
		return persons;

	Person p;

	while(file >> p)	//Plocka in alla personer stegvis ur filen in i vectorn
		persons.push_back(p);

	return persons;
}


size_t find_in_names(const std::vector<Person> & persons, std::string name_part)
{

	name_part = toUpper(name_part);
			
	//Räkna bara de personer som har söksträngen som en substräng inom sig
	return	std::count_if(persons.begin(), persons.end(), [&](const Person & p)
	{
		std::string tmp = p.name;

		tmp = toUpper(tmp);
		
		return tmp.find(name_part) != std::string::npos;
	});
}

std::vector<Person> find_person_from_city(const std::vector<Person> & haystack, 
		std::string city)
{
	std::vector<Person> persons_found;
	city = toUpper(city);

	for(const auto & p : haystack)
	{
		std::string tmp = p.address.city;
	
		tmp = toUpper(tmp);
		
		//Om nuvarande stad matchar vår sökterm, så ska denna person med
		if(tmp == city) persons_found.push_back(p);
	}

	return persons_found;
}
