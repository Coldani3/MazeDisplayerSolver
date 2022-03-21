#include <Solvers/NeuralNetwork/NeuralNetwork.h>
#include <Maths/Matrix.h>

#include <vector>
#include <memory>
#include <random>

NeuralNetwork::NeuralNetwork(int inputNodes, std::vector<int> hiddenLayerNodes, int outputNodes, float learnRate) {

}

NeuralNetwork::~NeuralNetwork() {

}

std::vector<float> NeuralNetwork::Query(std::vector<float> inputs) {
	Matrix<float> inputsMatrix(Matrix<float>::inputArrayToRow(inputs));

	Matrix<float> layerOutput = Matrix<float>::sigmoid(weightsBetweenLayers[0] * inputsMatrix);

	for (int i = 1; i < weightsBetweenLayers.size(); i++) {
		layerOutput = Matrix<float>::sigmoid(weightsBetweenLayers[i] * layerOutput);
	}

	return Matrix<float>::flatten(layerOutput);
}

void NeuralNetwork::Train(std::vector<float> inputs, std::vector<float> expected) {
	Matrix<float> inputsMatrix(Matrix<float>::inputArrayToRow(inputs));
	Matrix<float> targetdOutputsMatrix(Matrix<float>::inputArrayToRow(expected));

	std::vector<std::unique_ptr<Matrix<float>>> layerOutputs(weightsBetweenLayers.size());

	layerOutputs[0] = std::make_unique<Matrix<float>>(Matrix<float>::sigmoid(weightsBetweenLayers[0] * inputsMatrix));

	//skip first layer weights
	for (int i = 1; i < weightsBetweenLayers.size(); i++) {
		layerOutputs[i] = std::make_unique<Matrix<float>>(Matrix<float>::sigmoid(weightsBetweenLayers[i] * *layerOutputs[i - 1]));
	}

	Matrix<float> finalOutput = *layerOutputs.back();

	//TODO: change this to use quasi-Newton?
	//TODO: adjust learnRate dynamically based on the direction we're going (ie. shrink if we suddenly turn around)

	Matrix<float> errors = targetdOutputsMatrix - finalOutput;

	std::vector<std::unique_ptr<Matrix<float>>> errorPerLayer(weightsBetweenLayers.size());
	int index = weightsBetweenLayers.size() - 1;

	for (auto iter = weightsBetweenLayers.end(); iter != weightsBetweenLayers.begin(); iter--) {
		errorPerLayer[index] = std::make_unique<Matrix<float>>(Matrix<float>::transpose(*iter) * errors);
		index--;
	}

	Matrix<float> layerErrors = *errorPerLayer.back();

	//+= doesn't work for some reason so we do this instead
	weightsBetweenLayers.back() = weightsBetweenLayers.back() + (learnRate * errors * finalOutput * (1.0 - finalOutput) * Matrix<float>::transpose(*layerOutputs[layerOutputs.size() - 2]));

	for (int i = weightsBetweenLayers.size() - 2; i > 0; i--) {
		weightsBetweenLayers[i] = weightsBetweenLayers[i] + (learnRate * /*layerErrors */ layerErrors * *layerOutputs[i] * (1.0 - *layerOutputs[i]) * Matrix<float>::transpose(*layerOutputs[i - 1]));
		layerErrors = *errorPerLayer[i];
	}

	//TODO: figure out if we should be using some calculated errors value from a closer to output layer
	Matrix<float> inputLayerErrors = *errorPerLayer.front();

	weightsBetweenLayers.front() = weightsBetweenLayers.front() + (learnRate * inputLayerErrors * *layerOutputs[0] * (1.0 - *layerOutputs[0]) * Matrix<float>::transpose(inputsMatrix));
	//note: for 1.0 - matrix, create new Matrix with defaultValue parameter of 1.0 and same size as matrix and subtract matrix from the 1.0 matrix

	
}