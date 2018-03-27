#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <cctype>
#include <limits>

struct Location {
	std::string street;
	std::string city;
	uint32_t zip;
};


struct Person {
	std::string name;
	std::string id;
	Location address;
};

std::istream & operator>>(std::istream & in, Person & p);

std::vector<Person> read_file(const std::string & filename);

size_t find_in_names(const std::vector<Person> & persons, std::string name_part);
 
std::vector<Person> find_person_from_city(const std::vector<Person> & haystack,
                 std::string city);
