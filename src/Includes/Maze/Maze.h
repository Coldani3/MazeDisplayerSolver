#include <string>
#include <iostream>
#include <vector>

#pragma once

//The flags each bit of each maze cell contain.
enum MazeDirections {
	FORWARD = (unsigned int) 1,
	BACKWARD = (unsigned int) 2,
	RIGHT = (unsigned int) 4,
	LEFT = (unsigned int) 8,
	UP = (unsigned int) 16,
	DOWN = (unsigned int) 32,
	ANA = (unsigned int) 64,
	KATA = (unsigned int) 128
};

//A maze that stores the data and allows access of mazes up to and including 4 dimensions.
class Maze {
public:
	int width = 0;
	int height = 0;
	int depth = 0;
	int hyperDepth = 0;
	int dimensions;
	std::vector<int> mazeEntrance;
	std::vector<int> mazeExit;
	std::vector<int> sizes;

	Maze();
	Maze(std::vector<int> dimensions);
	~Maze();
	void loadFromFile(std::string fileName);
	//Read the byte representing the cell at coordinates
	unsigned char operator[] (std::vector<int> coordinates) const;
	//Provide the byte representing the cell at coordinates as a reference to modify
	unsigned char& operator[](std::vector<int> coordinates);

private:
	//The data that makes up the maze.
	std::vector<unsigned char> mazeData;

	//C++ doesn't like this[] with header separated classes :/

	unsigned char getCell(std::vector<int> coordinates) const;
	void setCell(std::vector<int> coordinates, char to);
	unsigned int getSize();
};