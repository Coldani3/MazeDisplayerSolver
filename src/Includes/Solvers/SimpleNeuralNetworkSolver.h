#include <Solvers/Solver.h>
#include <vector>
#include <Solvers/NeuralNetwork/NeuralNetwork.h>
#include <Maze/Maze.h>

#pragma once
class SimpleNeuralNetworkSolver : public Solver
{
public:
	const int maxQueries = 4;
	const int trainsPerCycle = 50;

	SimpleNeuralNetworkSolver(std::vector<unsigned int> nodesPerLayer, int networks, float learnRate, std::shared_ptr<Maze> maze, std::shared_ptr<MazePathManager> pathManager);
	void solve();
	void train(NeuralNetwork network, std::vector<Coordinate<int>> path);

	std::vector<std::vector<Coordinate<int>>> queryNetworkPaths(const Coordinate<int>& startPath);
	std::vector<float> networkInputForCoords(const Coordinate<int>& coords);
	std::vector<std::vector<int>> getMostSuccessfulPath(std::vector<std::vector<std::vector<int>>> paths);
	//block direction being the direction the path came from to exclude going back and just solving the whole maze again.
	bool validateCanReachExitFrom(const Coordinate<int>&, unsigned char blockDirection);

private:
	std::vector<NeuralNetwork> networks;
};

