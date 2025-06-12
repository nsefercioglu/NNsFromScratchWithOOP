//
//  GUI.cpp
//  neuralNetworks
//
//  Created by Necati Sefercioğlu
//

#include "GUI.hpp"
#include <iostream>

// Constructor: Initializes the GUI with window, input display, datasets, and training parameters
GUI::GUI(sf::RenderWindow& window, Input& inputDisplay, Dataset& trainData, Dataset& testData,
         double learningRate, int epochs)
    : window(window), inputDisplay(inputDisplay), trainData(trainData), testData(testData),
      learningRate(learningRate), epochs(epochs), selectedLayer(-1), network(nullptr), isBuilt(false) {
    // Load font for button labels and neuron counts
    if (!font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        throw std::runtime_error("Failed to load font");
    }

    // Initialize buttons (Add Layer, Add Neuron, Build, Train, Test)
    buttons.emplace_back(300.0f, 520.0f, 100.0f, 40.0f, "Add Layer", font);
    buttons.emplace_back(410.0f, 520.0f, 100.0f, 40.0f, "Add Neuron", font);
    buttons.emplace_back(520.0f, 520.0f, 100.0f, 40.0f, "Build", font);
    buttons.emplace_back(630.0f, 520.0f, 100.0f, 40.0f, "Train", font);
    buttons.emplace_back(740.0f, 520.0f, 100.0f, 40.0f, "Test", font);

    // Set button colors
    for (auto& button : buttons) {
        button.setFillColor(sf::Color::Green);
    }
}

// Handles user input events (mouse clicks, window close)
void GUI::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos = window.mapPixelToCoords(
                sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

            // Check for button clicks
            if (buttons[0].isClicked(mousePos)) { // Add Layer
                addLayer();
            }
            else if (buttons[1].isClicked(mousePos)) { // Add Neuron
                if (selectedLayer != static_cast<size_t>(-1)) {
                    addNeuron();
                }
                else {
                    std::cout << "Please select a layer first!" << std::endl;
                }
            }
            else if (buttons[2].isClicked(mousePos)) { // Build
                buildNetwork();
            }
            else if (buttons[3].isClicked(mousePos)) { // Train
                if (isBuilt) {
                    trainNetwork();
                }
                else {
                    std::cout << "Please build the network first!" << std::endl;
                }
            }
            else if (buttons[4].isClicked(mousePos)) { // Test
                if (isBuilt) {
                    testNetwork();
                }
                else {
                    std::cout << "Please build the network first!" << std::endl;
                }
            }
            else {
                // Check if a layer is clicked for selection
                selectLayer(mousePos);
            }
        }
    }
}

// Draws the GUI elements to the window
void GUI::draw() {
    window.clear(sf::Color::White);

    // Draw input display
    inputDisplay.draw(window);

    // Draw layers
    for (size_t i = 0; i < layerRects.size(); ++i) {
        // Highlight selected layer with a red outline
        if (i == selectedLayer) {
            layerRects[i].setOutlineColor(sf::Color::Red);
            layerRects[i].setOutlineThickness(2.0f);
        }
        else {
            layerRects[i].setOutlineColor(sf::Color::Black);
            layerRects[i].setOutlineThickness(1.0f);
        }
        window.draw(layerRects[i]);
    }

    // Draw neurons
    for (const auto& layerNeurons : neurons) {
        for (const auto& neuron : layerNeurons) {
            window.draw(neuron);
        }
    }

    // Draw connections if network is built
    if (isBuilt) {
        drawConnections();
    }

    // Draw neuron counts below each layer
    for (const auto& neuronCount : neuronCounts) {
        window.draw(neuronCount);
    }

    // Draw buttons
    for (const auto& button : buttons) {
        window.draw(button);
    }

    window.display();
}

// Getter: Returns the network architecture (layer sizes)
const std::vector<int>& GUI::getLayerSizes() const {
    return layerSizes;
}

// Adds a new layer to the network
void GUI::addLayer() {
    // Create a new layer rectangle
    sf::RectangleShape layerRect(sf::Vector2f(LAYER_WIDTH, LAYER_HEIGHT));
    layerRect.setPosition(LAYER_X_START + layerRects.size() * LAYER_SPACING, LAYER_Y);
    layerRect.setFillColor(sf::Color(100, 100, 255, 200)); // Semi-transparent blue
    layerRect.setOutlineColor(sf::Color::Black);
    layerRect.setOutlineThickness(1.0f);
    layerRects.push_back(layerRect);

    // Initialize an empty vector for neurons in this layer
    neurons.push_back(std::vector<sf::CircleShape>());

    // Update layer sizes (initially 0 neurons)
    layerSizes.push_back(0);

    // Create a text label for the neuron count
    sf::Text neuronCount;
    neuronCount.setFont(font);
    neuronCount.setString("0"); // Initially 0 neurons
    neuronCount.setCharacterSize(16); // Smaller font size for readability
    neuronCount.setFillColor(sf::Color::Black);
    // Position the text below the layer, centered horizontally
    float textX = LAYER_X_START + (layerRects.size() - 1) * LAYER_SPACING + (LAYER_WIDTH / 2);
    float textY = LAYER_Y + LAYER_HEIGHT + 10.0f; // 10 pixels below the layer
    neuronCount.setPosition(textX, textY);
    // Center the text horizontally
    sf::FloatRect textBounds = neuronCount.getLocalBounds();
    neuronCount.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top);
    neuronCounts.push_back(neuronCount);

    selectedLayer = layerRects.size() - 1; // Select the new layer
}

