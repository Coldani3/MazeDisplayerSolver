#include "NeuralNetwork.h"
#include "../../Math/Matrix.h"

#include <vector>
#include <memory>
#include <random>
#include <iostream>
#include <fstream>

NeuralNetwork::NeuralNetwork()
{
}

//nodesForLayers[0] = input layer nodes
NeuralNetwork::NeuralNetwork(std::vector<unsigned int> nodesForLayers, float learnRate) {
	weightsBetweenLayers = std::vector<Matrix<float>>(nodesForLayers.size());
	this->nodesForLayers = nodesForLayers;

	srand((unsigned)time(0));

	for (int i = 0; i < nodesForLayers.size() - 1; i++) {
		weightsBetweenLayers[i] = Matrix<float>(nodesForLayers[i + 1], nodesForLayers[i]);
		//TODO: randomise weights

		for (int row = 0; row < weightsBetweenLayers[i].rows; row++) {
			for (int col = 0; col < weightsBetweenLayers[i].columns; col++) {
				weightsBetweenLayers[i][row][col] = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX + 1.0f)) - 0.5;
			}
		}
	}

	this->learnRate = learnRate;
}

NeuralNetwork::~NeuralNetwork() {

}

std::vector<float> NeuralNetwork::query(std::vector<float> inputs) {
	Matrix<float> inputsMatrix(Matrix<float>::inputArrayToRow(inputs));

	Matrix<float> layerOutput = Matrix<float>::sigmoid(weightsBetweenLayers[0] * inputsMatrix);

	for (int i = 1; i < weightsBetweenLayers.size(); i++) {
		layerOutput = Matrix<float>::sigmoid(weightsBetweenLayers[i] * layerOutput);
	}

	return Matrix<float>::flatten(layerOutput);
}

void NeuralNetwork::train(std::vector<float> inputs, std::vector<float> expected) {
	Matrix<float> inputsMatrix(Matrix<float>::inputArrayToRow(inputs));
	Matrix<float> targetdOutputsMatrix(Matrix<float>::inputArrayToRow(expected));

	std::vector<std::unique_ptr<Matrix<float>>> layerOutputs(weightsBetweenLayers.size());

	layerOutputs[0] = std::make_unique<Matrix<float>>(Matrix<float>::tanH/*sigmoid*/(weightsBetweenLayers[0] * inputsMatrix));

	//get the outputs for each layer so we can calculate the errors
	//skip first layer weights
	for (int i = 1; i < weightsBetweenLayers.size(); i++) {
		layerOutputs[i] = std::make_unique<Matrix<float>>(Matrix<float>::tanH/*sigmoid*/(weightsBetweenLayers[i] * *layerOutputs[i - 1]));
	}

	Matrix<float> finalOutput = *layerOutputs.back();

	//TODO: change this to use quasi-Newton?
	//TODO: adjust learnRate dynamically based on the direction we're going (ie. shrink if we suddenly turn around)

	Matrix<float> errors = targetdOutputsMatrix - finalOutput;

	std::vector<std::unique_ptr<Matrix<float>>> errorPerLayer(weightsBetweenLayers.size());
	size_t index = weightsBetweenLayers.size() - 1;

	/*calculate each layer's errors by multiplying that layer's weights by the total errors
	  (TODO: should we be using the previous layer's error per layer? this feels like it shouldn't work with matrix multiplication because
	  sizes and all that)*/
	for (auto iter = weightsBetweenLayers.end(); iter != weightsBetweenLayers.begin(); iter--) {
		errorPerLayer[index] = std::make_unique<Matrix<float>>(Matrix<float>::transpose(*iter) * errors);
		index--;
	}

	Matrix<float> layerErrors = *errorPerLayer.back();

	//+= doesn't work for some reason so we do this instead
	weightsBetweenLayers.back() = weightsBetweenLayers.back() + (learnRate * errors * (1 - (finalOutput * finalOutput))/*finalOutput * (1.0 - finalOutput)*/ * Matrix<float>::transpose(*layerOutputs[layerOutputs.size() - 2]));

	for (size_t i = weightsBetweenLayers.size() - 2; i > 0; i--) {
		weightsBetweenLayers[i] = weightsBetweenLayers[i] + (learnRate * layerErrors * (1- (*layerOutputs[i] * *layerOutputs[i]))/**layerOutputs[i] * (1.0 - *layerOutputs[i])*/ * Matrix<float>::transpose(*layerOutputs[i - 1]));
		layerErrors = *errorPerLayer[i];
	}

	//TODO: figure out if we should be using some calculated errors value from a closer to output layer
	Matrix<float> inputLayerErrors = *errorPerLayer.front();

	weightsBetweenLayers.front() = weightsBetweenLayers.front() + (learnRate * inputLayerErrors * (1 - (*layerOutputs[0] * *layerOutputs[0]))/**layerOutputs[0] * (1.0 - *layerOutputs[0])*/ * Matrix<float>::transpose(inputsMatrix));
	//note: for 1.0 - matrix, create new Matrix with defaultValue parameter of 1.0 and same size as matrix and subtract matrix from the 1.0 matrix

	//learn rate * errors of layer above (closer to out) * outputs of layer above * (1.0 - outputs of layer above) * outputs of this layer transposed
}

// <layersCount> <nodesForLayers (length layersCount)> <learnRate> <layer1Weights (length layersSize 1)> ... <layerNWeights (length layersSize N)>
void NeuralNetwork::save(std::string fileName) {
	std::ofstream outStream;
	outStream.open(fileName);

	outStream << (char)nodesForLayers.size();

	for (int i = 0; i < nodesForLayers.size(); i++) {
		outStream << (char) nodesForLayers[i];
	}

	outStream << learnRate;

	for (int i = 0; i < weightsBetweenLayers.size(); i++) {
		Matrix<float> layer = weightsBetweenLayers[i];

		for (int row = 0; row < layer.rows; row++) {
			for (int col = 0; col < layer.columns; col++) {
				outStream << layer[row][col];
			}
		}
	}

	outStream.close();
}

//man isn't this just so much more sane than that string split nonsense
NeuralNetwork NeuralNetwork::load(std::string fileName) {
	std::ifstream inStream;
	inStream.open(fileName);
	char currentChar;
	//char* readBytes;
	inStream.get(currentChar);

	int layers = currentChar;

	std::vector<unsigned int> nodesForLayers = std::vector<unsigned int>(layers);

	for (int i = 0; i < layers; i++) {
		inStream.read((char*) &nodesForLayers[i], sizeof(unsigned int));
	}

	float learnRate;
	inStream.read((char*)&learnRate, sizeof(float));

	NeuralNetwork out(nodesForLayers, learnRate);

	for (int i = 0; i < out.weightsBetweenLayers.size() - 1; i++) {
		Matrix<float> matrix = out.weightsBetweenLayers[i];

		for (int row = 0; row < matrix.rows; row++) {
			for (int col = 0; col < matrix.columns; col++) {
				inStream.read((char*) &matrix[row][col], sizeof(float));
			}
		}
	}

	inStream.close();

	return out;
}

std::vector<int> NeuralNetwork::normalize(std::vector<float> output)
{
	std::vector<int> out(output.size());

	for (int i = 0; i < output.size(); i++) {
		out.push_back(static_cast<int>(roundf(output[i])));
	}

	return out;
}
