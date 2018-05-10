#include "maze.h"

bool Tile::operator==(const Tile & other)
{
	return x == other.x && y == other.y;
}

Maze::Maze(uint32_t width, uint32_t height, uint32_t seed)
	: m_width(width), m_height(height), m_seed(seed)
{

	assert(width > 2 && height > 2 && "Assert: Bad dimension");

	m_data = new char[m_width * m_height] {char_wall};
	
	for(int i = 0; i < m_width * m_height; i++)
		m_data[i] = char_wall;

	m_gen.seed(m_seed);
}

Maze::~Maze()
{
	if(m_data) delete[] m_data;
}

char & Maze::operator()(uint32_t x, uint32_t y)
{
	return m_data[y * m_width + x];
}

char & Maze::operator()(Tile index)
{
	return m_data[index.y * m_width + index.x];
}

void Maze::generate()
{
	Tile start(0,1);
	Tile end   = randomBorder();

	while(end == start) end = randomBorder();

	(*this)(start) = char_start;
	(*this)(end)   = char_end;

	std::stack<Tile> history;

	Booltable table(m_width, m_height);

	std::vector<Tile> list;
	list.push_back(Tile(1,1));

	while(!list.empty())
	{
		std::shuffle(list.begin(), list.end(), m_gen);

		for(auto l : list)
		{
			history.push(l);	
			table(l) = true;
			(*this)(l) = char_path;
		}

		list = getNeighbours(history.top());

		for(int i = 0; i < list.size(); i++)
		{
			auto elem = list[i];
			if(isBorder(elem) || table(elem))
			{
				list.erase(list.begin() + i);
				--i;
			}
		}
	}
}

std::vector<Tile> Maze::getNeighbours(Tile index)
{
	std::vector<Tile> list;
	
	list.push_back(Tile(index.x - 1, index.y));
	list.push_back(Tile(index.x + 1, index.y));
	list.push_back(Tile(index.x, index.y - 1));
	list.push_back(Tile(index.x, index.y + 1));

	return list;
}

bool Maze::isBorder(Tile index)
{
	return index.x == 0 ||
		index.x == m_width  - 1 ||
		index.y == 0 ||
		index.y == m_height - 1;
}

Tile Maze::randomBorder()
{
	Tile pos;

	std::uniform_int_distribution<int> wDist(1, m_width  - 2),
					hDist(1, m_height - 2),
					boolean(0,1);

	pos.x = wDist(m_gen);
	pos.y = hDist(m_gen);

	boolean(m_gen) == 1 ? pos.x = 0 : pos.y = 0;

	return pos;
}

Tile Maze::randomNotBorder()
{
	Tile pos;

	
	std::uniform_int_distribution<int> wDist(1, m_width  - 2),
					hDist(1, m_height - 2);
	pos.x = wDist(m_gen);
	pos.y = hDist(m_gen);

	return pos;
}
