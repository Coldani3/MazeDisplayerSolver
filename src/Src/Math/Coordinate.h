#ifndef CD3_MAZE_RENDERER_COORDINATE
#define CD3_MAZE_RENDERER_COORDINATE

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <string>

#pragma once
template <typename T>
class Coordinate {
private:
	std::vector<T> coords;

	void fillEmptyCoordsWithZeroes() {
		while (this->coords.size() < 4) {
			this->coords.push_back(0);
		}
	}

public:
	friend class Coordinate;

	Coordinate() {
		coords = { 0, 0, 0, 0 };
	}

	Coordinate(const std::vector<T>& coords) {
		if (coords.size() <= 4) {
			std::copy(coords.begin(), coords.end(), std::back_inserter(this->coords));
			fillEmptyCoordsWithZeroes();
		}
		else {
			throw std::out_of_range("Coordinates are greater than 4D!");
		}
	}

	Coordinate(std::vector<T>&& coords) {
		if (coords.size() <= 4) {
			this->coords = std::move(coords);
			fillEmptyCoordsWithZeroes();
		}
		else {
			throw std::out_of_range("Coordinates are greater than 4D!");
		}
	}

	Coordinate(Coordinate<T>&& moving) noexcept : coords(std::move(moving.coords)) {
	}

	Coordinate(const Coordinate<T>& copying) {
		coords = copying.coords;
	}

	Coordinate<T> operator+(const Coordinate<T>& coord2) const {
		Coordinate<T> out(this->coords);

		for (int i = 0; i < coords.size(); ++i) {
			out.coords[i] += coord2.coords[i];
		}

		return out;
	}

	Coordinate<T> operator+=(const Coordinate<T>& coord2) {
		//shouldn't really *need* to do this but just in case I do drugs
		//and allocate a Coordinate on the heap we should do this anyways
		if (this != &coord2) {
			coords = (*this + coord2).coords;
		}
		return *this;
	}

	Coordinate<T> operator-(const Coordinate<T>& coord2) const {
		Coordinate<T> out(this->coords);

		for (int i = 0; i < coords.size(); ++i) {
			out.coords[i] -= coord2.coords[i];
		}

		return out;
	}

	Coordinate<T> operator-=(const Coordinate<T>& coord2) {
		//shouldn't really *need* to do this but just in case I do drugs
		//and allocate a Coordinate on the heap we should do this anyways
		if (this != &coord2) {
			coords = (*this - coord2).coords;
		}
		return *this;
	}

	Coordinate<T> operator=(const Coordinate<T>& coord) {
		coords = coord.coords;
	}

	Coordinate<T> operator=(Coordinate<T>&& coord) {
		coords = std::move(coord.coords);
		return *this;
	}

	bool operator==(const Coordinate<T>& coord) const {
		return this == &coord || (coord.dimensions() == dimensions() && std::equal(coords.begin(), coords.end(), coord.coords.begin()));
	}

	bool operator!=(const Coordinate<T>& coord) const {
		return !(*this == coord);
	}

	//I'd rather have this in the .cpp but I guess I can't
	template <typename U>
	Coordinate<U> as() const {
		Coordinate<U> out;

		for (int i = 0; i < dimensions(); ++i) {
			out.coords[i] = static_cast<U>(coords[i]);
		}

		return out;
	}

	size_t dimensions() const {
		return coords.size();
	}

	std::vector<T> toVector() const {
		//copy to prevent weirdness
		std::vector<T> out = coords;
		return out;
	}

	T x() const noexcept {
		return coords[0];
	}

	T& x() {
		return coords[0];
	}

	T y() const noexcept {
		return coords[1];
	}

	T& y() {
		return coords[1];
	}

	T z() const noexcept {
		return coords[2];
	}

	T& z() {
		return coords[2];
	}

	T w() const noexcept {
		return coords[3];
	}

	T& w() {
		return coords[3];
	}

};
#endif