#include "Balloon.h"

Balloon::Balloon(int xPosition, int maxYPosition, bool green)
{	
	if (green)
	{
		balloonTileset.loadFromFile("Sprites/balloonGreen.png");
		yPosition = maxYPosition + 192;
	}
	else
	{
		balloonTileset.loadFromFile("Sprites/balloonRed.png"); 
		yPosition = 400;
	}
	balloonVertex.setPrimitiveType(sf::Quads);
	balloonVertex.resize(4);

	this->xPosition = xPosition;
	this->maxYPosition = maxYPosition;

	tileNumber = 0;
	tileSize = 64;
	move = 0;
	right = true;
	flyTime = 30;
	breakTime = 50;
	setAppearance();
	breaking = false;
}

void Balloon::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &balloonTileset;
	target.draw(balloonVertex, states);
}

sf::FloatRect Balloon::getGlobalBounds()
{
	return balloonVertex.getBounds();
}

void Balloon::breakTheBalloon()
{
	breaking = true;
}

void Balloon::setAppearance(int tileNumber, int size)
{
	sf::Vertex* quad = &balloonVertex[0];

	int n = tileNumber % (balloonTileset.getSize().x / size);
	int m = tileNumber / (balloonTileset.getSize().x / size);

	quad[0].texCoords = sf::Vector2f(n * size, m * size);
	quad[1].texCoords = sf::Vector2f((n + 1) * size, m * size);
	quad[2].texCoords = sf::Vector2f((n + 1) * size, (m + 1) * size);
	quad[3].texCoords = sf::Vector2f(n * size, (m + 1) * size);

	quad[0].position = sf::Vector2f(xPosition, yPosition);
	quad[1].position = sf::Vector2f(xPosition + size, yPosition);
	quad[2].position = sf::Vector2f(xPosition + size, yPosition + size);
	quad[3].position = sf::Vector2f(xPosition, yPosition + size);
}

void Balloon::fly()
{
	
	if (clock.getElapsedTime().asMilliseconds() >= flyTime && !breaking)
	{
		clock.restart();

		yPosition -= 5;
		if (yPosition < -192)
			yPosition = maxYPosition + 192;

		if (right && move < 15)
		{
			xPosition += 1;
			move++;
		}
		if (!right && move < 15)
		{
			xPosition -= 1;
			move++;
		}
		if (move == 15)
		{
			right = !right;
			move = 0;
		}

		setAppearance();
	}
	else if(clock.getElapsedTime().asMilliseconds() >= breakTime && breaking)
	{
		clock.restart();
		tileNumber++;
		setAppearance(tileNumber);	

		if (tileNumber == 3)
		{
			tileNumber = 0;
			breaking = false;
			yPosition = maxYPosition + 192;
		}
	}
}

sf::Vector2f Balloon::getPosition()
{
	return sf::Vector2f(xPosition + 32, yPosition + 32);
}
