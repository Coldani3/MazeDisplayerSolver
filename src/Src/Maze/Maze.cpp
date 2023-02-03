#include "Maze.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <exception>
#include <filesystem>

bool isNumberChar(char character) {
	return (character >= 48 && character <= 57);
}

int charToNum(char character) {
	return character - 48;
}

unsigned char Maze::getCell(const Coordinate<int>& coordinates) const {
	return mazeData[(height * width * depth * coordinates.w()) + (height * width * coordinates.z()) + (height * coordinates.y()) + coordinates.x()];
}

void Maze::setCell(const Coordinate<int>& coordinates, char to) {
	mazeData[(height * width * depth * coordinates.w()) + (height * width * coordinates.z()) + (height * coordinates.y()) + coordinates.x()] = to;
}

unsigned char Maze::operator[](const Coordinate<int>& coordinates) const {
	return getCell(coordinates);
}

unsigned char& Maze::operator[](const Coordinate<int>& coordinates) {
	return mazeData[(height * width * depth * coordinates.w()) + (height * width * coordinates.z()) + (height * coordinates.y()) + coordinates.x()];
}

bool Maze::inBounds(const Coordinate<int>& coordinates) {
	if (coordinates.dimensions() != 4) {
		return false;
	}

	return (coordinates.x() >= 0 && coordinates.x() < sizes[0]) &&
		(coordinates.y() >= 0 && coordinates.y() < sizes[1]) &&
		(coordinates.z() >= 0 && coordinates.z() < sizes[2]) && 
		(coordinates.w() >= 0 && coordinates.w() < sizes[3]);
}

bool Maze::canAccessFrom(const Coordinate<int>& fromCoords, const Coordinate<int>& toCoords) {
	return false;
}

bool Maze::wInBounds(int w) {
	return w >= 0 && w < hyperDepth;
}

Maze::Maze() {}

Maze::Maze(const std::vector<int>& dimensionsVec) {
	int length = 1;
	size_t dimensionsSize = dimensionsVec.size();

	if (dimensionsSize < 2) {
		try {
			throw dimensionsSize;
		} catch (int e) {
			std::cerr << "A maze cannot have less than 2 dimensions (was " << e << ")" << std::endl;
			throw;
		}
	}

	for (int i = 0; i < dimensionsSize; i++) {
		length *= dimensionsVec[i];
	}

	width = dimensionsVec[0];
	height = dimensionsVec[1];

	dimensions = 2;

	if (dimensionsSize > 2) {
		depth = dimensionsVec[2];
		dimensions = 3;
	}

	if (dimensionsSize > 3) {
		hyperDepth = dimensionsVec[3];
		dimensions = 4;
	}

	mazeData = std::vector<unsigned char>(length);
	sizes = dimensionsVec;
}

Maze::~Maze() {
	
}

//man I really want to go back and change the file format but eh
void Maze::loadFromFile(std::string fileName) {
	std::ifstream inFile;

	inFile.open(fileName, std::ios::binary);

	if (!inFile.good()) {
		std::cerr << "Could not find file " << fileName << "!" << std::endl;
		throw std::runtime_error("Could not find file " + fileName);
	}

	//get cell size
	char currentChar;
	int cellSize = 0;
	inFile.get(currentChar);

	//<dimensions> <maze x size> <maze y size> <maze z size> <maze w size> <maze entrance x, y, z, w> <maze exit x, y, z, w> <raw data>

	int dimensions = currentChar;//charToNum(currentChar);

	if (dimensions != 4) {
		std::stringstream message;
		message << "Maze file of dimensions " << dimensions << " detected! We currently only support 4 dimensions!";
		throw std::runtime_error(message.str());
	}

	sizes = std::vector<int>(4);

	for (int i = 0; i < dimensions; i++) {
		inFile.get(currentChar);
		sizes[i] = currentChar;
	}

	std::vector<int> entrance = std::vector<int>(4);

	for (int i = 0; i < dimensions; i++) {
		inFile.get(currentChar);
		entrance[i] = currentChar;
	}

	std::vector<int> exit = std::vector<int>(4);

	for (int i = 0; i < dimensions; i++) {
		inFile.get(currentChar);
		exit[i] = currentChar;
	}

	mazeExit = Coordinate<int>(exit);
	mazeEntrance = Coordinate<int>(entrance);
	this->dimensions = dimensions;
	width = sizes[0];
	height = sizes[1];
	depth = sizes[2];
	hyperDepth = sizes[3];

	int size = 1;

	for (int i = 0; i < sizes.size(); i++) {
		size *= sizes[i];
	}

	std::vector<unsigned char> mazeCellData = std::vector<unsigned char>(size);

	//read the data
	for (int i = 0; i < size; i++) {
		inFile.get(currentChar);
		mazeCellData[i] = currentChar;
	}

	mazeData = mazeCellData;

	inFile.close();
}

unsigned int Maze::getSize() {
	unsigned int total = 1;

	for (int i = 0; i < sizes.size(); i++) {
		total *= sizes[i];
	}

	return total;
}