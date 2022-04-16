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

		//collect logs of initial networks movements, using the most successful movement paths as training data for future iterations
		//alternatively precalculate all paths to the exit and use that as training data, and cache this for subsequent runs
		//calculate the paths as a tree?

		//use maze exit as expected???
		//urgh gotta change the entire neuralnetwork to somehow work with mazes
		//allNetworks[train].train();
	}
	//train for trainsPerCycle times
}

void NeuralNetworkSolver::train(std::vector<int> startCoords, NeuralNetwork network) {

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

float NeuralNetworkSolver::getSuccess(std::vector<int> finalCoords, int steps) {
	float distance = 0.0f;

	for (int i = 0; i < finalCoords.size(); i++) {
		distance += pow(abs(mazeSolving.mazeExit[i] - finalCoords[i]), 2);
	}

	distance = sqrt(distance);

	return distance;
}

void NeuralNetworkSolver::setupNetworks() {
	for (int layer = 0; layer < maxLayersPerNetwork - minLayers; layer++) {

		for (int nodes = 3; nodes < 10; nodes++) {
			NetworksConfiguration config = NetworksConfiguration();
			config.layers = layer + minLayers;
			config.activeNetworks = std::vector<NeuralNetwork>(networksPerGroup);
			config.nodesPerLayer = std::vector<unsigned int>(config.layers);

			//each direction available to it (8) + its current location (4)
			config.nodesPerLayer[0] = 12; //8 + 4;
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

std::vector<float> NeuralNetworkSolver::networkInputForCoords(std::vector<int> coords) {
	unsigned int cellData = mazeSolving[coords];

	std::vector<float> out(12);

	for (int i = 0; i < 4; i++) {
		out.push_back(coords[i]);
	}

	for (int i = 0; i < 8; i++) {
		out.push_back((cellData & (1 << i)) >> i);
	}

	return out;
}

std::vector<std::vector<std::vector<int>>> NeuralNetworkSolver::getPathsTakenForNetworks(std::vector<int> startPosition, std::vector<NeuralNetwork> networks)
{
	std::vector<std::vector<std::vector<int>>> networkPaths;

	for (int i = 0; i < networkPaths.size(); i++) {
		networkPaths[i].push_back(startPosition);
	}

	for (int steps = 0; steps < 10; steps++) {
		for (int network = 0; network < networks.size(); network++) {
			std::vector<int> outs = NeuralNetwork::normalize(networks[network].query(networkInputForCoords(networkPaths[network][steps])));
			networkPaths[network].push_back(addCoords(networkPaths[network][steps], outs));
		}
	}
	

	return networkPaths;
}

std::vector<int> NeuralNetworkSolver::addCoords(std::vector<int> coords1, std::vector<int> coords2) {
	std::vector<int> out;
	
	for (int i = 0; i < 4; i++) {
		out.push_back(coords1[i] + coords2[i]);
	}

	return out;
}