// Adds a neuron to the selected layer (circle in GUI, updates layerSizes)
void GUI::addNeuron() {
    if (selectedLayer >= neurons.size()) {
        throw std::runtime_error("Invalid selected layer");
    }

    // Add a new neuron to the selected layer
    sf::CircleShape neuron(NEURON_RADIUS);
    neuron.setFillColor(sf::Color::Green);
    neurons[selectedLayer].push_back(neuron);

    // Update the number of neurons in this layer
    layerSizes[selectedLayer]++;

    // Update the neuron count text
    neuronCounts[selectedLayer].setString(std::to_string(layerSizes[selectedLayer]));
    // Recenter the text after updating the string
    sf::FloatRect textBounds = neuronCounts[selectedLayer].getLocalBounds();
    neuronCounts[selectedLayer].setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top);

    // Recalculate positions for all neurons in the selected layer
    size_t totalNeurons = neurons[selectedLayer].size();
    float layerX = LAYER_X_START + selectedLayer * LAYER_SPACING + LAYER_WIDTH / 2 - NEURON_RADIUS;

    if (totalNeurons == 1) {
        // Place a single neuron in the center of the layer
        float yPos = LAYER_Y + LAYER_HEIGHT / 2 - NEURON_RADIUS;
        neurons[selectedLayer][0].setPosition(layerX, yPos);
    } else {
        // Define padding to ensure equal distances above top neuron and below bottom neuron
        float padding = 20.0f;
        // Distance between the centers of the topmost and bottommost neurons
        float distanceBetweenCenters = (LAYER_Y + LAYER_HEIGHT - padding - NEURON_RADIUS) - (LAYER_Y + padding + NEURON_RADIUS);
        // Spacing between neuron centers
        float spacing = distanceBetweenCenters / (totalNeurons - 1);
        for (size_t i = 0; i < totalNeurons; ++i) {
            float yPos = LAYER_Y + padding + i * spacing;
            neurons[selectedLayer][i].setPosition(layerX, yPos);
        }
    }
}

// Builds the network by creating connections between neurons and initializing the Network object
void GUI::buildNetwork() {
    if (layerSizes.empty()) {
        std::cout << "No layers to build!" << std::endl;
        return;
    }

    // Clear previous connections
    connections.clear();

    // Create connections between neurons in adjacent layers
    for (size_t i = 0; i < neurons.size() - 1; ++i) {
        std::vector<sf::Vertex> layerConnections;
        for (size_t j = 0; j < neurons[i].size(); ++j) {
            for (size_t k = 0; k < neurons[i + 1].size(); ++k) {
                sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f(neurons[i][j].getPosition().x + NEURON_RADIUS * 2,
                                            neurons[i][j].getPosition().y + NEURON_RADIUS)),
                    sf::Vertex(sf::Vector2f(neurons[i + 1][k].getPosition().x,
                                            neurons[i + 1][k].getPosition().y + NEURON_RADIUS))
                };
                line[0].color = sf::Color::Black;
                line[1].color = sf::Color::Black;
                layerConnections.push_back(line[0]);
                layerConnections.push_back(line[1]);
            }
        }
        connections.push_back(layerConnections);
    }

    // Initialize the Network with the current architecture
    std::vector<int> networkSizes = {784}; // Input layer (784 pixels)
    networkSizes.insert(networkSizes.end(), layerSizes.begin(), layerSizes.end()); // Hidden and output layers
    delete network; // Delete previous network if exists
    network = new Network(networkSizes, learningRate);
    isBuilt = true;

    std::cout << "Network built with architecture: ";
    for (int size : networkSizes) {
        std::cout << size << " ";
    }
    std::cout << std::endl;
}

// Trains the network using the training dataset, prints loss per epoch
void GUI::trainNetwork() {
    std::cout << "Starting training..." << std::endl;

    const size_t DISPLAY_UPDATE_INTERVAL = 100; // Update display every 100 samples

    for (int epoch = 0; epoch < epochs; ++epoch) {
        double totalLoss = 0.0;
        for (size_t i = 0; i < trainData.getNumSamples(); ++i) {
            // Handle SFML window events periodically to reduce overhead
            if (i % DISPLAY_UPDATE_INTERVAL == 0) {
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                        return;
                    }
                }
            }

            // Get current sample and label
            const auto& sample = trainData.getSample(i);
            int label = trainData.getLabel(i);

            // Update input display and render periodically
            if (i % DISPLAY_UPDATE_INTERVAL == 0) {
                inputDisplay.setSample(sample);
                draw();
            }

            // Train the network
            auto output = network->forward(sample);
            totalLoss += network->computeLoss(output, label);
            network->backpropagate(sample, label);
        }
        // Print average loss for the epoch
        std::cout << "Epoch " << epoch + 1 << ", Loss: " << totalLoss / trainData.getNumSamples() << std::endl;

        // Ensure the last sample of the epoch is displayed
        const auto& lastSample = trainData.getSample(trainData.getNumSamples() - 1);
        inputDisplay.setSample(lastSample);
        draw();
    }
}

// Tests the network using the test dataset, prints accuracy
void GUI::testNetwork() {
    std::cout << "Starting testing..." << std::endl;
    double accuracy = network->test(testData);
    std::cout << "Final Test Accuracy: " << accuracy * 100 << "%" << std::endl;
}

// Helper: Checks if a layer is clicked based on mouse position
void GUI::selectLayer(sf::Vector2f mousePos) {
    selectedLayer = static_cast<size_t>(-1); // Use size_t for consistency
    for (size_t i = 0; i < layerRects.size(); ++i) {
        if (layerRects[i].getGlobalBounds().contains(mousePos)) {
            selectedLayer = i;
            break;
        }
    }
}

// Helper: Draws connections between neurons in adjacent layers
void GUI::drawConnections() {
    for (const auto& layerConnections : connections) {
        window.draw(layerConnections.data(), layerConnections.size(), sf::Lines);
    }
}
