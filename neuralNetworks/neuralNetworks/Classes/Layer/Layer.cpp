//
//  Layer.cpp
//  neuralNetworks
//
//  Created by Necati Sefercioğlu
//

#include "Layer.hpp"

// Constructor: Initializes a layer with a specified number of neurons, each taking inputSize inputs
Layer::Layer(int numNeurons, int inputSize, bool useReLU) : numNeurons(numNeurons), inputSize(inputSize), layerUseReLU(useReLU) {
    // Create numNeurons neurons, each initialized with a weight vector of size inputSize and useReLU flag
    for (int i = 0; i < numNeurons; ++i) {
        neurons.emplace_back(inputSize, useReLU);
    }
}

// Adds a new neuron to the layer
void Layer::addNeuron() {
    // Append a new neuron with a weight vector of size inputSize and the layer's useReLU setting
    neurons.emplace_back(inputSize, layerUseReLU);
    // Increment the count of neurons in the layer
    numNeurons++;
}

// Forward pass: Computes the output of each neuron in the layer given the inputs
std::vector<double> Layer::forward(const std::vector<double>& inputs) {
    // Validate that the input size matches the expected input size for the layer
    if (inputs.size() != inputSize) {
        throw std::invalid_argument("Input size does not match layer's input size");
    }
    // Store the outputs of all neurons in the layer
    std::vector<double> outputs;
    // Compute the forward pass for each neuron and collect its output
    for (auto& neuron : neurons) {
        outputs.push_back(neuron.forward(inputs));
    }
    // Return the vector of neuron outputs
    return outputs;
}

// Computes gradients for all neurons in the layer during backpropagation
void Layer::computeGradients(const std::vector<double>& nextLayerGradients,
                             const std::vector<std::vector<double>>& nextLayerWeights,
                             bool isOutputLayer, int target) {
    if (isOutputLayer) {
        for (int i = 0; i < numNeurons; ++i) {
            // dL/da_i is provided by nextLayerGradients (p_i - y_i)
            double dL_da = nextLayerGradients[i];
            // da_i/dz_i = 1 since output layer is linear (no ReLU)
            double da_dz = 1.0;
            // dL/dz_i = dL/da_i * da_i/dz_i
            neurons[i].setGradient(dL_da * da_dz);
        }
    } else {
        // Hidden layer computation
        for (int i = 0; i < numNeurons; ++i) {
            double gradSum = 0.0;
            for (int j = 0; j < nextLayerGradients.size(); ++j) {
                gradSum += nextLayerWeights[j][i] * nextLayerGradients[j];
            }
            // da_i/dz_i = 1 if ReLU input z_i > 0, else 0; since a_i = ReLU(z_i), check a_i > 0
            neurons[i].setGradient(gradSum * (neurons[i].getOutput() > 0 ? 1.0 : 0.0));
        }
    }
}

// Updates weights and biases of all neurons in the layer using gradient descent
void Layer::updateWeights(double learningRate) {
    // Iterate through each neuron in the layer
    for (auto& neuron : neurons) {
        // Update the neuron's weights and bias based on its gradient and learning rate
        neuron.updateWeights(learningRate);
    }
}

// Getter: Returns a const reference to the vector of neurons in the layer
const std::vector<Neuron>& Layer::getNeurons() const {
    return neurons;
}
