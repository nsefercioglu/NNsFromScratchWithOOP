//
//  Layer.hpp
//  neuralNetworks
//
//  Created by Necati Sefercioğlu
//

#ifndef Layer_hpp
#define Layer_hpp

#include "Neuron.hpp"
#include <vector>
#include <stdexcept>

// Class representing a layer of neurons in a neural network
class Layer {
private:
    std::vector<Neuron> neurons;    // Collection of neurons in the layer
    int numNeurons;                 // Number of neurons in the layer
    int inputSize;                  // Number of inputs each neuron expects (size of previous layer)
    bool layerUseReLU;              // Flag to determine if neurons in this layer use ReLU

public:
    // Constructor: Initializes a layer with a specified number of neurons, input size, and ReLU flag
    Layer(int numNeurons, int inputSize, bool useReLU = true);

    // Adds a new neuron to the layer
    void addNeuron();

    // Forward pass: Computes outputs for all neurons given the input vector
    std::vector<double> forward(const std::vector<double>& inputs);

    // Computes gradients for neurons, handling output and hidden layers differently
    void computeGradients(const std::vector<double>& nextLayerGradients,
                          const std::vector<std::vector<double>>& nextLayerWeights,
                          bool isOutputLayer, int target);

    // Updates weights and biases of all neurons using gradient descent
    void updateWeights(double learningRate);

    // Getter: Returns a const reference to the vector of neurons
    const std::vector<Neuron>& getNeurons() const;
};

#endif /* Layer_hpp */
