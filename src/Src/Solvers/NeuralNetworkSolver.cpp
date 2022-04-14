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

	std::vector<int> startMazePos = mazeSolving.mazeEntrance;

	std::vector<NeuralNetwork> allNetworks = getAllNetworks();
	std::vector<std::vector<int>> positionsOfNetworks;
	//calculated in the order x y z w
	std::vector<int> distances;

	//set all networks to the start position
	for (int i = 0; i < allNetworks.size(); i++) {
		positionsOfNetworks.push_back(startMazePos);
	}

	for (int train = 0; train < trainsPerCycle; train++) {
		int directionsAvailable = mazeSolving[positionsOfNetworks[train]];

		for (int i = 0; i < startMazePos.size(); i++) {
			distances.push_back(positionsOfNetworks[train][i]);
			distances.push_back(mazeSolving.sizes[i] - positionsOfNetworks[train][i]);
		}

		//urgh gotta change the entire neuralnetwork to somehow work with mazes
		allNetworks[train].train()
	}
	//train for trainsPerCycle times
}

std::map<NeuralNetwork, float> NeuralNetworkSolver::getSuccessOfNetworks() {
	std::map<NeuralNetwork, float> out;

	for (int i = 0; i < networkConfigurations.size(); i++) {
		for (int network = 0; network < networkConfigurations[i].activeNetworks.size(); network++) {
			out.insert(std::pair<NeuralNetwork, float>(networkConfigurations[i].activeNetworks[network], networkConfigurations[i].networksSuccesses[network]));
		}
	}

	return out;
}

std::vector<NeuralNetwork> NeuralNetworkSolver::getAllNetworks() {
	std::vector<NeuralNetwork> out;
	for (int config = 0; config < networkConfigurations.size(); config++) {
		for (int network = 0; network < networkConfigurations[config].activeNetworks.size(); network++) {
			out.push_back(networkConfigurations[config].activeNetworks[network]);
		}
	}

	return out;
}

void NeuralNetworkSolver::setupNetworks() {
	for (int layer = 0; layer < maxLayersPerNetwork - minLayers; layer++) {

		for (int nodes = 3; nodes < 10; nodes++) {
			NetworksConfiguration config = NetworksConfiguration();
			config.layers = layer + minLayers;
			config.activeNetworks = std::vector<NeuralNetwork>(networksPerGroup);
			config.nodesPerLayer = std::vector<unsigned int>(config.layers);

			//distance to walls (2 * 4) + each direction available to it (8) + its current location (4)
			config.nodesPerLayer[0] = 20; //(2 * 4) + 8 + 4;
			//total maze + each direction available to it at any given time + its current location
			//config.nodesPerLayer[0] = mazeSolving.getSize() + 8 + 4;
			//4D coordinate change
			config.nodesPerLayer.back() = 4;

			for (int i = 1; i < config.nodesPerLayer.size() - 1; i++) {
				config.nodesPerLayer[i] = nodes;
			}

			for (int network = 0; network < networksPerGroup; network++) {
				config.activeNetworks.push_back(NeuralNetwork(config.nodesPerLayer, config.defaultLearnRate));
			}

			networkConfigurations.push_back(config);
		}
	}
}
