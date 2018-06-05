#pragma once
#include <cstdint>
#include <algorithm>
#include "vec2.h"

using Vec2i = Vec2<int32_t>;

template <class T>
class Table
{
public:
	Table(int32_t width, int32_t height, T value)
		: m_width(width), m_height(height)
	{
		m_tableData = new T[width*height];
		std::fill_n(m_tableData, m_width * m_height, value);
	}

	virtual ~Table()
	{
		delete [] m_tableData;
	}

	void resize(int32_t width, int32_t height, T value)
	{
		m_width = width;
		m_height = height;

		delete [] m_tableData;
		m_tableData = new T[width * height];

		std::fill_n(m_tableData, m_width * m_height, value);
	}

	T & operator()(int32_t x, int32_t y)
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
