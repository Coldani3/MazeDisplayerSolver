#include "Solver.h"

Solver::Solver(std::shared_ptr<Maze> maze, std::shared_ptr<MazePathManager> pathManager) {
	this->maze = maze;
	this->pathManager = pathManager;
}

float Solver::distanceBetween(const Coordinate<int>& coords1, const Coordinate<int>& coords2) {
	//4D pythagoras = sqrt(a^2 + b^2 + c^2 + d^2)
	return sqrtf(
			powf(
				abs(coords2.x() - coords1.x()), 
				2
			) + powf(
				abs(coords2.y() - coords1.y()),
				2
			) + powf(
				abs(coords2.z() - coords1.z()), 
				2
			) + powf(
				abs(coords2.w() - coords1.w()), 
				2
			)
	);
}

void Solver::solve() {

}

bool Solver::canAccessFrom(const Coordinate<int>& fromCoords, const Coordinate<int>& targetCoords) {
	std::vector<int> difference = (targetCoords - fromCoords).toVector();
	unsigned int side = 0;

	for (int i = 0; i < difference.size(); i++) {
		if (difference[i] != 0) {
			switch (i) {
				//TODO: I'm fairly sure that this is a temporary thing because of my maze generator using the wrong
				//directions. Go back to that and fix that change adn then fix it here.
				//X
				case 0:
					if (difference[i] > 0) {
						side = RIGHT;
					} else {
						side = LEFT;
					}

					break;
				//Y
				case 1:
					if (difference[i] > 0) {
						side = UP;
					} else {
						side = DOWN;
					}

					break;
				//Z
				case 2:
					if (difference[i] > 0) {
						side = FORWARD;
					} else {
						side = BACKWARD;
					}
					break;
				//W
				case 3:
					if (difference[i] > 0) {
						side = ANA;
					} else {
						side = KATA;
					}
					break;
				}
		}
	}
	
	unsigned int opposite = 0;

	//AAA because we eliminated the visited bit when saving from the generator. AAA represents a 101010 type pattern.
	//check if this is the left of each direction bit pair
	if ((side & 0xAAAAAAAAAAAAAAAA) > 0) {
		//if it is, shift one way, getting the opposite
		opposite = side >> 1;
	} else {
		//otherwise shift the other way
		opposite = side << 1;
	}

	return ((*maze)[targetCoords] & opposite) > 0;
}

void Solver::clear()
{
}
