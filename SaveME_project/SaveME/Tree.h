#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>

class Tree
	:public sf::Drawable, public sf::Transformable
{
public:
	Tree(sf::Vector2f givenPosition = sf::Vector2f(0, 0));
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::FloatRect getGlobalBounds();
	sf::Vector2f getPosition();
	bool shake(int howManyTimes = 6);
	void stopShakeing();
private:
	sf::Vector2f position;
	sf::VertexArray treeVertex;
	sf::Texture treeTileset;
	sf::Texture branchesTileset;
	sf::Clock clock;
	int shakeTime;
	int tileSize;
	int tileNumber;
	void setAppearance(int size = 150);
};