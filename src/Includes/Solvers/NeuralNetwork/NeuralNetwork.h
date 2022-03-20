#include <vector>
#include <Maths/Matrix.h>

#pragma once
class NeuralNetwork
{
public:
	NeuralNetwork(int inputNodes, std::vector<int> hiddenLayerNodes, int outputNodes, float learnRate);
	~NeuralNetwork();
	std::vector<float> Query(std::vector<float> inputs);
	void Train(std::vector<float> inputs, std::vector<float> expected);

private:
	std::vector<Matrix<float>> weightsBetweenLayers;
	float learnRate;
};

