#pragma once
#include<SFML\Graphics.hpp>

class HexPicker
	:public sf::Drawable, public sf::Transformable
{
private:
	sf::Vector2f position;
	int digits;
	sf::Font* font;
	sf::RectangleShape background;
public:
	sf::Text* letters;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	HexPicker(int digits = 3, sf::Vector2f position = sf::Vector2f());
	sf::FloatRect getGlobalBounds();
	bool checkAndActualize(sf::Vector2i mousePosition);
	sf::Color getColor();
	int convertToDec(int i);
	sf::String randHex();
};