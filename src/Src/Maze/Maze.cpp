#include <Maze/Maze.h>
#include <string>
#include <iostream>
#include <fstream>

Maze::Maze() {

}

Maze::~Maze() {

}

void Maze::loadFromFile(std::string fileName) {
	std::ifstream inFile;

	inFile.open(fileName);
}
