//
//  Network.hpp
//  neuralNetworks
//
//  Created by Necati Sefercioğlu
//

#ifndef Network_hpp
#define Network_hpp

#include "Layer.hpp"
#include "Dataset.hpp"
#include <vector>
#include <stdexcept>

// Class representing a neural network composed of layers
class Network {
private:
    std::vector<Layer> layers;      // Sequence of layers in the network
    int inputSize;                  // Number of input features (784 for MNIST)
    int outputSize;                 // Number of output classes (10 for digits 0-9)
    double learningRate;            // Learning rate for gradient descent

public:
    // Constructor: Initialize network with specified architecture and learning rate
    Network(const std::vector<int>& layerSizes, double learningRate);

    // Add a new layer to the network
    void addLayer(int numNeurons, int inputSize);

    // Forward pass: Compute output probabilities given an input sample
    std::vector<double> forward(const std::vector<double>& input);

    // Compute cross-entropy loss for a given sample and its label
    double computeLoss(const std::vector<double>& output, int label) const;

    // Backpropagation: Compute gradients and update weights for a given sample and label
    void backpropagate(const std::vector<double>& input, int label);

    // Train the network over multiple epochs using the training dataset
    void train(const Dataset& trainData, int epochs);

    // Test the network on the test dataset and compute accuracy
    double test(const Dataset& testData);
};

#endif /* Network_hpp */
