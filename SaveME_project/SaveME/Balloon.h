#pragma once
#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>

class Balloon
	:public sf::Drawable, public sf::Transformable
{
public:
	Balloon(int xPosition = 0, int maxYPosition = 0, bool green = true);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::FloatRect getGlobalBounds();
	void breakTheBalloon();
	void fly();
	sf::Vector2f getPosition();
private:
	int xPosition;
	int yPosition;
	int maxYPosition;
	sf::VertexArray balloonVertex;
	sf::Texture balloonTileset;

	int move;
	bool right;
	bool breaking;

	sf::Clock clock;
	int flyTime;
	int breakTime;
	int tileSize;
	int tileNumber;
	void setAppearance(int tileNumber = 0, int size = 64);
};
