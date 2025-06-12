//
//  Button.hpp
//  neuralNetworks
//
//  Created by Necati Sefercioğlu
//

#ifndef Button_hpp
#define Button_hpp

#include <SFML/Graphics.hpp>
#include <string>

// Class representing a clickable button with text label
class Button : public sf::Drawable {
public:
    // Constructor: Initializes button with position, size, label, and font
    Button(float x, float y, float width, float height, const std::string& label, sf::Font& font);

    // Checks if the button is clicked based on mouse position
    bool isClicked(sf::Vector2f mousePos) const;

    // Sets the fill color of the button's shape
    void setFillColor(const sf::Color& color);

private:
    // Draws the button and its text to the render target
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::RectangleShape shape; // Shape of the button
    sf::Text text;            // Text label of the button
};

#endif /* Button_hpp */
