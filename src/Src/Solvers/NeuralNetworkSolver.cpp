#include <Solvers/NeuralNetworkSolver.h>

const int minLayers = 3;

NeuralNetworkSolver::NeuralNetworkSolver(Maze maze, int maxNetworkGroups, int networksPerGroup, int maxLayersPerNetwork, int trainsPerCycle) {
	this->maxNetworkGroups = maxNetworkGroups;
	this->networksPerGroup = networksPerGroup;
	this->maxLayersPerNetwork = maxLayersPerNetwork;
	this->trainsPerCycle = trainsPerCycle;
	this->mazeSolving = maze;
	networkConfigurations = std::vector<NetworksConfiguration>(maxLayersPerNetwork - minLayers);
}

NeuralNetworkSolver::~NeuralNetworkSolver() {
}

void NeuralNetworkSolver::solve() {
	setupNetworks();
}

std::map<NeuralNetwork, float> NeuralNetworkSolver::getSuccessOfNetworks() {
	return std::map<NeuralNetwork, float>();
}

void NeuralNetworkSolver::setupNetworks() {
	for (int layer = 0; layer < maxLayersPerNetwork - minLayers; layer++) {

		//for (int j = 0; j < )
		NetworksConfiguration config = NetworksConfiguration();
		config.layers = layer + minLayers;
		config.activeNetworks = std::vector<NeuralNetwork>(networksPerGroup);
		
		networkConfigurations.push_back(config);
	}
}
