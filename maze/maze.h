#pragma once
#include "table.h"
#include <random>
#include <cassert>
#include <stack>
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>

class Maze : public Table<char>
{
public:
	Maze(int32_t width, int32_t height, uint32_t seed);

	bool open(const std::string & file);
	
	void generate();

	bool find();

	friend std::ostream & operator<<(std::ostream & os, Maze & maze);

private:

	std::vector<Vec2i> getNeighbours(Vec2i index);
	
	bool isBorder(Vec2i index);

	Vec2i randomBorder();

	Vec2i randomNotBorder();

	const static char char_wall = '*',
			char_path  = ' ',
			char_start = 'S',
			char_solve = '+',
			char_end   = 'X',
			char_found = 'x';

	std::mt19937 m_gen;

};
