#include <Maze/Maze.h>
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

unsigned char Maze::getCell(const std::vector<int>& coordinates) const {
	std::array<int, 4> coordsToFour = { 0, 0, 0, 0 };

	for (int i = 0; i < coordinates.size(); i++) {
		coordsToFour[i] = coordinates[i];
	}

	return mazeData[(height * width * depth * coordsToFour[3]) + (height * width * coordsToFour[2]) + (height * coordsToFour[1]) + coordsToFour[0]];
}

void Maze::setCell(const std::vector<int>& coordinates, char to) {
	std::array<int, 4> coordsToFour = { 0, 0, 0, 0 };

	for (int i = 0; i < coordinates.size(); i++) {
		coordsToFour[i] = coordinates[i];
	}

	mazeData[(height * width * depth * coordsToFour[3]) + (height * width * coordsToFour[2]) + (height * coordsToFour[1]) + coordsToFour[0]] = to;
}

unsigned char Maze::operator[](const std::vector<int>& coordinates) const {
	return getCell(coordinates);
}

unsigned char& Maze::operator[](const std::vector<int>& coordinates) {
	std::array<int, 4> coordsToFour = { 0, 0, 0, 0 };

	for (int i = 0; i < coordinates.size(); i++) {
		coordsToFour[i] = coordinates[i];
	}

	return mazeData[(height * width * depth * coordsToFour[3]) + (height * width * coordsToFour[2]) + (height * coordsToFour[1]) + coordsToFour[0]];
}

bool Maze::inBounds(const std::vector<int>& coordinates) {
	if (coordinates.size() != 4) {
		return false;
	}

	return (coordinates[0] >= 0 && coordinates[0] < sizes[0]) && 
		(coordinates[1] >= 0 && coordinates[1] < sizes[1]) && 
		(coordinates[2] >= 0 && coordinates[2] < sizes[2]) && 
		(coordinates[3] >= 0 && coordinates[3] < sizes[3]);
}

bool Maze::canAccessFrom(const std::vector<int>& fromCoords, const std::vector<int>& toCoords) {
	return false;
}

bool Maze::wInBounds(int w) {
	return w >= 0 && w < hyperDepth;
}

Maze::Maze() {}

Maze::Maze(const std::vector<int>& dimensionsVec) {
	int length = 1;
	int dimensionsSize = dimensionsVec.size();

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

	mazeExit = exit;
	mazeEntrance = entrance;
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