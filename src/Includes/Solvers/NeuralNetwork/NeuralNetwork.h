#include <vector>
#include <memory>
#include <Maths/Matrix.h>

#pragma once
class NeuralNetwork
{
public:
	NeuralNetwork(std::vector<int> nodesForLayers, float learnRate);
	~NeuralNetwork();
	std::vector<float> Query(std::vector<float> inputs);
	void Train(std::vector<float> inputs, std::vector<float> expected);

private:
	std::vector<std::unique_ptr<Matrix<float>>> weightsBetweenLayers;
	float learnRate;
};

