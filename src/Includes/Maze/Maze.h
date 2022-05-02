#include <string>
#include <iostream>
#include <vector>

#pragma once

//The flags each bit of each maze cell contain.
enum MazeDirections {
	UP = 1,
	DOWN = 2,
	RIGHT = 4,
	LEFT = 8,
	FORWARD = 16,
	BACKWARD = 32,
	ANA = 64,
	KATA = 128
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