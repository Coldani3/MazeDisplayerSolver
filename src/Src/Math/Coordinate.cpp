#include <Maths/Coordinate.h>
#pragma once


template<class T>
void Coordinate<T>::fillEmptyCoordsWithZeroes() {
	while (this->coords.size() < 4) {
		this->coords.push_back(0);
	}
}

template<class T>
Coordinate<T>::Coordinate() {
	coords = { 0, 0, 0, 0 };
}

template<class T>
Coordinate<T>::Coordinate(const std::vector<T>& coords) {
	if (coords.size() <= 4) {
		std::copy(coords.begin(), coords.end(), std::back_inserter(this->coords));
		fillEmptyCoordsWithZeroes();
	} else {
		throw std::out_of_range("Coordinates are greater than 4D!");
	}
}

template<class T>
Coordinate<T>::Coordinate(std::vector<T>&& coords) {
	if (coords.size() <= 4) {
		this->coords = std::move(coords);
		fillEmptyCoordsWithZeroes();
	} else {
		throw std::out_of_range("Coordinates are greater than 4D!");
	}
}

template<class T>
Coordinate<T>::Coordinate(Coordinate<T>&& moving) : coords(moving.coords) {
	moving.coords = nullptr;
}

template<class T>
Coordinate<T>::Coordinate(const Coordinate<T>& copying) {
	coords = copying.coords;
}

template<class T>
Coordinate<T> Coordinate<T>::operator+(const Coordinate<T>& coord2) const {
	Coordinate<T> out(this->coords);

	for (int i = 0; i < coords.size(); ++i) {
		out[i] += coord2[i];
	}

	return out;
}

template<class T>
Coordinate<T> Coordinate<T>::operator+=(const Coordinate<T>& coord2) {
	//shouldn't really *need* to do this but just in case I do drugs
	//and allocate a Coordinate on the heap we should do this anyways
	if (this != &coord2) {
		coords = (*this + coord2).coords;
	}
	return *this;
}

template<class T>
Coordinate<T> Coordinate<T>::operator-(const Coordinate<T>& coord2) const {
	Coordinate<T> out(this->coords);

	for (int i = 0; i < coords.size(); ++i) {
		out[i] -= coord2[i];
	}

	return out;
}

template<class T>
Coordinate<T> Coordinate<T>::operator-=(const Coordinate<T>& coord2) {
	//shouldn't really *need* to do this but just in case I do drugs
	//and allocate a Coordinate on the heap we should do this anyways
	if (this != &coord2) {
		coords = (*this - coord2).coords;
	}
	return *this;
}

template<class T>
Coordinate<T> Coordinate<T>::operator=(const Coordinate<T>& coord) {
	coords = coord.coords;
}

template<class T>
Coordinate<T> Coordinate<T>::operator=(Coordinate<T>&& coord) {
	coords = std::move(coord.coords);
}

template<class T>
bool Coordinate<T>::operator==(const Coordinate<T>& coord) {
	return this == &coord || (coord.size() == size() && std::equal(coords.begin(), coords.end(), coord.coords.begin()));
}

template<class T>
size_t Coordinate<T>::size() const {
	return coords.size();
}

template<class T>
T& Coordinate<T>::operator[](int index) {
	if (index < coords.size() && index > -1) {
		return coords[index];
	} else {
		throw std::out_of_range("Index out of bounds for coordinate (received index of " + std::to_string(index) + ").");
	}
}

template<class T>
T Coordinate<T>::operator[](int index) const {
	if (index < coords.size() && index > -1) {
		return coords[index];
	} else {
		throw std::out_of_range("Index out of bounds for coordinate (received index of " + std::to_string(index) + ").");
	}
}

template<class T>
T Coordinate<T>::x() const noexcept {
	return coords[0];
}

template<class T>
T Coordinate<T>::y() const noexcept {
	return coords[1];
}

template<class T>
T Coordinate<T>::z() const noexcept {
	return coords[2];
}

template<class T>
T Coordinate<T>::w() const noexcept {
	return coords[3];
}

template<class T>
std::vector<T> Coordinate<T>::toVector() const {
	//copy to prevent weirdness
	std::vector<T> out = coords;
	return out;
}