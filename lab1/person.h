#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <cctype>
#include <limits>

//Struct för lagrande av geografisk plats
struct Location 
{
	std::string street;
	std::string city;
	uint32_t zip;
};

//Struct för lagrande av person
struct Person 
{
	std::string name;
	std::string id;
	Location address;
};

//Egendefinierad inströmsoperator för personer
std::istream & operator>>(std::istream & in, Person & p);

//Läser alla personer ur en given fil
std::vector<Person> read_file(const std::string & filename);

//Hämtar antalet personer vars namn till viss del överränsstämmer med söktermen
size_t find_in_names(const std::vector<Person> & persons, std::string name_part);
 
//Hämtar en lista på personer som bor i en angiven stak
std::vector<Person> find_person_from_city(const std::vector<Person> & haystack,
                 std::string city);
