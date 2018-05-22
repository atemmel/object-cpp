#include "maze.h"

Maze::Maze(int32_t width, int32_t height, uint32_t seed)
	: Table<char>(width, height, char_wall)
{
	assert(width > 2 && height > 2 && "Assert: Bad dimension");
	
	m_gen.seed(m_seed);
}

void Maze::generate()
{
	Vec2i start(1,1);

	for(int32_t i = 0; i < m_width; i++)
		for(int32_t j = 0; j < m_height; j++)
			(*this)(i,j) = char_wall;


	//(*this)(start)	= char_start;
	//(*this)(end)	= char_end;
	(*this)(1,1)	= char_start;

	std::stack<Vec2i> history;

	Table<bool> table(m_width, m_height, false);

	std::vector<Vec2i> neighbours;
	neighbours.push_back(Vec2i(1,1));

	bool endSet = false;

	auto accepted = [&](const Vec2i & tile) 
	{
		auto close = getNeighbours(tile);
		
		int32_t n_found = 0;
		for(auto & l : close)
		{
			if(table(l))
			{
				if(n_found > 0) return false;
				else ++n_found;
			}
		}

		return true;
	};

	do
	{
		while(!neighbours.empty())
		{
			table(neighbours.back()) = true;
			history.push(neighbours.back());
			neighbours = getNeighbours(history.top());

			for(size_t i = 0; i < neighbours.size(); i++)
			{
				auto elem	= neighbours[i];
				auto st 	= !accepted(elem);

				if(isBorder(elem) || table(elem) || st)
				{
					neighbours.erase(neighbours.begin() + i);
					--i;
				}
			}
			
			std::shuffle(neighbours.begin(), neighbours.end(), m_gen);

			history.push(neighbours.back());
			(*this)(neighbours.back()) = char_path;
			
			/*
			system("clear");
			std::cout << *this;
			std::this_thread::sleep_for(
				std::chrono::milliseconds(200));*/
		}

		history.pop();

		while(neighbours.empty() && !history.empty())
		{
			neighbours = getNeighbours(history.top());
			
			for(size_t i = 0; i < neighbours.size(); i++)
			{
				auto elem	= neighbours[i];
				auto st 	= !accepted(elem);

				if(isBorder(elem) || table(elem) || st)
				{
					neighbours.erase(neighbours.begin() + i);
					--i;
				}
			}
			history.pop();
		}

		std::shuffle(neighbours.begin(), neighbours.end(), m_gen);
		(*this)(neighbours.back()) = char_path;
	}
	while(!history.empty() && !history.empty());

	Vec2i end;

	do
	{
		end = randomBorder();
		neighbours = getNeighbours(end);

		for(size_t i = 0; i < neighbours.size(); i++)
		{
			if((*this)(neighbours[i]) != char_path)
			{
				neighbours.erase(neighbours.begin() + i);
				--i;
			}
		}

	}
	while(neighbours.empty() || end == start);

	(*this)(end) = char_end;
	
	//std::cout << *this;
}

bool Maze::find()
{
	Vec2i start;

	for(int32_t i = 0; i < m_width; i++)
		for(int32_t j = 0; j < m_height; j++)
			if((*this)(i,j) == char_start)
				start = Vec2i(i,j);

	std::stack<Vec2i> history;
	std::vector<Vec2i> neighbours;
	Table<bool> visited(m_width, m_height, false);

	neighbours.push_back(start);

	bool foundExit = 0;

	Vec2i end;

	do
	{
		while(!neighbours.empty())
		{
			history.push(neighbours.back());
			(*this)(neighbours.back()) = 'S';
			visited(neighbours.back()) = true;
			neighbours = getNeighbours(history.top());

			for(size_t i = 0; i < neighbours.size(); i++)
			{
				//Delete unwanted
				if((*this)(neighbours[i]) == char_wall || visited(neighbours[i]))
				{
					neighbours.erase(neighbours.begin() + i);
					--i;
				}
				else if((*this)(neighbours[i]) == char_end)
				{
					foundExit = 1;
					return true;
				}
			}

			std::shuffle(neighbours.begin(), neighbours.end(), m_gen);


		/*	system("clear");
			std::cout << *this;
			auto s = history.top();
			std::cout << s.x << ',' << s.y << '\n';
			std::this_thread::sleep_for(
				std::chrono::milliseconds(200));*/
		}

		//history.pop();
		do
		{
			neighbours = getNeighbours(history.top());
			history.pop();
			neighbours = getNeighbours(history.top());

			for(size_t i = 0; i < neighbours.size(); i++)
			{
				//Delete unwanted
				if((*this)(neighbours[i]) == char_wall || visited(neighbours[i]))
				{
					neighbours.erase(neighbours.begin() + i);
					--i;
				}
			}
			
			end = history.top();
		}
		while(neighbours.empty() && !history.empty());

		std::shuffle(neighbours.begin(), neighbours.end(), m_gen);
	}
	while(!foundExit && !history.empty() || !neighbours.empty());

	std::cout << end.x << ',' << end.y << '\n';

	return !history.empty();
}

std::ostream & operator<<(std::ostream & os, Maze & maze)
{
	for(int32_t i = 0; i < maze.m_height; i++)
	{
		for(int32_t j = 0; j < maze.m_width; j++)
			os << maze(j, i);
		os << '\n';
	}
	return os;
}

std::vector<Vec2i> Maze::getNeighbours(Vec2i index)
{
	std::vector<Vec2i> list;
	
	if(index.x > 0)			list.push_back(Vec2i(index.x - 1, index.y));
	if(index.x < m_width - 1)	list.push_back(Vec2i(index.x + 1, index.y));
	if(index.y > 0)			list.push_back(Vec2i(index.x, index.y - 1));
	if(index.y < m_height - 1)	list.push_back(Vec2i(index.x, index.y + 1));

	return list;
}

bool Maze::isBorder(Vec2i index)
{
	return index.x == 0 ||
		index.x == m_width  - 1 ||
		index.y == 0 ||
		index.y == m_height - 1;
}

Vec2i Maze::randomBorder()
{
	Vec2i pos;

	std::uniform_int_distribution<int32_t> wDist(1, m_width  - 2),
					hDist(1, m_height - 2),
					boolean(0,1);

	pos.x = wDist(m_gen);
	pos.y = hDist(m_gen);

	boolean(m_gen) == 1 ? pos.x = 0 : pos.y = 0;

	return pos;
}

Vec2i Maze::randomNotBorder()
{
	Vec2i pos;

	std::uniform_int_distribution<int32_t> wDist(1, m_width  - 2),
					hDist(1, m_height - 2);
	pos.x = wDist(m_gen);
	pos.y = hDist(m_gen);

	return pos;
}
