#include <Maze/Maze.h>
#include <string>
#include <iostream>
#include <fstream>

Maze::Maze() {

}

Maze::Maze(int dimensions[]) {

}

Maze::~Maze() {
	delete mazeData;
}

void Maze::loadFromFile(std::string fileName) {
	std::ifstream inFile;

	inFile.open(fileName);
}
