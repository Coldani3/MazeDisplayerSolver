#include <vector>

#pragma once
template <class T>
class Matrix
{
public:
	int rows;
	int columns;

	Matrix(int rows, int columns) {

	}
	~Matrix() {

	}

#pragma region Operator overloads
	Matrix operator*(Matrix by) {

	}

	Matrix operator*(T by) {

	}

	Matrix operator+(Matrix by) {

	}

	Matrix operator+(T by) {

	}

	Matrix operator-(Matrix by) {

	}

	Matrix operator-(T by) {

	}

	T operator[](int row, int column) const {

	}

	T& operator[](int row, int column) {

	}
#pragma endregion

	static Matrix identity(int size) {

	}

protected:
	std::vector<std::vector<T>> matrix;
};

