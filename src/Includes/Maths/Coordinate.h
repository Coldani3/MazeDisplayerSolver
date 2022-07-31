#pragma once
#include <vector>

template <class T>
class Coordinate {
private:
	std::vector<T> coords;

public:
	Coordinate(std::vector<T> coords);
	Coordinate<T> operator+(Coordinate<T> coord2);
	Coordinate<T> operator-(Coordinate<T> coord2);

	int size();

	std::vector<T> toVector();

	T& operator[](int index);
	T operator[](int index) const;

};

