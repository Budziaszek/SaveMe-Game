#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class ColorPicker
	:public sf::Drawable
{
public:
	ColorPicker(sf::Vector2f startPosition = sf::Vector2f(0, 0));
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	int getValue();
	void actualizeValue(sf::Vector2i mousePosition);
	sf::FloatRect getGlobalBounds();
private:
	sf::RectangleShape rectangle;
	sf::RectangleShape pointer;
	sf::Vector2f position;
	int value;
	int maxValue;
	int minValue;
	int changeValue;
};