#include <string>
#include <iostream>
#include <vector>
#include "../Maths/Coordinate.h"

#pragma once

//The flags each bit of each maze cell contain.
enum MazeDirections {
	//so uh apparently the saved mazes are the wrong way around? I commented out what they should be
	FORWARD = (unsigned int) 16,//1,
	BACKWARD = (unsigned int) 32,//2,
	RIGHT = (unsigned int) 4,//4,
	LEFT = (unsigned int) 8,//8,
	UP = (unsigned int) 1,//16,
	DOWN = (unsigned int) 2,//32,
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
	Coordinate<int> mazeEntrance;
	Coordinate<int> mazeExit;
	std::vector<int> sizes;

	Maze();
	Maze(const std::vector<int>& dimensions);
	~Maze();
	void loadFromFile(std::string fileName);
	//Read the byte representing the cell at coordinates
	unsigned char operator[] (const Coordinate<int>& coordinates) const;
	//Provide the byte representing the cell at coordinates as a reference to modify
	unsigned char& operator[](const Coordinate<int>& coordinates);
	bool inBounds(const Coordinate<int>& coordinates);
	bool canAccessFrom(const Coordinate<int>& fromCoords, const Coordinate<int>& toCoords);
	bool wInBounds(int w);

private:
	//The data that makes up the maze.
	std::vector<unsigned char> mazeData;

	//C++ doesn't like this[] with header separated classes :/

	unsigned char getCell(const Coordinate<int>& coordinates) const;
	void setCell(const Coordinate<int>& coordinates, char to);
	unsigned int getSize();
};