#include <vector>
#include <memory>
#include <Maths/Matrix.h>
#include <string>

#pragma once
class NeuralNetwork
{
public:
	NeuralNetwork(std::vector<unsigned int> nodesForLayers, float learnRate);
	~NeuralNetwork();
	std::vector<float> query(std::vector<float> inputs);
	void train(std::vector<float> inputs, std::vector<float> expected);
	void save(std::string fileLocation);
	static NeuralNetwork load(std::string fileLocation);

private:
	std::vector<std::unique_ptr<Matrix<float>>> weightsBetweenLayers;
	float learnRate;
	std::vector<unsigned int> nodesForLayers;
};

