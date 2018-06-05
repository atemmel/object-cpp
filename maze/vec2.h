#pragma once

//Template till en 2dimensionell matematisk vektor
//Används för att indexera tabeller, navigera i 
//labyrinten med mera
template <class T>
struct Vec2
{
	Vec2(T x = 0, T y = 0) :
		x(x), y(y) {};

	bool operator==(const Vec2 & other)
	{
		return x == other.x && 
			y == other.y;
	}

	T x, y;
};
