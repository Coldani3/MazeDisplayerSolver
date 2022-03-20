#include <Solvers/NeuralNetwork/NeuralNetwork.h>
#include <Maths/Matrix.h>

#include <vector>

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

	std::vector<Matrix<float>> layerOutputs(weightsBetweenLayers.size());

	layerOutputs[0] = Matrix<float>::sigmoid(weightsBetweenLayers[0] * inputsMatrix);

	//skip first layer weights
	for (int i = 1; i < weightsBetweenLayers.size(); i++) {
		layerOutputs[i] = Matrix<float>::sigmoid(weightsBetweenLayers[i] * layerOutputs[i - 1]);
	}

	Matrix<float> finalOutput = layerOutputs.back();

	//TODO: change this to use quasi-Newton?

	Matrix<float> errors = targetdOutputsMatrix - layerOutputs.back();

	weightsBetweenLayers.back() = weightsBetweenLayers.back() + (learnRate * errors * finalOutput * (1.0 - finalOutput) * Matrix<float>::transpose(layerOutputs[layerOutputs.size() - 2]));

	//note: for 1.0 - matrix, create new Matrix with defaultValue parameter of 1.0 and same size as matrix and subtract matrix from the 1.0 matrix

	
}