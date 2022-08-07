#include <Maths/Coordinate.h>
#include <stdexcept>
#include <string>
#pragma once


template<class T>
Coordinate<T>::Coordinate(std::vector<T> coords) {
	for (int i = 0; i < coords.size() < 4; ++i) {
		this->coords[i] = coords[i];
	}
}

template<class T>
Coordinate<T> Coordinate<T>::operator+(Coordinate coord2) {
	Coordinate<T> out(this->coords);

	for (int i = 0; i < coords.size(); ++i) {
		out[i] += coord2[i];
	}

	return out;
}

template<class T>
Coordinate<T> Coordinate<T>::operator-(Coordinate coord2) {
	Coordinate<T> out(this->coords);

	for (int i = 0; i < coords.size(); ++i) {
		out[i] -= coord2[i];
	}

	return out;
}

template<class T>
int Coordinate<T>::size() {
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
std::vector<T> Coordinate<T>::toVector() {
	//copy to prevent weirdness
	std::vector<T> out = coords;
	return out;
}