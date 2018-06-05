#pragma once
#include <cstdint>
#include <algorithm>
#include "vec2.h"

using Vec2i = Vec2<int32_t>; //Specificering av template klass

template <class T>
class Table
{
public:

	//Tar in bredd höjd och defaultvärde
	Table(int32_t width, int32_t height, T value)
		: m_width(width), m_height(height)
	{

		//Allokera minne och tilldela
		m_tableData = new T[width*height];
		std::fill_n(m_tableData, m_width * m_height, value);
	}

	//Copy constructor
	Table(const Table & other)
	{
		//Hämta nya dimensioner
		m_width = other.m_width;
		m_height = other.m_height;

		//Allokera minne och tilldela
		m_tableData = new T[m_width * m_height];

		for(int32_t x = 0; x < m_width; x++)
			for(int32_t y = 0; y < m_height; y++)
			{
				(*this)(x,y) = other(x,y);
			}
	}

	//Virtuell destructor som frigör minne
	virtual ~Table()
	{
		delete [] m_tableData;
	}

	//Tilldelningsoperator
	Table & operator=(const Table & rhs)
	{
		//Självtilldelning
		if(this == &rhs) return *this;

		//Ta bort existerande data
		delete [] m_tableData;

		//Hämta nya dimensioner
		m_width = rhs.m_width;
		m_height = rhs.m_height;

		//Allokera minne och tilldela
		m_tableData = new T[m_width * m_height];

		for(int32_t x = 0; x < m_width; x++)
			for(int32_t y = 0; y < m_height; y++)
			{
				(*this)(x,y) = rhs(x,y);
			}

		return *this;
	}

	//Ändra storlek på en maze
	void resize(int32_t width, int32_t height, T value)
	{
		//Hämta nya dimensioner
		m_width = width;
		m_height = height;

		//Frigör och allokera minne
		delete [] m_tableData;
		m_tableData = new T[width * height];

		//Tilldela
		std::fill_n(m_tableData, m_width * m_height, value);
	}

	//Hämta element ur tabellen
	T & operator()(int32_t x, int32_t y)
	{
		return m_tableData[y * m_width + x];
	}


	T  operator()(int32_t x, int32_t y) const
	{
		return m_tableData[y * m_width + x];
	}

	T & operator()(Vec2i index)
	{
		return m_tableData[index.y * m_width + index.x];
	}

protected:
	T * m_tableData = nullptr;

	int32_t m_height,
		 m_width;
};
