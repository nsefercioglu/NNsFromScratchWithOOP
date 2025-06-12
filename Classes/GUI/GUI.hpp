//
//  GUI.hpp
//  neuralNetworks
//
//  Created by Necati Sefercioğlu
//

#ifndef GUI_hpp
#define GUI_hpp

#include "Button.hpp"
#include "Input.hpp"
#include "Network.hpp"
#include "Dataset.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// Class representing the graphical user interface for the neural network simulation
class GUI {
private:
    sf::RenderWindow& window;              // Reference to the SFML window for rendering
    Input& inputDisplay;                   // Reference to the input display for MNIST samples
    sf::Font font;                         // Font for button labels and neuron count labels
    std::vector<Button> buttons;           // List of interactive buttons (Add Layer, Add Neuron, Build, Train, Test)
    std::vector<sf::RectangleShape> layerRects; // Rectangles representing each layer
    std::vector<std::vector<sf::CircleShape>> neurons; // 2D vector of neuron circles (per layer)
    std::vector<std::vector<sf::Vertex>> connections; // Lines connecting neurons between layers
    std::vector<sf::Text> neuronCounts;    // Text labels showing the number of neurons in each layer
    std::vector<int> layerSizes;           // Number of neurons in each layer (for Network construction)
    Dataset& trainData;                    // Reference to the training dataset
    Dataset& testData;                     // Reference to the test dataset
    Network* network;                      // Pointer to the neural network (initialized after Build)
    double learningRate;                   // Learning rate for the network
    int epochs;                            // Number of epochs for training
    size_t selectedLayer;                  // Index of the currently selected layer (size_t to match vector sizes)
    bool isBuilt;                          // Flag to indicate if the network has been built

    // Constants for GUI layout
    const float LAYER_WIDTH = 60.0f;       // Width of each layer rectangle
    const float LAYER_HEIGHT = 400.0f;     // Height of each layer rectangle
    const float NEURON_RADIUS = 6.0f;      // Radius of neuron circles
    const float LAYER_X_START = 350.0f;    // Starting x-position for the first layer
    const float LAYER_Y = 50.0f;           // Y-position for layers
    const float LAYER_SPACING = 120.0f;    // Spacing between layers

public:
    // Constructor: Initializes the GUI with window, input display, datasets, and training parameters
    GUI(sf::RenderWindow& window, Input& inputDisplay, Dataset& trainData, Dataset& testData,
        double learningRate, int epochs);

    // Handles user input events (mouse clicks, window close)
    void handleEvents();

    // Draws the GUI elements to the window (layers, neurons, connections, buttons, input display)
    void draw();

    // Getter: Returns the network architecture (layer sizes)
    const std::vector<int>& getLayerSizes() const;

private:
    // Adds a new layer to the network (rectangle in GUI, updates layerSizes)
    void addLayer();

    // Adds a neuron to the selected layer (circle in GUI, updates layerSizes)
    void addNeuron();

    // Builds the network by creating connections between neurons and initializing the Network object
    void buildNetwork();

    // Trains the network using the training dataset, prints loss per epoch
    void trainNetwork();

    // Tests the network using the test dataset, prints accuracy
    void testNetwork();

    // Helper: Checks if a layer is clicked based on mouse position, updates selectedLayer
    void selectLayer(sf::Vector2f mousePos);

    // Helper: Draws connections between neurons in adjacent layers
    void drawConnections();
};

#endif /* GUI_hpp */
