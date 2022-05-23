#include <Solvers/Solver.h>

Solver::Solver(Maze maze, std::shared_ptr<MainRenderManager> renderer) {
	this->maze = maze;
	this->renderer = renderer;
}

float Solver::distanceBetween(std::vector<int> coords1, std::vector<int> coords2) {
	return sqrtf(pow(abs(coords2[0] - coords1[0]), 2) + pow(abs(coords2[2] - coords1[1]), 2) + pow(abs(coords2[2] - coords1[2]), 2) + pow(abs(coords2[3] - coords1[3]), 2));
}

void Solver::solve() {

}

std::vector<int> Solver::addCoords(std::vector<int> coords1, std::vector<int> coords2) const {
	std::vector<int> out;

	for (int i = 0; i < 4; i++) {
		out.push_back(coords1[i] + coords2[i]);
	}

	return out;
}

std::vector<float> Solver::floatify(std::vector<int> vec) {
	std::vector<float> out(vec.size());

	for (int i = 0; i < vec.size(); i++) {
		out[i] = (float)vec[i];
	}

	return out;
}

std::vector<int> Solver::intify(std::vector<float> vec) {
	std::vector<int> out(vec.size());

	for (int i = 0; i < vec.size(); i++) {
		out[i] = (int)vec[i];
	}

	return out;
}

std::vector<int> Solver::minusCoords(std::vector<int> coords1, std::vector<int> coords2) {
	std::vector<int> out;

	for (int i = 0; i < 4; i++) {
		out.push_back(coords1[i] - coords2[i]);
	}

	return out;
}

bool Solver::canAccessFrom(std::vector<int> fromCoords, std::vector<int> targetCoords) {
	std::vector<int> difference = minusCoords(targetCoords, fromCoords);
	unsigned int side = 0;

	for (int i = 0; i < difference.size(); i++) {
		if (difference[i] != 0) {
			switch (i) {
				case 0:
					if (difference[i] > 0) {
						side = RIGHT;
					}
					else {
						side = LEFT;
					}

					break;
				case 1:
					

					if (difference[i] > 0) {
						side = UP;
					}
					else {
						side = DOWN;
					}

					break;
				case 2:
					if (difference[i] > 0) {
						side = FORWARD;
					} else {
						side = BACKWARD;
					}
					break;
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

	//AAA because we eliminated the visited bit when saving from the generator
	if ((side & 0xAAAAAAAAAAAAAAAA) > 0) {
		//if it is, shift one way, getting the opposite
		opposite = side >> 1;
	} else {
		//otherwise shift the other way
		opposite = side << 1;
	}

	return (maze[targetCoords] & opposite) > 0;
}

void Solver::clear()
{
}
