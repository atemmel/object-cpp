#pragma once
#include "table.h"
#include <random>
#include <stack>
#include <vector>
#include <iostream>
#include <chrono>
#include <fstream>

class Maze : private Table<char>
{
public:

	//Constructor
	Maze(int32_t width, int32_t height, uint32_t seed);

	//Öppna från fil
	bool open(const std::string & file);
	
	//Bygg labyrint
	void generate();

	//Hitta i labyrint
	bool find();

	//För utströmmning
	friend std::ostream & operator<<(std::ostream & os, Maze & maze);

private:

	//Returnerar grannar vid ett givet index
	std::vector<Vec2i> getNeighbours(Vec2i index);
	
	//Kollar om ett index är en kant
	bool isBorder(Vec2i index);

	//Returnerar en slumpmässig kant
	Vec2i randomBorder();

	const static char char_wall = '*',
			char_path  = ' ',
			char_start = 'S',
			char_solve = '+',
			char_end   = 'X',
			char_found = 'x';

	std::mt19937 m_gen;

};
