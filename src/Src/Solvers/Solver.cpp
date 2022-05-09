#include <Solvers/Solver.h>

Solver::Solver(Maze maze, std::shared_ptr<RenderManager> renderer) {
	this->maze = maze;
	this->renderer = renderer;
}

float Solver::distanceBetween(std::vector<int> coords1, std::vector<int> coords2) {
	return sqrtf(pow(abs(coords2[0] - coords1[0]), 2) + pow(abs(coords2[2] - coords1[1]), 2) + pow(abs(coords2[2] - coords1[2]), 2) + pow(abs(coords2[3] - coords1[3]), 2));
}

std::vector<int> Solver::addCoords(std::vector<int> coords1, std::vector<int> coords2) {
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