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

int charToNum(char character) {
	return character - 48;
}

char Maze::getCell(std::vector<int> coordinates) const {
	std::array<int, 4> coordsToFour = { 0, 0, 0, 0 };

	for (int i = 0; i < coordinates.size(); i++) {
		coordsToFour[i] = coordinates[i];
	}

	return mazeData[(height * width * depth * coordsToFour[3]) + (height * width * coordsToFour[2]) + (height * coordsToFour[1]) + coordsToFour[0]];
}

void Maze::setCell(std::vector<int> coordinates, char to) {
	std::array<int, 4> coordsToFour = { 0, 0, 0, 0 };

	for (int i = 0; i < coordinates.size(); i++) {
		coordsToFour[i] = coordinates[i];
	}

	mazeData[(height * width * depth * coordsToFour[3]) + (height * width * coordsToFour[2]) + (height * coordsToFour[1]) + coordsToFour[0]] = to;
}

char Maze::operator[](std::vector<int> coordinates) const {
	return getCell(coordinates);
}

char& Maze::operator[](std::vector<int> coordinates) {
	std::array<int, 4> coordsToFour = { 0, 0, 0, 0 };

	for (int i = 0; i < coordinates.size(); i++) {
		coordsToFour[i] = coordinates[i];
	}

	return mazeData[(height * width * depth * coordsToFour[3]) + (height * width * coordsToFour[2]) + (height * coordsToFour[1]) + coordsToFour[0]];
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
			cellSize = (cellSize * 10) + charToNum(currentChar);
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
				dimSizes[dimSizeCounter] = (dimSizes[dimSizeCounter] * 10) + charToNum(currentChar);
				inFile.get(currentChar);
			} else {
				throw "Malformed maze file dimension specification in " + fileName;
			}
		}

		inFile.get(currentChar);

		dimSizeCounter++;
	}

	dimensions = dimSizes.size();

	std::vector<int> coords = { 0, 0 };
	//TODO: actually read the slice size coordinates and not just assume the maze is one massive cube.
	std::vector<char> sliceData;
	std::array<int, 2> sliceSize = { 0, 0 };
	std::array<int, 2> higherDimCoords = { 0, 0 };
	int sliceSizeIndex = 0;
	int higherDimCoordsIndex = 0;

	//skip past } to [
	inFile.get(currentChar);

	while (currentChar != '+') {
		//get higher dim coords

		while (currentChar != ']') {
			inFile.get(currentChar);

			if (currentChar == '|') {
				sliceSizeIndex++;
			} else {
				sliceSize[sliceSizeIndex] = sliceSize[sliceSizeIndex] * 10 + charToNum(currentChar);
			}
		}

		sliceData = std::vector<char>(sliceSize[0] * sliceSize[1]);

		inFile.get(currentChar);

		if (currentChar == '(') {
			while (currentChar != ')') {
				inFile.get(currentChar);
			}
		}

		inFile.get(currentChar);

		if (currentChar == '\\') {
			while (currentChar != '/') {
				inFile.get(currentChar);

				if (currentChar == '|') {
					higherDimCoordsIndex++;
				} else {
					higherDimCoords[higherDimCoordsIndex] = higherDimCoords[higherDimCoordsIndex] * 10 + charToNum(currentChar);
				}
			}

			inFile.get(currentChar);
		} else if (dimensions >= 3) {
			throw "Malformed maze file, a slice is missing its specification of its 3D/4D coordinates!";
		}

		coords.insert(coords.end(), higherDimCoords.begin(), higherDimCoords.end());

		while (currentChar != '[') {
			sliceData.push_back(currentChar);
			inFile.get(currentChar);
		}

		int dataIndex = 0;

		//input data into maze object
		for (int y = 0; y < sliceSize[1]; y++) {
			for (int x = 0; x < sliceSize[0]; x++) {
				coords[0] = x;
				coords[1] = y;
				setCell(coords, sliceData[dataIndex]);
				dataIndex++;
			}
		}

		//clean up for next slice
		sliceData.clear();
		sliceSize = { 0, 0 };
		sliceSizeIndex = 0;
		higherDimCoordsIndex = 0;
		coords = { 0, 0 };
	}

	//skip open bracket
	inFile.get(currentChar);

	//access and set the entrance and exit like this so I don't write the same code twice and don't have to manually set it after
	std::array<std::unique_ptr<std::vector<int>>, 2> mazeEntranceExit = { std::unique_ptr<std::vector<int>>(&mazeEntrance), std::unique_ptr<std::vector<int>>(&mazeExit) };

	int mazeEntranceExitIndex = 0;
	int coordIndex = 0;

	while (currentChar != '+') {
		if (currentChar == ',') {
			coordIndex++;
		} else if (isNumberChar(currentChar)) {
			//this looks kinda icky. oh well.
			(*mazeEntranceExit[mazeEntranceExitIndex])[coordIndex] = (*mazeEntranceExit[mazeEntranceExitIndex])[coordIndex] * 10 + charToNum(currentChar);
		} else if (currentChar == ')') {
			mazeEntranceExitIndex++;
			coordIndex = 0;
		}

		inFile.get(currentChar);
	}

	inFile.close();
}