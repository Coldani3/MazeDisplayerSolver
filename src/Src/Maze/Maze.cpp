#include <Maze/Maze.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <exception>

bool isNumberChar(char character) {
	return (character >= 48 && character <= 57);
}

Maze::Maze(std::vector<int> dimensionsVec) {
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

	mazeData = std::vector<char>(length);
	sizes = dimensionsVec;
}

Maze::~Maze() {
	
}

void Maze::loadFromFile(std::string fileName) {
	std::ifstream inFile;

	inFile.open(fileName);

	//get cell size
	char currentChar;
	int cellSize = 0;
	inFile.get(currentChar);

	while (currentChar != '{') {
		if (isNumberChar(currentChar)) {
			cellSize *= 10;
			cellSize += (int)currentChar;
			inFile.get(currentChar);
		} else {
			throw "Malformed maze file cell size in " + fileName;
		}
	}

	//get dimensions
	int dimSizeCounter = 0;
	std::vector<int> dimSizes = std::vector<int>();
	inFile.get(currentChar);
	dimSizes.push_back(0);

	while (currentChar != '}') {
		while (currentChar != ',' && currentChar != '}') {
			if (!(dimSizes.size() >= dimSizeCounter + 1)) {
				dimSizes.push_back(0);
			}

			if (isNumberChar(currentChar)) {
				dimSizes[dimSizeCounter] *= 10;
				dimSizes[dimSizeCounter] += (int)currentChar;
				inFile.get(currentChar);
			} else {
				throw "Malformed maze file dimension specification in " + fileName;
			}
		}

		dimSizeCounter++;
	}

	while (!inFile.eof()) {
		inFile.get(currentChar);


	}

	inFile.close();
}

char Maze::operator[](std::vector<int> coordinates) const {
	std::array<int, 4> coordsToFour = { 0, 0, 0, 0 };

	for (int i = 0; i < coordinates.size(); i++) {
		coordsToFour[i] = coordinates[i];
	}

	return mazeData[(height * width * depth * coordsToFour[3]) + (height * width * coordsToFour[2]) + (height * coordsToFour[1]) + coordsToFour[0]];
}

char& Maze::operator[](std::vector<int> coordinates) {
	std::array<int, 4> coordsToFour = { 0, 0, 0, 0 };

	for (int i = 0; i < coordinates.size(); i++) {
		coordsToFour[i] = coordinates[i];
	}

	return mazeData[(height * width * depth * coordsToFour[3]) + (height * width * coordsToFour[2]) + (height * coordsToFour[1]) + coordsToFour[0]];
}