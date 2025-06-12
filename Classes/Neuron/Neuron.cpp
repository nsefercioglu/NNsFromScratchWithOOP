//
//  Neuron.cpp
//  neuralNetworks
//
//  Created by Necati Sefercioğlu
//

#include "Neuron.hpp"
#include <stdexcept>
#include <cmath>

// Static member initialization for random weight initialization
std::default_random_engine Neuron::generator;
std::uniform_real_distribution<double> Neuron::distribution(-1.0, 1.0);

// Constructor: Initializes neuron with random weights and bias
Neuron::Neuron(int numInputs, bool useReLU) : numInputs(numInputs), useReLU(useReLU), output(0.0), gradient(0.0) {
    // Initialize weights with random values between -1 and 1
    weights.resize(numInputs);
    for (auto& w : weights) {
        w = distribution(generator);
    }
    // Initialize bias with random value between -1 and 1
    bias = distribution(generator);
}

// Applies ReLU activation function
double Neuron::relu(double x) const {
    return std::max(0.0, x);
}

// Computes the neuron's output
double Neuron::forward(const std::vector<double>& inputs) {
    // Validate input size
    if (inputs.size() != numInputs) {
        throw std::invalid_argument("Input size does not match number of weights");
    }
    // Store inputs for weight updates
    this->inputs = inputs;
    // Compute weighted sum
    double sum = bias;
    for (int i = 0; i < numInputs; ++i) {
        sum += weights[i] * inputs[i];
    }
    // Apply activation: ReLU if useReLU is true, otherwise linear
    output = useReLU ? relu(sum) : sum;
    return output;
}

// Updates weights and bias using gradient descent
void Neuron::updateWeights(double learningRate) {
    // Update each weight: w_new = w_old - learningRate * gradient * input
    for (int i = 0; i < numInputs; ++i) {
        weights[i] -= learningRate * gradient * inputs[i];
    }
    // Update bias: b_new = b_old - learningRate * gradient
    bias -= learningRate * gradient;
}

// Gets the neuron's output
double Neuron::getOutput() const {
    return output;
}

// Gets the neuron's gradient
double Neuron::getGradient() const {
    return gradient;
}

// Gets the neuron's weights
const std::vector<double>& Neuron::getWeights() const {
    return weights;
}

// Sets the neuron's gradient (for hidden layers)
void Neuron::setGradient(double grad) {
    gradient = grad;
}
