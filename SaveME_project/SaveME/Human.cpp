#include "Human.h"
#include <iostream>

Human::Human(Sex givenSex, sf::String givenName, sf::Vector2u givenWindowSize)
{
	sex = givenSex;
	walkStatus = -1;
	lifeStatus = Death::ALIVE;
	speed = 5;
	numberOfWalkStatuses = 9;

	name = givenName;
	windowSize = givenWindowSize;

	maxActiveHeight = 64;
	maxActiveWidth = 64;
	basicX = -50;
	basicY = 220;

	humanVertex.setPrimitiveType(sf::Quads);
	humanVertex.resize(4);
	int i = rand() % 4;
	std::string s = "Sprites/guy" + std::to_string(i) + ".png";
	humanTileset.loadFromFile(s);
	bombTileset.loadFromFile("Sprites/bomb.png");

	tileNumber = DOWN*numberOfWalkStatuses;
	tileSize = 64;
	setAppearance(tileNumber);
}
Human::Death Human::getLifeStatus()
{
	return lifeStatus;
}
void Human::setLifeStatus(Death status)
{
	lifeStatus = status;
}
void Human::walk(Direction x, Direction y)
{
	walkStatus++;
	if (walkStatus > numberOfWalkStatuses - 1)
		walkStatus = 1;

	if (x == NONE && y == NONE)
	{
		tileNumber = DOWN;
	}
	else if (x == RIGHT && (basicX + speed + tileSize) < (int)windowSize.x)
	{
		tileNumber = RIGHT*numberOfWalkStatuses + walkStatus;
		basicX += speed;
	}
	else if (x == LEFT && (basicX - speed + tileSize/4) > 0)
	{
		tileNumber = LEFT*numberOfWalkStatuses + walkStatus;
		basicX -= speed;
	}
	else if (y == DOWN && (basicY + speed + tileSize) < (int)windowSize.y)
	{
		tileNumber = DOWN*numberOfWalkStatuses + walkStatus;
		basicY += speed;
	}
	else if (y == UP && (basicY - speed + tileSize / 4) > 0)
	{
		tileNumber = UP*numberOfWalkStatuses + walkStatus;
		basicY -= speed;	
	}
	setAppearance(tileNumber);
}

void Human::stand(Direction d)
{
	setAppearance(d*numberOfWalkStatuses);
}

void Human::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &humanTileset;
	target.draw(humanVertex, states);
}

sf::Vector2f Human::getPosition()
{
	return sf::Vector2f(basicX, basicY);
}

void Human::setPosition(int x, int y)
{
	basicX = x;
	basicY = y;
}

sf::String Human::getName()
{
	return name;
}

sf::FloatRect Human::getGlobalBounds()
{
	return humanVertex.getBounds();
}

sf::FloatRect Human::getFeetGlobalBounds()
{
	sf::VertexArray temp = humanVertex;
	sf::Vertex* quad = &temp[0];

	quad[0].position = sf::Vector2f(basicX + 18, basicY + 64 - 12);
	quad[1].position = sf::Vector2f(basicX + 44, basicY + 64 - 12);
	quad[2].position = sf::Vector2f(basicX + 44, basicY + 64 - 2);
	quad[3].position = sf::Vector2f(basicX + 18, basicY + 64 - 2);

	return temp.getBounds();
}

void Human::killByBomb()
{
	static int killStatus = 0;
	sf::Vertex* quad = &humanVertex[0];
	humanTileset = bombTileset;
	if(killStatus<24)
		killStatus++;
	setAppearance(killStatus, 230);
}

void Human::walkLeft()
{
	walkStatus++;
	if (walkStatus > numberOfWalkStatuses - 1)
		walkStatus = 1;

	tileNumber = LEFT * numberOfWalkStatuses + walkStatus;
	basicX -= speed;

	setAppearance(tileNumber);
}

void Human::setWindowSize(sf::Vector2u size)
{
	windowSize = size;
}

void Human::setAppearance(int tileNumber, int size)
{
	sf::Vertex* quad = &humanVertex[0];

	int n = tileNumber % (humanTileset.getSize().x / size);
	int m = tileNumber / (humanTileset.getSize().x / size);

	quad[0].texCoords = sf::Vector2f(n * size, m * size);
	quad[1].texCoords = sf::Vector2f((n + 1) * size, m * size);
	quad[2].texCoords = sf::Vector2f((n + 1) * size, (m + 1) * size);
	quad[3].texCoords = sf::Vector2f(n * size, (m + 1) * size);

	quad[0].position = sf::Vector2f(basicX, basicY);
	quad[1].position = sf::Vector2f(basicX + size, basicY);
	quad[2].position = sf::Vector2f(basicX + size, basicY + size);
	quad[3].position = sf::Vector2f(basicX, basicY + size);
}

