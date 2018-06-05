#include "maze.h"

Maze::Maze(int32_t width, int32_t height, uint32_t seed)
	: Table<char>(width, height, char_wall)
{
	m_gen.seed(seed);
}

bool Maze::open(const std::string & str)
{
	std::ifstream file(str);

	if(!file.is_open()) return false;

	std::vector<std::string> matrix;
	std::string row = "";

	int32_t prevWidth = 0;
	std::getline(file, row);
	prevWidth = row.size();
	matrix.push_back(row);
	while(std::getline(file, row))
	{
		matrix.push_back(row);
		if(prevWidth != row.size())	//Kolla om alla rader har samma längd
			return false;
		prevWidth = row.size();
	}

	if(matrix.empty()) return false;

	int32_t newWidth = matrix.front().size();
	int32_t newHeight = matrix.size();

	if(newWidth % 2 == 0 || newHeight % 2 == 0 || 
			newWidth < 5 || newHeight < 5)
		return false;

	//Har vi nått den här punkten så har vi lyckats läsa in!
	resize(newWidth, newHeight, char_wall);

	for(int32_t i = 0; i < newHeight; i++)
		for(int32_t j = 0; j < newWidth; j++)
		{
			(*this)(j,i) = matrix[i][j];
		}

	return true;
}

void Maze::generate()
{
	Vec2i start(0,1);

	for(int32_t i = 0; i < m_width; i++)
		for(int32_t j = 0; j < m_height; j++)
			(*this)(i,j) = char_wall;


	(*this)(start)	= char_start;

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
			if(neighbours.back() == Vec2i(0,7))
				std::cin.get();
			(*this)(neighbours.back()) = char_path;
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
			
		/*	system("clear");
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
	(*this)(1,1) = char_solve;
}

bool Maze::find()
{
	Vec2i start;

	if(m_width < 5 || m_height < 5)
		return false;

	bool foundStart = 0;
	for(int32_t i = 0; i < m_width; i++)
		for(int32_t j = 0; j < m_height; j++)
			if((*this)(i,j) == char_solve)
			{
				start = Vec2i(i,j);
				foundStart = 1;
			}

	if(!foundStart) return false;

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
					//neighbours.clear();
					//return true;
				}
			}

			if(foundExit) break;

			std::shuffle(neighbours.begin(), neighbours.end(), m_gen);


		/*	system("clear");
			std::cout << *this;
			auto s = history.top();
			std::cout << s.x << ',' << s.y << '\n';
			std::this_thread::sleep_for(
				std::chrono::milliseconds(200));*/
		}

		if(foundExit) break;

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

	while(!history.empty())
	{
		(*this)(history.top()) = char_found;
		history.pop();
	}

	return foundExit;
}

std::ostream & operator<<(std::ostream & os, Maze & maze)
{
	for(int32_t i = 0; i < maze.m_height; i++)
	{
		for(int32_t j = 0; j < maze.m_width; j++)
		{
			os << maze(j, i);
		}
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

	boolean(m_gen) == 1 ? pos.x = m_width - 1 : pos.y = m_height - 1;

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
