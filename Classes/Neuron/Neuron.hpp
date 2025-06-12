//
//  Neuron.hpp
//  neuralNetworks
//
//  Created by Necati Sefercioğlu
//

#ifndef Neuron_hpp
#define Neuron_hpp

#include <vector>
#include <random>

class Neuron {
private:
    std::vector<double> weights;    // Weights for each input connection
    double bias;                    // Bias term
    std::vector<double> inputs;     // Stored inputs for weight updates
    double output;                  // Output after activation
    double gradient;                // Gradient for backpropagation
    int numInputs;                  // Number of inputs (size of previous layer)
    bool useReLU;                   // Flag to apply ReLU activation

    // Random number generator for weight initialization
    static std::default_random_engine generator;
    static std::uniform_real_distribution<double> distribution;

    // Activation function (ReLU)
    double relu(double x) const;

public:
    // Constructor: Initialize neuron with random weights and bias
    Neuron(int numInputs, bool useReLU = true);

    // Forward pass: Compute output given inputs
    double forward(const std::vector<double>& inputs);

    // Update weights and bias using gradient descent
    void updateWeights(double learningRate);

    // Getters
    double getOutput() const;
    double getGradient() const;
    const std::vector<double>& getWeights() const;

    // Setter for gradient (used by hidden layers during backpropagation)
    void setGradient(double grad);
};

#endif /* Neuron_hpp */
