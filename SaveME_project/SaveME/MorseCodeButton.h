#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <string.h>

class MorseCodeButton
	:public sf::Drawable, public sf::Transformable
{
public:
	MorseCodeButton(sf::Vector2f position = sf::Vector2f(0, 0));
	sf::FloatRect getGlobalBounds();
	sf::FloatRect getButtonGlobalBounds();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void clickReaction();
	void releaseReaction();
	bool isLighting();
	sf::String actualize();
private:
	bool light;

	sf::Vector2f position;
	sf::VertexArray bulbVertex;
	sf::Texture bulbTileset;
	int tileSize;
	int tileNumber;

	sf::String message;
	bool messageStarted;
	int* sign;
	int actualSign;
	sf::Clock clock;
	int dotTimeMiliSec;
	int tolerance;

	sf::RectangleShape line;
	sf::RectangleShape box;
	sf::CircleShape button;
	sf::CircleShape buttonClicked;
	sf::Text key;
	sf::RectangleShape keyScreen;
	sf::Font* font;

	void setAppearance(int tileNumber, int size = 150);
	void resetSign();
	void addLetter();
};