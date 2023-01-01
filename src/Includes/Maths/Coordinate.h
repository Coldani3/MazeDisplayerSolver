#ifndef CD3_MAZE_RENDERER_COORDINATE
#define CD3_MAZE_RENDERER_COORDINATE

#pragma once
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <string>


template <typename T>
class Coordinate {
private:
	std::vector<T> coords;

	void fillEmptyCoordsWithZeroes();

public:
	friend class Coordinate;
	Coordinate();
	Coordinate(const std::vector<T>& coords);
	Coordinate(std::vector<T>&& coords);
	Coordinate(Coordinate<T>&& moving);
	Coordinate(const Coordinate<T>& copying);
	~Coordinate();
	Coordinate<T> operator+(const Coordinate<T>& coord2) const;
	Coordinate<T> operator+=(const Coordinate<T>& coord2);
	Coordinate<T> operator-(const Coordinate<T>& coord2) const;
	Coordinate<T> operator-=(const Coordinate<T>& coord2);
	Coordinate<T> operator=(const Coordinate<T>& coord);
	Coordinate<T> operator=(Coordinate<T>&& coord);
	bool operator==(const Coordinate<T>& coord) const;
	bool operator!=(const Coordinate<T>& coord) const;

	//I'd rather have this in the .cpp but I guess I can't
	template <typename U>
	Coordinate<U> as() const {
		Coordinate<U> out;

		for (int i = 0; i < dimensions(); ++i) {
			out.coords[i] = static_cast<U>(coords[i]);
		}

		return out;
	}

	size_t dimensions() const;

	std::vector<T> toVector() const;

	//T& operator[](int index);
	//T operator[](int index) const;

	T x() const noexcept;
	T& x();
	T y() const noexcept;
	T& y();
	T z() const noexcept;
	T& z();
	T w() const noexcept;
	T& w();

};
#endif