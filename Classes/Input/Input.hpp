//
//  Input.hpp
//  neuralNetworks
//
//  Created by Necati Sefercioğlu
//

#ifndef Input_hpp
#define Input_hpp

#include <SFML/Graphics.hpp>
#include <vector>

// Class representing the input display for MNIST samples
class Input {
private:
    std::vector<sf::RectangleShape> pixelRects; // Rectangles for scaled pixel display
    std::vector<double> currentSample;          // Pixel values of the current sample

public:
    // Constructor: Initializes the display rectangles
    Input();

    // Updates the display with a new sample
    // Parameters:
    //   sample: Vector of 784 normalized pixel values
    // Throws: std::invalid_argument if sample size is incorrect
    void setSample(const std::vector<double>& sample);

    // Draws the input display to the window
    // Parameters:
    //   window: SFML window to render the display
    void draw(sf::RenderWindow& window);
};

#endif /* Input_hpp */
