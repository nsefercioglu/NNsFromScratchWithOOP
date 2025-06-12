//
//  Dataset.hpp
//  neuralNetworks
//
//  Created by Necati Sefercioğlu
//

#ifndef Dataset_hpp
#define Dataset_hpp

#include <vector>
#include <string>

// Class representing a dataset of MNIST samples
class Dataset {
private:
    std::vector<int> labels;                // Vector storing the label of each sample (0-9)
    std::vector<std::vector<double>> data;  // Vector storing the pixel values for each sample

public:
    // Constructor: Initialize dataset from CSV file
    Dataset(const std::string& filename);

    // Get total number of samples
    size_t getNumSamples() const;

    // Get label of sample at index
    int getLabel(size_t index) const;

    // Get pixel values of sample at index
    const std::vector<double>& getSample(size_t index) const;
};

#endif /* Dataset_hpp */
