#include "ColorPicker.h"

ColorPicker::ColorPicker(sf::Vector2f startPosition)
{
	position = startPosition;
	
	rectangle = sf::RectangleShape(sf::Vector2f(10, 260));
	pointer = sf::RectangleShape(sf::Vector2f(14, 5));
	
	rectangle.setPosition(position);
	pointer.setPosition(position.x - 2, position.y + 255);

	rectangle.setFillColor(sf::Color(50, 50, 50));
	pointer.setFillColor(sf::Color(0, 0, 0));

	minValue = 0;
	maxValue = 255;
	changeValue = 5;
	value = 0;
}

void ColorPicker::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(rectangle, states);
	target.draw(pointer, states);
}

int ColorPicker::getValue()
{
	return value;
}

void ColorPicker::actualizeValue(sf::Vector2i mousePosition)
{
	int temp = 260 - mousePosition.y - 3 - position.y;
	
	if (temp < minValue)
		temp = minValue;
	else if (temp > maxValue)
		temp = maxValue;

	value = temp;

	pointer.setPosition(rectangle.getPosition().x - 2, rectangle.getPosition().y + 255 - value);
}

sf::FloatRect ColorPicker::getGlobalBounds()
{
	sf::RectangleShape rect = sf::RectangleShape(sf::Vector2f(14, 260));
	rect.setPosition(position.x - 2, position.y);
	return rect.getGlobalBounds();
}
