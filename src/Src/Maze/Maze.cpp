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

unsigned char Maze::getCell(std::vector<int> coordinates) const {
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

unsigned char Maze::operator[](std::vector<int> coordinates) const {
	return getCell(coordinates);
}

unsigned char& Maze::operator[](std::vector<int> coordinates) {
	std::array<int, 4> coordsToFour = { 0, 0, 0, 0 };

	for (int i = 0; i < coordinates.size(); i++) {
		coordsToFour[i] = coordinates[i];
	}

	return mazeData[(height * width * depth * coordsToFour[3]) + (height * width * coordsToFour[2]) + (height * coordsToFour[1]) + coordsToFour[0]];
}

Maze::Maze() {}

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

	mazeData = std::vector<unsigned char>(length);
	sizes = dimensionsVec;
}

Maze::~Maze() {
	
}

//man I really want to go back and change the file format but eh
void Maze::loadFromFile(std::string fileName) {
	std::ifstream inFile;

	inFile.open(fileName, std::ios::binary);

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
	//index of the size we are adding.
	int dimSizeCounter = 0;
	std::vector<int> dimSizes = std::vector<int>();
	//skip past {
	inFile.get(currentChar);
	//put a value in the dimSizes array so the data has somewhere to go
	dimSizes.push_back(0);

	while (currentChar != '}') {
		while (currentChar != ',' && currentChar != '}') {
			//std::cout << "i" << currentChar;
			if (!(dimSizes.size() >= dimSizeCounter + 1)) {
				//adding another value to the dimSizes array, put something empty in there
				dimSizes.push_back(0);
			}

			if (isNumberChar(currentChar)) {
				dimSizes[dimSizeCounter] = (dimSizes[dimSizeCounter] * 10) + charToNum(currentChar);
			} else {
				//std::cerr << currentChar;
				throw "Malformed maze file dimension specification in " + fileName;
			}

			inFile.get(currentChar);
		}

		//std::cout << "o" << currentChar;

		if (currentChar == ',') {
			inFile.get(currentChar);
		}

		dimSizeCounter++;

		//std::cout << currentChar;
	}

	dimensions = dimSizes.size();

	int dataSize = 1;

	for (int i = 0; i < dimensions; i++) {
		dataSize *= dimSizes[i];
	}

	if (mazeData.size() <= 0) {
		mazeData = std::vector<unsigned char>(dataSize);
	}

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

		if (currentChar == ')') {
			inFile.get(currentChar);
		}

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

		std::cout << "insert";

		while (currentChar != '[') {
			//std::cout << currentChar << (unsigned int) currentChar << std::endl;
			sliceData.push_back(currentChar);
			currentChar = inFile.get();

			//if (currentChar == '+') {
			//	//icky but eh
			//	goto toPlus;
			//}
		}

		int dataIndex = 0;

		std::cout << "begin maze input";

		//input data into maze object
		for (int y = 0; y < sliceSize[1]; y++) {
			for (int x = 0; x < sliceSize[0]; x++) {
				coords[0] = x;
				coords[1] = y;
				//std::cout << dataIndex << ", " << sliceData.size() << ", " << x << ", " << y << ";" << std::endl;
				setCell(coords, sliceData[dataIndex]);
				dataIndex++;
			}
		}

		std::cout << "maze input done";

		//clean up for next slice
		sliceData.clear();
		sliceSize = { 0, 0 };
		sliceSizeIndex = 0;
		higherDimCoordsIndex = 0;
		coords = { 0, 0 };
	}

	//skip open bracket
	inFile.get(currentChar);

	toPlus:

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

	mazeEntrance = *mazeEntranceExit[0];
	mazeExit = *mazeEntranceExit[1];

	inFile.close();
}

unsigned int Maze::getSize() {
	unsigned int total = 1;

	for (int i = 0; i < sizes.size(); i++) {
		total *= sizes[i];
	}

	return total;
}