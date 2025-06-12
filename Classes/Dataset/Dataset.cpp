//
//  Dataset.cpp
//  neuralNetworks
//
//  Created by Necati Sefercioğlu
//

#include "Dataset.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

// Constructor: Initialize dataset from CSV file
Dataset::Dataset(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;

        // Parse the label (first value in the row)
        if (!std::getline(ss, token, ',')) {
            throw std::runtime_error("Invalid line in file: " + line);
        }
        int label = std::stoi(token);

        // Parse the 784 pixel values
        std::vector<double> pixels;
        while (std::getline(ss, token, ',')) {
            double pixel = std::stod(token) / 255.0; // Normalize to [0,1]
            pixels.push_back(pixel);
        }

        // Verify that each sample has exactly 784 pixels
        if (pixels.size() != 784) {
            throw std::runtime_error("Expected 784 pixels, got " + std::to_string(pixels.size()));
        }

        // Store the parsed label and pixel values
        labels.push_back(label);
        data.push_back(pixels);
    }
    file.close();
}

// Get total number of samples
size_t Dataset::getNumSamples() const {
    return labels.size();
}

// Get label of sample at index
int Dataset::getLabel(size_t index) const {
    if (index >= labels.size()) {
        throw std::out_of_range("Index out of range");
    }
    return labels[index];
}

// Get pixel values of sample at index
const std::vector<double>& Dataset::getSample(size_t index) const {
    if (index >= data.size()) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}
