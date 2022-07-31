#include <Solvers/SimpleNeuralNetworkSolver.h>
#include <algorithm>

SimpleNeuralNetworkSolver::SimpleNeuralNetworkSolver(std::vector<unsigned int> nodesPerLayer, int networks, float learnRate, std::shared_ptr<Maze> maze, std::shared_ptr<MainRenderManager> renderer) : Solver(maze, renderer) {
	for (int i = 0; i < networks; i++) {
		this->networks.push_back(NeuralNetwork(nodesPerLayer, learnRate));
	}
}

std::vector<float> SimpleNeuralNetworkSolver::networkInputForCoords(std::vector<int> coords) {
	unsigned int cellData = (*maze)[coords];

	std::vector<float> out(12);

	for (int i = 0; i < 4; i++) {
		out.push_back(coords[i]);
	}

	for (int i = 0; i < 8; i++) {
		out.push_back((cellData & (1 << i)) >> i);
	}

	return out;
}

std::vector<std::vector<int>> SimpleNeuralNetworkSolver::getMostSuccessfulPath(std::vector<std::vector<std::vector<int>>> paths) {
	std::vector<float> successes(paths.size());
	
	//all paths are the same length so the closest should be the best path.
	for (int i = 0; i < paths.size(); i++) {
		std::vector<int> lastCoord = paths[i].back();

		float distance = distanceBetween(lastCoord, maze->mazeExit);

		successes[i] = distance;
	}

	return paths[std::distance(successes.begin(), std::max_element(successes.begin(), successes.end()))];
}

std::vector<std::vector<std::vector<int>>> SimpleNeuralNetworkSolver::queryNetworkPaths(std::vector<int> startPath) {
	//network -> list of coords -> each coord object
	std::vector<std::vector<std::vector<int>>> paths;

	//add start path to start of all paths
	for (int i = 0; i < networks.size(); i++) {
		paths[i].push_back(startPath);
	}

	//for every network, query for maxQueries steps.
	for (int network = 0; network < networks.size(); network++) {
		//TODO: isn't this just querying it for every coordinate in the path?
		for (int query = 0; query < maxQueries; query++) {
			//get current coordinate
			std::vector<int> difference = NeuralNetwork::normalize(networks[network].query(networkInputForCoords(paths[network][query])));
			paths[network].push_back(addCoords(paths[network][query], difference));
		}
	}

	return paths;
}

void SimpleNeuralNetworkSolver::solve() {
	//query all networks and pick the best path
	std::vector<std::vector<std::vector<int>>> paths = queryNetworkPaths(maze->mazeEntrance);
	std::vector<float> successes(paths.size());

	//all paths are the same length so the closest should be the best path.
	for (int i = 0; i < paths.size(); i++) {
		std::vector<int> lastCoord = paths[i].back();

		float distance = distanceBetween(lastCoord, maze->mazeExit);//sqrtf(pow(lastCoord[0], 2) + pow(lastCoord[1], 2) + pow(lastCoord[2], 2) + pow(lastCoord[3], 2));

		successes[i] = distance;
	}

	std::vector<std::vector<int>> mostSuccessfulPath = paths[std::distance(successes.begin(), std::max_element(successes.begin(), successes.end()))];

	//use most successful path to train AIs
	
	for (int network = 0; network < networks.size(); network++) {
		for (int i = 0; i < trainsPerCycle; i++) {
			train(networks[network], mostSuccessfulPath);
		}
	}

	queryNetworkPaths(mostSuccessfulPath.back());
}

void SimpleNeuralNetworkSolver::train(NeuralNetwork network, std::vector<std::vector<int>> path) {
	for (int i = 0; i < path.size() - 1; i++) {
		std::vector<int> expected = minusCoords(path[i + 1], path[i]);

		std::vector<float> input = networkInputForCoords(expected);

		network.train(input, floatify(expected));
	}
}