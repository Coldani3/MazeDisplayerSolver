#ifndef CD3_MAZE_RENDERER_COORDINATE
#define CD3_MAZE_RENDERER_COORDINATE

#pragma once
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <string>


template <class T>
class Coordinate {
private:
	std::vector<T> coords;

	void fillEmptyCoordsWithZeroes();

public:
	Coordinate();
	Coordinate(const std::vector<T>& coords);
	Coordinate(std::vector<T>&& coords);
	Coordinate(Coordinate<T>&& moving);
	Coordinate(const Coordinate<T>& copying);
	Coordinate<T> operator+(const Coordinate<T>& coord2) const;
	Coordinate<T> operator+=(const Coordinate<T>& coord2);
	Coordinate<T> operator-(const Coordinate<T>& coord2) const;
	Coordinate<T> operator-=(const Coordinate<T>& coord2);
	Coordinate<T> operator=(const Coordinate<T>& coord);
	Coordinate<T> operator=(Coordinate<T>&& coord);
	bool operator==(const Coordinate<T>& coord);

	template <class U>
	Coordinate<U> as<U>() const {
		Coordinate<U> out;

		for (int i = 0; i < size(); ++i) {
			out.coords[i] = static_cast<U>(coords[i]);
		}

		return out;
	}

	size_t size() const;

	std::vector<T> toVector() const;

	T& operator[](int index);
	T operator[](int index) const;

	T x() const noexcept;
	T y() const noexcept;
	T z() const noexcept;
	T w() const noexcept;

};

#endif

