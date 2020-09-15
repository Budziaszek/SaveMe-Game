#include "Tree.h"

Tree::Tree(sf::Vector2f givenPosition)
{
	treeVertex.setPrimitiveType(sf::Quads);
	treeVertex.resize(4);
	treeTileset.loadFromFile("Sprites/tree.png");
	branchesTileset.loadFromFile("Sprites/tree_branches.png");

	position = givenPosition;

	tileNumber = 1;
	tileSize = 150;
	setAppearance();
	shakeTime = 70;
}

void Tree::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	static bool onlyBranches = false;
	if (onlyBranches)
	{
		states.transform *= getTransform();
		states.texture = &branchesTileset;
		target.draw(treeVertex, states);
	}
	else
	{
		states.transform *= getTransform();
		states.texture = &treeTileset;
		target.draw(treeVertex, states);
	}
	onlyBranches = !onlyBranches;

}

sf::FloatRect Tree::getGlobalBounds()
{
	return treeVertex.getBounds();
}

sf::Vector2f Tree::getPosition()
{
	return position;
}

bool Tree::shake(int howManyTimes)
{
	static int counter = 0;
	if (clock.getElapsedTime().asMilliseconds() > (unsigned int) shakeTime)
	{
		
		if (tileNumber == 2)
			tileNumber = 0;
		else if (tileNumber == 0 || tileNumber == 1)
			tileNumber = 2;

		counter++;
		clock.restart();
		setAppearance();
	}
	else if(counter == howManyTimes)
	{
		stopShakeing();
		counter = 0;
		return false;
	}
	return true;
}

void Tree::stopShakeing()
{
	tileNumber = 1;
	setAppearance();
}

void Tree::setAppearance(int size)
{
	sf::Vertex* quad = &treeVertex[0];

	int n = tileNumber % (treeTileset.getSize().x / size);
	int m = tileNumber / (treeTileset.getSize().x / size);

	quad[0].texCoords = sf::Vector2f(n * size, m * size);
	quad[1].texCoords = sf::Vector2f((n + 1) * size, m * size);
	quad[2].texCoords = sf::Vector2f((n + 1) * size, (m + 1) * size);
	quad[3].texCoords = sf::Vector2f(n * size, (m + 1) * size);

	quad[0].position = sf::Vector2f(position.x, position.y);
	quad[1].position = sf::Vector2f(position.x + size, position.y);
	quad[2].position = sf::Vector2f(position.x + size, position.y + size);
	quad[3].position = sf::Vector2f(position.x, position.y + size);
}
