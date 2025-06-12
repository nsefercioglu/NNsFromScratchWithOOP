//
//  Input.cpp
//  neuralNetworks
//
//  Created by Necati Sefercioğlu
//

#include "input.hpp"
#include <stdexcept>

// Constructor: Initializes the display rectangles
Input::Input() {
    // Create 784 rectangles for a 280x280 display at position (10, 50)
    pixelRects.resize(784);
    for (int i = 0; i < 28; ++i) {
        for (int j = 0; j < 28; ++j) {
            int index = i * 28 + j;
            pixelRects[index].setSize(sf::Vector2f(10.f, 10.f));
            pixelRects[index].setPosition(10.f + static_cast<float>(j * 10), 50.f + static_cast<float>(i * 10));
            pixelRects[index].setFillColor(sf::Color::White);
        }
    }
    // Initialize sample with zeros
    currentSample.resize(784, 0.0);
}

// Updates the display with a new sample
void Input::setSample(const std::vector<double>& sample) {
    // Validate sample size
    if (sample.size() != 784) {
        throw std::invalid_argument("Sample must have 784 pixel values");
    }
    currentSample = sample;
    // Update rectangle colors: invert mapping for correct display
    for (int i = 0; i < 784; ++i) {
        uint8_t colorValue = static_cast<uint8_t>((1.0 - currentSample[i]) * 255);
        pixelRects[i].setFillColor(sf::Color(colorValue, colorValue, colorValue));
    }
}

// Draws the input display to the window
void Input::draw(sf::RenderWindow& window) {
    // Draw black border around the 280x280 display
    sf::RectangleShape border(sf::Vector2f(280.f, 280.f));
    border.setPosition(10.f, 50.f);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color::Black);
    border.setOutlineThickness(2.f);
    window.draw(border);
    // Render each rectangle
    for (const auto& rect : pixelRects) {
        window.draw(rect);
    }
}
