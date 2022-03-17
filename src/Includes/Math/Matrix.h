#include <vector>

#pragma once
template <class T>
class Matrix
{
public:
	int rows;
	int columns;

	Matrix(int rows, int columns) {
		matrix = std::vector<std::vector<T>>(rows);

		for (int i = 0; i < rows; i++) {
			matrix[i] = std::vector<T>(columns);
		}

		this->rows = rows;
		this->columns = columns;
	}

	Matrix(std::vector<std::vector<T>> matrix) {
		this->matrix = matrix;
		rows = matrix.size();
		columns = matrix[0].size();
	}

	~Matrix() {

	}


#pragma region Operator overloads
	Matrix operator*(Matrix by) {
		Matrix<T> out(columns, by.rows);

		if (rows == by.columns) {
			for (int thisRow = 0; thisRow < rows; thisRow++) {
				for (int byColumn = 0; byColumn < rows; byColumn++) {
					T temp = 0.0;

					for (int i = 0; i < rows; i++) {
						temp += matrix[thisRow][i] * by[i][byColumn];
					}

					out[thisRow, byColumn] = temp;
				}
			}
		}

		return out;
	}

	Matrix operator*(T by) {
		Matrix out(rows, columns);

		for (int row = 0; row < rows; row++) {
			for (int column = 0; column < columns; column++) {
				out[row, column] = matrix[row][column] * by;
			}
		}

		return out;
	}

	Matrix operator+(Matrix by) {
		Matrix out(rows, columns);

		for (int row = 0; row < rows; row++) {
			for (int column = 0; column < columns; column++) {
				out[row, column] = matrix[row][column] + by[row, column];
			}
		}

		return out;
	}

	Matrix operator-(Matrix by) {
		Matrix out(rows, by.columns);

		for (int row = 0; row < rows; row++) {
			for (int column = 0; column < columns; column++) {
				out[row, column] = matrix[row][column] - by[row, column];
			}
		}

		return out;
	}

	Matrix operator-(T by) {
		Matrix out(rows, columns);

		for (int row = 0; row < rows; row++) {
			for (int column = 0; column < columns; column++) {
				out[row, column] = matrix[row][column] - by[row, column];
			}
		}

		return out;
	}

	T operator[](int row, int column) const {
		return matrix[row][column];
	}

	T& operator[](int row, int column) {
		return matrix[row][column];
	}
#pragma endregion

	Matrix transpose() {
		Matrix out(columns, rows);

		for (int thisRow = 0; thisRow < rows; thisRow++) {
			for (int thisColumn = 0; thisColumn < columns; thisColumn++) {
				out[thisColumn, thisRow] = matrix[thisRow][thisColumn];
			}
		}

		return out;
	}

#pragma region Static utils
	static Matrix identity(int size) {
		Matrix identity = new Matrix(size, size);

		for (int i = 0; i < size; i++) {
			identity[i, i] = 1;
		}

		return identity;
	}

	static Matrix transpose(Matrix transposing) {
		return transposing.transpose();
	}
#pragma endregion

protected:
	std::vector<std::vector<T>> matrix;
};

