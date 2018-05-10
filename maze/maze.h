#pragma once
#include <random>
#include <cassert>
#include <stack>
#include <vector>
#include <algorithm>
#include <iostream>
#include <thread>
#include <chrono>

struct Tile
{
	Tile(int32_t x = 0, int32_t y = 0) :
		x(x), y(y) {};

	bool operator==(const Tile & other);


	int32_t x, y;
};

class Booltable 
{
public:
	Booltable(uint32_t width, uint32_t height)
		: m_width(width), m_height(height)
	{
		m_data = new bool[width * height];
	}

	Booltable(const Booltable & other) = delete;
	Booltable operator=(const Booltable & other) = delete;

	~Booltable()
	{
		delete[] m_data;
	}

	bool & operator()(int32_t x, int32_t y)
	{
		return m_data[y * m_width + x];
	}

	bool & operator()(const Tile & other)
	{
		return m_data[other.y * m_width + other.x];
	}
private:
	uint32_t m_width, m_height;
	bool * m_data = 0;
};

class Maze
{
public:
	Maze(uint32_t width, uint32_t height, uint32_t seed);
	
	~Maze();

	char & operator()(uint32_t x, uint32_t y);

	char & operator()(Tile index);

	void generate();

	friend std::ostream & operator<<(std::ostream & os, Maze & maze);

private:

	std::vector<Tile> getNeighbours(Tile index);
	
	bool isBorder(Tile index);

	Tile randomBorder();

	Tile randomNotBorder();

	const static char char_wall = '*',
			char_path  = ' ',
			char_start = 'S',
			char_end   = 'X';

	std::mt19937 m_gen;

	uint32_t m_width, 
		 m_height, 
		 m_seed;

	char * m_data = nullptr;
};

