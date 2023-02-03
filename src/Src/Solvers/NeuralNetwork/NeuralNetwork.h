#ifndef CD3_MAZE_NEURAL_NETWORK
#define CD3_MAZE_NEURAL_NETWORK

#include <vector>
#include <memory>
#include "../../Math/Matrix.h"
#include <string>

#pragma once
class NeuralNetwork
{
public:
	NeuralNetwork();
	NeuralNetwork(std::vector<unsigned int> nodesForLayers, float learnRate);
	~NeuralNetwork();
	std::vector<float> query(std::vector<float> inputs);
	void train(std::vector<float> inputs, std::vector<float> expected);
	void save(std::string fileLocation);
	static NeuralNetwork load(std::string fileLocation);
	static std::vector<int> normalize(std::vector<float>);

private:
	std::vector<Matrix<float>> weightsBetweenLayers;
	float learnRate;
	std::vector<unsigned int> nodesForLayers;
};

#endif