#include "maze.h"

Maze::Maze(int32_t width, int32_t height, uint32_t seed)
	: Table<char>(width, height, char_wall)
{
	m_gen.seed(seed);	//Seeda generatorn
}

bool Maze::open(const std::string & str)
{
	std::ifstream file(str);

	if(!file.is_open()) return false;

	std::vector<std::string> matrix;
	std::string row = "";

	//Hämta längden på första raden
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

	int32_t newWidth = matrix.front().size();
	int32_t newHeight = matrix.size();

	//Om de inlästa dimensioner ser fel ut, så gick inläsningen fel
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

	//Sätt allt till väggar
	for(int32_t i = 0; i < m_width; i++)
		for(int32_t j = 0; j < m_height; j++)
			(*this)(i,j) = char_wall;


	(*this)(start)	= char_start;

	std::stack<Vec2i> history;

	//Skapa en tabell med alla besökta
	Table<bool> table(m_width, m_height, false);

	std::vector<Vec2i> neighbours;
	neighbours.push_back(Vec2i(1,1));

	//Om slutet satts
	bool endSet = false;

	//Funktionslambda för att avgöra om en granne är besökbar eller ej
	auto accepted = [&](const Vec2i & tile) 
	{
		auto close = getNeighbours(tile);
		
		int32_t n_found = 0;
		for(auto & l : close)
		{
			//Om den är besökt
			if(table(l))
			{
				//Om antalet besökta grannar är > 0 avbryt, öka annars antalet
				if(n_found > 0) return false;
				else ++n_found;
			}
		}

		return true;
	};

	do
	{
		while(!neighbours.empty())	//Så länge vi har grannar
		{
			(*this)(neighbours.back()) = char_path;	//Skriv väg
			table(neighbours.back()) = true;	//Sätt besökt
			history.push(neighbours.back());	//Stoppa in grannen i stacken

			neighbours = getNeighbours(history.top());

			for(size_t i = 0; i < neighbours.size(); i++)
			{
				auto elem	= neighbours[i];
				auto accept 	= !accepted(elem);

				//Om grannen ej bör besökas
				if(isBorder(elem) || table(elem) || accept)
				{
					//Tag bort den
					neighbours.erase(neighbours.begin() + i);
					--i;
				}
			}
			
			//Blanda vectorn
			std::shuffle(neighbours.begin(), neighbours.end(), m_gen);
		}

		history.pop();

		//Om vi gått in i en återvändsgränd
		while(neighbours.empty() && !history.empty())
		{
			neighbours = getNeighbours(history.top()); //Hämta nya grannar
			
			//Ta bort oönskade grannar
			for(size_t i = 0; i < neighbours.size(); i++)
			{
				auto elem	= neighbours[i];
				auto accept 	= !accepted(elem);

				if(isBorder(elem) || table(elem) || accept)
				{
					neighbours.erase(neighbours.begin() + i);
					--i;
				}
			}
			history.pop();	//Backa bakåt i stacken
		}
		
		//Blanda grannar
		std::shuffle(neighbours.begin(), neighbours.end(), m_gen);
	}
	while(!history.empty());

	Vec2i end;
	
	//Skapa ett slut
	do
	{	
		//Hämta en slumpmässig kant och dess grannar
		end = randomBorder();
		neighbours = getNeighbours(end);

		//För varje granne
		for(size_t i = 0; i < neighbours.size(); i++)
		{
			//Om den inte är en väg, ta bort den
			if((*this)(neighbours[i]) != char_path) 
			{
				neighbours.erase(neighbours.begin() + i);
				--i;
			}
		}

	}
	while(neighbours.empty() || end == start);

	//Markera början och slut
	(*this)(end) = char_end;
	(*this)(1,1) = char_solve;
}

