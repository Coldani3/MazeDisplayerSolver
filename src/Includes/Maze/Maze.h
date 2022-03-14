#include <string>
#include <iostream>
#include <vector>

#pragma once

//The flags each bit of each maze cell contain.
enum MazeDirections {
	RIGHT = 1,
	LEFT = 2,
	UP = 4,
	DOWN = 8,
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
	std::vector<int> sizes;

	Maze(std::vector<int> dimensions);
	~Maze();
	void loadFromFile(std::string fileName);
	//access individual bytes
	char operator[] (std::vector<int> coordinates) const;
	char& operator[](std::vector<int> coordinates);

protected:
	//The data that makes up the maze.
	std::vector<char> mazeData;
};