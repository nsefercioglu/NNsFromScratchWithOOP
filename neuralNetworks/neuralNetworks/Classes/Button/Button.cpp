//
//  Button.cpp
//  neuralNetworks
//
//  Created by Necati Sefercioğlu
//

#include "Button.hpp"

// Constructor: Initializes button with position, size, label, and font
Button::Button(float x, float y, float width, float height, const std::string& label, sf::Font& font)
    : shape(sf::Vector2f(width, height)) {
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::White); // Default color
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(1.0f);

    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::Black);

    // Center the text within the button
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    text.setPosition(x + width / 2.0f, y + height / 2.0f);
}

// Checks if the button is clicked based on mouse position
bool Button::isClicked(sf::Vector2f mousePos) const {
    return shape.getGlobalBounds().contains(mousePos);
}

// Sets the fill color of the button's shape
void Button::setFillColor(const sf::Color& color) {
    shape.setFillColor(color);
}

// Draws the button and its text to the render target
void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(shape, states);
    target.draw(text, states);
}
