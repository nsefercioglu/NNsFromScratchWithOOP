//
//  main.cpp
//  neuralNetworks
//
//  Created by Necati Sefercioğlu
//

#include "GUI.hpp"
#include "Input.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

// Constant parameters for training
const double LEARNING_RATE = 0.01;
const int EPOCHS = 50;

// Main function to run the neural network simulation with GUI
int main() {
    try {
        // Initialize SFML window
        sf::RenderWindow window(sf::VideoMode(1000, 600), "Neural Network Simulation");
        window.setFramerateLimit(60); // Limit frame rate for smoother display

        // Initialize input display
        Input inputDisplay;

        // Load datasets
        std::string trainPath = "...";
        std::string testPath = "...";
        Dataset trainData(trainPath);
        Dataset testData(testPath);

        // Print dataset sizes
        std::cout << "Training samples: " << trainData.getNumSamples() << std::endl;
        std::cout << "Test samples: " << testData.getNumSamples() << std::endl;

        // Initialize GUI
        GUI gui(window, inputDisplay, trainData, testData, LEARNING_RATE, EPOCHS);

        // Main loop
        while (window.isOpen()) {
            // Handle events through GUI
            gui.handleEvents();

            // Draw the GUI
            gui.draw();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
