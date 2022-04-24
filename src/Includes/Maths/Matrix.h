#include <vector>
#include <Maths/Consts.h>
#include <cmath>

#pragma once
template <class T>
class Matrix
{
public:
	int rows;
	int columns;

	//please don't actually use this constructor, it's just here so I can initialise a vector of these and it won't let me make it private
	Matrix() {};

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
	Matrix<T> operator*(Matrix<T> by) {
		Matrix<T> out(columns, by.rows);

		if (rows == by.columns) {
			for (int thisRow = 0; thisRow < rows; thisRow++) {
				for (int byColumn = 0; byColumn < rows; byColumn++) {
					T temp = 0.0;

					for (int i = 0; i < rows; i++) {
						temp += matrix[thisRow][i] * by[i][byColumn];
					}

					out[thisRow][byColumn] = temp;
				}
			}
		}

		return out;
	}

	Matrix<T> operator*(T by) {
		Matrix<T> out(rows, columns);

		for (int row = 0; row < rows; row++) {
			for (int column = 0; column < columns; column++) {
				out[row][column] = matrix[row][column] * by;
			}
		}

		return out;
	}

	friend Matrix<T> operator*(T value, Matrix<T> by) {
		Matrix<T> out(by.rows, by.columns);

		for (int row = 0; row < by.rows; row++) {
			for (int col = 0; col < by.columns; col++) {
				out[row][col] = by[row][col] * value;
			}
		}

		return out;
	}

	Matrix<T> operator+(Matrix<T> by) {
		Matrix<T> out(rows, columns);

		for (int row = 0; row < rows; row++) {
			for (int column = 0; column < columns; column++) {
				out[row][column] = matrix[row][column] + by[row][column];
			}
		}

		return out;
	}

	Matrix<T> operator-(Matrix<T> by) {
		Matrix<T> out(rows, by.columns);

		for (int row = 0; row < rows; row++) {
			for (int column = 0; column < columns; column++) {
				out[row][column] = matrix[row][column] - by[row][column];
			}
		}

		return out;
	}

	Matrix<T> operator-(T by) {
		Matrix<T> out(rows, columns);

		for (int row = 0; row < rows; row++) {
			for (int column = 0; column < columns; column++) {
				out[row][column] = matrix[row][column] - by;
			}
		}

		return out;
	}

	friend Matrix operator-(T value, Matrix<T> by) {
		Matrix<T> out(by.rows, by.columns);

		for (int row = 0; row < by.rows; row++) {
			for (int col = 0; col < by.columns; col++) {
				out[row][col] = value - by[row][col];
			}
		}

		return out;
	}

	std::vector<T> operator[](int row) const {
		return matrix[row];
	}

	std::vector<T> &operator[](int row) {
		return matrix[row];
	}
#pragma endregion

	Matrix<T> transpose() {
		Matrix<T> out(columns, rows);

		for (int thisRow = 0; thisRow < rows; thisRow++) {
			for (int thisColumn = 0; thisColumn < columns; thisColumn++) {
				out[thisColumn][thisRow] = matrix[thisRow][thisColumn];
			}
		}

		return out;
	}

#pragma region Static utils
	static Matrix<T> identity(int size) {
		Matrix<T> identity(size, size);

		for (int i = 0; i < size; i++) {
			identity[i][i] = 1;
		}

		return identity;
	}

	static Matrix<T> transpose(Matrix<T> transposing) {
		return transposing.transpose();
	}

	static std::vector<std::vector<T>> inputArrayToRow(std::vector<T> inputVec) {
		std::vector<std::vector<T>> out(inputVec.size());

		for (int i = 0; i < inputVec.size(); i++) {
			out[i][0] = inputVec[i];
		}

		return out;
	}

	static Matrix<T> sigmoid(Matrix<T> input) {
		Matrix out(input.rows, input.columns);

		for (int row = 0; row < input.rows; row++) {
			for (int col = 0; col < input.columns; col++) {
				out[row][col] = 1 / (1 + std::pow(M_E, -input[row][col]));
			}
		}

		return out;
	}

	static Matrix<T> tanH(Matrix<T> input) {
		Matrix out(input.rows, input.columns);

		for (int row = 0; row < input.rows; row++) {
			for (int col = 0; col < input.columns; col++) {
				out[row][col] = std::tanh(input[row][col]);
			}
		}

		return out;
	}

	static std::vector<T> flatten(Matrix<T> input) {
		std::vector<T> out(input.rows * input.columns);
		int counter = 0;

		for (int row = 0; row < input.rows; row++) {
			for (int col = 0; col < input.columns; col++) {
				out[counter] = input[row][col];
				counter++;
			}
		}

		return out;
	}
#pragma endregion

protected:
	std::vector<std::vector<T>> matrix;

};

