#include <Solvers/Solver.h>
#include <vector>
#include <Solvers/NeuralNetwork/NeuralNetwork.h>
#include <Maze/Maze.h>

#pragma once
class SimpleNeuralNetworkSolver : public Solver
{
public:
	SimpleNeuralNetworkSolver(std::vector<unsigned int> nodesPerLayer, int networks, float learnRate, Maze maze);
	void solve();
	void train(NeuralNetwork network, std::vector<std::vector<int>> path);

	std::vector<std::vector<std::vector<int>>> queryNetworkPaths(std::vector<int> startPath);
	std::vector<float> networkInputForCoords(std::vector<int> coords);
	std::vector<int> addCoords(std::vector<int> coords1, std::vector<int> coords2);
	std::vector<int> minusCoords(std::vector<int> coords1, std::vector<int> coords2);
	std::vector<int> intify(std::vector<float> vec);
	std::vector<float> floatify(std::vector<int> vec);
	std::vector<std::vector<int>> getMostSuccessfulPath(std::vector<std::vector<std::vector<int>>> paths);

private:
	std::vector<NeuralNetwork> networks;
	Maze maze;
};

