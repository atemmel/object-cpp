#include "maze.h"

Maze::Maze(int32_t width, int32_t height, uint32_t seed)
	: Table<char>(width, height, char_wall)
{
	assert(width > 2 && height > 2 && "Assert: Bad dimension");
	
	m_gen.seed(m_seed);
}

void Maze::generate()
{
	Vec2i start(0,1);
	Vec2i end   = randomBorder();

	while(end == start) end = randomBorder();

	(*this)(start)	= char_start;
	(*this)(end)	= char_end;
	(*this)(1,1)	= '+';

	std::stack<Vec2i> history;

	Table<bool> table(m_width, m_height, false);

	std::vector<Vec2i> list;
	list.push_back(Vec2i(1,1));

	auto accepted = [&](const Vec2i & tile) 
	{
		auto close = getNeighbours(tile);
		
		int32_t n_found = 0;
		for(auto & l : close)
		{
			if(table(l))
			{
				if(n_found > 1) return false;
				else ++n_found;
			}
		}

		return true;
	};

	do
	{
		while(!list.empty())
		{
			table(list.back()) = true;
			history.push(list.back());
			list = getNeighbours(history.top());

			for(size_t i = 0; i < list.size(); i++)
			{
				auto elem	= list[i];
				auto st 	= !accepted(elem);

				if(isBorder(elem) || table(elem) || st)
				{
					list.erase(list.begin() + i);
					--i;
				}
			}
			
			std::shuffle(list.begin(), list.end(), m_gen);

			history.push(list.back());
			(*this)(list.back()) = char_path;

			system("clear");
			std::cout << *this;
			std::this_thread::sleep_for(
				std::chrono::milliseconds(200));
		}

		
		history.pop();
		while(list.empty() && !history.empty())
		{
			assert(!history.empty());

			list = getNeighbours(history.top());
			
			for(size_t i = 0; i < list.size(); i++)
			{
				auto elem	= list[i];
				auto st 	= !accepted(elem);

				if(isBorder(elem) || table(elem) || st)
				{
					list.erase(list.begin() + i);
					--i;
				}
			}
			history.pop();
		}

		std::shuffle(list.begin(), list.end(), m_gen);
	}
	while(!history.empty());
	
	//std::cout << *this;
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
