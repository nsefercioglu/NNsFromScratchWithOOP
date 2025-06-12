//
//  Network.cpp
//  neuralNetworks
//
//  Created by Necati Sefercioğlu
//

#include "Network.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>

// Constructor: Initialize network with specified architecture and learning rate
Network::Network(const std::vector<int>& layerSizes, double learningRate)
    : inputSize(layerSizes[0]), outputSize(layerSizes.back()), learningRate(learningRate) {
    if (layerSizes.size() < 2) {
        throw std::invalid_argument("Network must have at least two layers (input and output)");
    }
    // Create layers based on the provided sizes, disabling ReLU for the output layer
    for (size_t i = 1; i < layerSizes.size(); ++i) {
        int numNeurons = layerSizes[i];
        int inputSize = layerSizes[i - 1];
        bool useReLU = (i < layerSizes.size() - 1); // true for hidden layers, false for output
        layers.emplace_back(numNeurons, inputSize, useReLU);
    }
}

// Add a new layer to the network
void Network::addLayer(int numNeurons, int inputSize) {
    layers.emplace_back(numNeurons, inputSize);
}

// Forward pass: Compute output probabilities given an input sample
std::vector<double> Network::forward(const std::vector<double>& input) {
    if (input.size() != inputSize) {
        throw std::invalid_argument("Input size does not match network input size");
    }
    std::vector<double> activations = input;
    for (auto& layer : layers) {
        activations = layer.forward(activations);
    }
    // Apply Softmax to the output layer
    double maxZ = *std::max_element(activations.begin(), activations.end());
    double sumExp = 0.0;
    for (auto& a : activations) {
        a = std::exp(a - maxZ); // For numerical stability
        sumExp += a;
    }
    for (auto& a : activations) {
        a /= sumExp;
    }
    return activations;
}

// Compute cross-entropy loss for a given sample and its label
double Network::computeLoss(const std::vector<double>& output, int label) const {
    if (label < 0 || label >= outputSize) {
        throw std::invalid_argument("Invalid label for loss computation");
    }
    double loss = -std::log(output[label] + 1e-10); // Add epsilon to avoid log(0)
    return loss;
}

// Backpropagation: Compute gradients and update weights for a given sample and label
void Network::backpropagate(const std::vector<double>& input, int label) {
    // Forward pass to get activations
    std::vector<std::vector<double>> activations;
    activations.push_back(input);
    std::vector<double> current = input;
    for (auto& layer : layers) {
        current = layer.forward(current);
        activations.push_back(current);
    }
    // Compute Softmax probabilities from output layer logits
    std::vector<double> logits = activations.back();
    std::vector<double> probabilities(logits.size());
    double maxZ = *std::max_element(logits.begin(), logits.end());
    double sumExp = 0.0;
    for (size_t i = 0; i < logits.size(); ++i) {
        probabilities[i] = std::exp(logits[i] - maxZ);
        sumExp += probabilities[i];
    }
    for (auto& p : probabilities) {
        p /= sumExp;
    }
    // Compute output layer gradients (p_i - y_i for Softmax + Cross-Entropy)
    std::vector<double> outputGradients(outputSize);
    for (int i = 0; i < outputSize; ++i) {
        outputGradients[i] = probabilities[i] - (i == label ? 1.0 : 0.0);
    }
    // Backpropagate through layers
    std::vector<double> nextLayerGradients = outputGradients;
    for (size_t l = layers.size() - 1; l < layers.size(); --l) {
        bool isOutputLayer = (l == layers.size() - 1);
        // Collect weights from the next layer (if not the output layer)
        std::vector<std::vector<double>> nextLayerWeights;
        if (!isOutputLayer && l + 1 < layers.size()) {
            for (const auto& neuron : layers[l + 1].getNeurons()) {
                nextLayerWeights.push_back(neuron.getWeights());
            }
        }
        layers[l].computeGradients(nextLayerGradients, nextLayerWeights, isOutputLayer, label);
        // Prepare gradients for the previous layer
        if (l > 0) {
            nextLayerGradients.clear();
            for (const auto& neuron : layers[l].getNeurons()) {
                nextLayerGradients.push_back(neuron.getGradient());
            }
        }
    }
    // Update weights
    for (auto& layer : layers) {
        layer.updateWeights(learningRate);
    }
}

// Train the network over multiple epochs using the training dataset
void Network::train(const Dataset& trainData, int epochs) {
    for (int epoch = 0; epoch < epochs; ++epoch) {
        double totalLoss = 0.0;
        for (size_t i = 0; i < trainData.getNumSamples(); ++i) {
            const auto& sample = trainData.getSample(i);
            int label = trainData.getLabel(i);
            // Forward pass
            auto output = forward(sample);
            // Compute loss
            double loss = computeLoss(output, label);
            totalLoss += loss;
            // Backpropagation
            backpropagate(sample, label);
        }
        // Print average loss for the epoch
        std::cout << "Epoch " << epoch + 1 << ", Loss: " << totalLoss / trainData.getNumSamples() << std::endl;
    }
}

// Test the network on the test dataset and compute accuracy
double Network::test(const Dataset& testData) {
    int correct = 0;
    for (size_t i = 0; i < testData.getNumSamples(); ++i) {
        const auto& sample = testData.getSample(i);
        int label = testData.getLabel(i);
        // Forward pass
        auto output = forward(sample);
        // Predict the digit with the highest probability
        ptrdiff_t predicted = std::distance(output.begin(), std::max_element(output.begin(), output.end()));
        if (predicted == label) {
            correct++;
        }
    }
    // Compute and return accuracy
    double accuracy = static_cast<double>(correct) / testData.getNumSamples();
    std::cout << "Test Accuracy: " << accuracy << std::endl;
    return accuracy;
}