bool Maze::find()
{
	Vec2i start;

	//Om labyrinten är för liten så går den inte att lösa
	if(m_width < 5 || m_height < 5)
		return false;

	//Försök att hitta början
	bool foundStart = 0;
	for(int32_t i = 0; i < m_width; i++)
		for(int32_t j = 0; j < m_height; j++)
			if((*this)(i,j) == char_solve)
			{
				start = Vec2i(i,j);
				foundStart = 1;
			}

	//Om början inte finns så går det inte att hitta slutet
	if(!foundStart) return false;

	//Skapa en stack, grannlista och tabell för besökta celler
	std::stack<Vec2i> history;
	std::vector<Vec2i> neighbours;
	Table<bool> visited(m_width, m_height, false);

	neighbours.push_back(start);

	bool foundExit = 0;	//Om slutet hittats

	do
	{
		while(!neighbours.empty()) //Medans vi har grannar
		{
			history.push(neighbours.back());	//Stoppa in en i stacken
			visited(neighbours.back()) = true;	//Markera han som besökt

			neighbours = getNeighbours(history.top());	//Hämta nya grannar

			for(size_t i = 0; i < neighbours.size(); i++)
			{
				//Ta bort oönskade grannar
				if((*this)(neighbours[i]) == char_wall || visited(neighbours[i]))
				{
					neighbours.erase(neighbours.begin() + i);
					--i;
				}
				else if((*this)(neighbours[i]) == char_end)
				{
					foundExit = 1;
				}
			}

			if(foundExit) break;	//Om ett slut hittats, börja bryta ut ur loopar

			std::shuffle(neighbours.begin(), neighbours.end(), m_gen);
		}

		if(foundExit) break;	//Bryt ut ur sista loopen

		//Om vi gått in i en återvändsgränd
		do
		{
			history.pop();

			//Specialfall då stacken bara har ett element i sig
			if(history.empty()) return false;

			neighbours = getNeighbours(history.top());	//Hämta nya grannar

			for(size_t i = 0; i < neighbours.size(); i++)
			{
				//Radera oönskade grannar
				if((*this)(neighbours[i]) == char_wall || visited(neighbours[i]))
				{
					neighbours.erase(neighbours.begin() + i);
					--i;
				}
			}
		}
		while(neighbours.empty() && !history.empty());

		std::shuffle(neighbours.begin(), neighbours.end(), m_gen);
	}
	while(!foundExit && !history.empty() || !neighbours.empty());

	//Så länge stacken inte är tom, poppa den och markera vägen
	while(!history.empty())
	{
		(*this)(history.top()) = char_found;
		history.pop();
	}

	return foundExit;
}

std::ostream & operator<<(std::ostream & os, Maze & maze)
{
	//Lägg alla tecken i utströmmen
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
	
	//Returnera de grannar som finns inom labyrintens dimensioner
	if(index.x > 0)			list.push_back(Vec2i(index.x - 1, index.y));
	if(index.x < m_width - 1)	list.push_back(Vec2i(index.x + 1, index.y));
	if(index.y > 0)			list.push_back(Vec2i(index.x, index.y - 1));
	if(index.y < m_height - 1)	list.push_back(Vec2i(index.x, index.y + 1));

	return list;
}

bool Maze::isBorder(Vec2i index)
{
	//Returnerar true om indexet är en kant
	return index.x == 0 ||
		index.x == m_width  - 1 ||
		index.y == 0 ||
		index.y == m_height - 1;
}

Vec2i Maze::randomBorder()
{
	Vec2i pos;

	//Skapa en distribution för bredd höjd och true/false
	std::uniform_int_distribution<int32_t> wDist(1, m_width  - 2),
					hDist(1, m_height - 2),
					boolean(0,1);

	//Slumpa x och y enligt distributionen
	pos.x = wDist(m_gen);
	pos.y = hDist(m_gen);

	//Nollställ någon utav dem
	boolean(m_gen) == 1 ? pos.x = m_width - 1 : pos.y = m_height - 1;

	return pos;
}
