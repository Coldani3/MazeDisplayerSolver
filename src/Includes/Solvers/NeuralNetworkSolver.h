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
    NeuralNetworkSolver(Maze maze, int maxNetworkGroups, int networksPerGroup, int maxLayersPerNetwork, int trainsPerCycle);
    ~NeuralNetworkSolver();
    void solve();
    std::map<NeuralNetwork, float> getSuccessOfNetworks();
    std::vector<NeuralNetwork> getAllNetworks();

private:
    Maze mazeSolving;
    std::vector<NetworksConfiguration> networkConfigurations;
    int maxNetworkGroups, networksPerGroup, maxLayersPerNetwork, trainsPerCycle;

    void setupNetworks();
};

