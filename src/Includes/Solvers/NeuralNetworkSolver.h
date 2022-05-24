#pragma once
#include <Solvers/Solver.h>
#include <vector>
#include <map>
#include <memory>
#include <Solvers/NeuralNetwork/NeuralNetwork.h>
#include <Maze/Maze.h>

struct NetworksConfiguration {
    int layers;
    float defaultLearnRate;
    std::vector<unsigned int> nodesPerLayer;
    std::vector<NeuralNetwork> activeNetworks;
    std::vector<float> networksSuccesses;
};

class NeuralNetworkSolver :
    public Solver
{
public:
    NeuralNetworkSolver(std::shared_ptr<Maze> maze, std::shared_ptr<RenderManager> renderer, int maxNetworkGroups, int networksPerGroup, int maxLayersPerNetwork, int trainsPerCycle);
    ~NeuralNetworkSolver();
    void solve();
    void train(std::vector<int>, NeuralNetwork network);
    std::map<NeuralNetwork, float> getSuccessOfNetworks();
    std::vector<NeuralNetwork> getAllNetworks();
    float getSuccess(std::vector<int> finalCoords, int steps);

private:
    Maze mazeSolving;
    std::vector<NetworksConfiguration> networkConfigurations;
    int maxNetworkGroups, networksPerGroup, maxLayersPerNetwork, trainsPerCycle;

    void setupNetworks();
    std::vector<float> networkInputForCoords(std::vector<int> coords);
    std::vector<std::vector<std::vector<int>>> getPathsTakenForNetworks(std::vector<int> startPosition, std::vector<NeuralNetwork> networks);
    std::vector<int> addCoords(std::vector<int> coords1, std::vector<int> coords2);
};

