#include "MorseCodeButton.h"

MorseCodeButton::MorseCodeButton(sf::Vector2f position)
{
	bulbTileset.loadFromFile("Sprites/lightbulb.png");
	this->position = position;
	light = true;
	bulbVertex.setPrimitiveType(sf::Quads);
	bulbVertex.resize(4);

	tileNumber = 0;
	tileSize = 150;
	setAppearance(tileNumber);

	line = sf::RectangleShape(sf::Vector2f(10, 70));
	line.setPosition(position.x + 71, position.y + 120);
	line.setFillColor(sf::Color(50, 50, 50));

	box = sf::RectangleShape(sf::Vector2f(150, 50));
	box.setPosition(position.x + 2, position.y + 150);
	box.setFillColor(sf::Color(40, 40, 40));	
	
	button = sf::CircleShape(15);
	button.setFillColor(sf::Color(80, 80, 80));
	button.setPosition(position.x + 62, position.y + 160);

	buttonClicked = sf::CircleShape(10);
	buttonClicked.setFillColor(sf::Color(60, 60, 60));
	buttonClicked.setPosition(position.x + 67, position.y + 165);

	keyScreen = sf::RectangleShape(sf::Vector2f(20, 15));
	keyScreen.setFillColor(sf::Color(0, 0, 0));
	keyScreen.setPosition(position.x + 130, position.y + 185);

	font = new sf::Font();
	font->loadFromFile("Fonts/Bodoni.ttf");
	key.setFont(*font);
	key.setCharacterSize(12);
	key.setPosition(position.x + 110, position.y + 155);

	message = "";
	sign = new int[4];
	resetSign();
	actualSign = 0;
	messageStarted = false;

	dotTimeMiliSec = 1000;
	tolerance = 400;
}

sf::FloatRect MorseCodeButton::getGlobalBounds()
{
	return sf::FloatRect(position.x, position.y, 150, 300);
}

sf::FloatRect MorseCodeButton::getButtonGlobalBounds()
{
	return button.getGlobalBounds();
}

void MorseCodeButton::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(line, states);
	target.draw(box, states);
	target.draw(button, states);
	if (light)
		target.draw(buttonClicked, states);
	target.draw(keyScreen, states);
	target.draw(key, states);

	states.transform *= getTransform();
	states.texture = &bulbTileset;
	target.draw(bulbVertex, states);
}

void MorseCodeButton::clickReaction()
{
	light = true;
	setAppearance(1);

	if (messageStarted)
	{
		int time = clock.getElapsedTime().asMilliseconds();
		if (time >= dotTimeMiliSec - tolerance && time <= dotTimeMiliSec + 2*tolerance)
		{
			actualSign++;
		}
		else //if (time >= 3 * (dotTimeMiliSec - tolerance) && time <= 3 * (dotTimeMiliSec + tolerance))
		{
			addLetter();
			resetSign();
			actualSign = 0;
		}
		/*else
		{
			std::cout << "koniec"<<std::endl;
			message = "";
			resetSign();
			actualSign = 0;
			messageStarted = false;
		}*/
	}
	else
		messageStarted = true;
	clock.restart();
}

void MorseCodeButton::releaseReaction()
{
	light = false;
	setAppearance(0);

	if (actualSign == 4)
	{
		addLetter();
		resetSign();
		actualSign = 0;
		messageStarted = false;
	}

	int time = clock.getElapsedTime().asMilliseconds();
	if (time >= dotTimeMiliSec - tolerance && time <= dotTimeMiliSec + 2*tolerance)
	{
		sign[actualSign] = 1;
	}
	else if (time >= 3*(dotTimeMiliSec - 2*tolerance) && time <= 3*(dotTimeMiliSec + tolerance))
	{
		sign[actualSign] = 2;
	}
	else
	{
		message = "";
		resetSign();
		actualSign = 0;
		messageStarted = false;
	}
	clock.restart();
}

bool MorseCodeButton::isLighting()
{
	return light;
}

sf::String  MorseCodeButton::actualize()
{
	if (clock.getElapsedTime().asSeconds() > 9)
	{
		message = "";
		messageStarted = false;
		resetSign();
		actualSign = 0;
		clock.restart();
	}
	else if (clock.getElapsedTime().asMilliseconds() >= 3 * (dotTimeMiliSec + tolerance))
	{
		addLetter();
		resetSign();
		actualSign = 0;
	}
	std::string signString;
	for (int i = 0; i < 4; i++)
	{
		if (sign[i] == 1)
			signString += ".";
		if (sign[i] == 2)
			signString += "-";
	}

	int time = clock.getElapsedTime().asSeconds();
	std::string s = std::to_string(time);
	key.setString(message+"\n" + signString + "\n         " + s);
	return message;
}

void MorseCodeButton::setAppearance(int tileNumber, int size)
{
	sf::Vertex* quad = &bulbVertex[0];

	int n = tileNumber % (bulbTileset.getSize().x / size);
	int m = tileNumber / (bulbTileset.getSize().x / size);

	quad[0].texCoords = sf::Vector2f(n * size, m * size);
	quad[1].texCoords = sf::Vector2f((n + 1) * size, m * size);
	quad[2].texCoords = sf::Vector2f((n + 1) * size, (m + 1) * size);
	quad[3].texCoords = sf::Vector2f(n * size, (m + 1) * size);

	quad[0].position = sf::Vector2f(position.x, position.y);
	quad[1].position = sf::Vector2f(position.x + size, position.y);
	quad[2].position = sf::Vector2f(position.x + size, position.y + size);
	quad[3].position = sf::Vector2f(position.x, position.y + size);
}

void MorseCodeButton::resetSign()
{
	for (int i = 0; i < 4; i++)
		sign[i] = -1;
}

void MorseCodeButton::addLetter()
{
	//1 - dot
	//2 - mark
	if (sign[0] == 1 && sign[1] == 2 && sign[2] == -1 && sign[3] == -1)
		message += "A";
	else if (sign[0] == 2 && sign[1] == 1 && sign[2] == 1 && sign[3] == 1)
		message += "B";
	else if (sign[0] == 2 && sign[1] == 1 && sign[2] == 2 && sign[3] == 1)
		message += "C";
	else if (sign[0] == 2 && sign[1] == 1 && sign[2] == 1 && sign[3] == -1)
		message += "D";
	else if (sign[0] == 1 && sign[1] == -1 && sign[2] == -1 && sign[3] == -1)
		message += "E";
	else if (sign[0] == 1 && sign[1] == 1 && sign[2] == 1 && sign[3] == 1)
		message += "F";
	else if (sign[0] == 2 && sign[1] == 2 && sign[2] == 1 && sign[3] == -1)
		message += "G";
	else if (sign[0] == 1 && sign[1] == 1 && sign[2] == 1 && sign[3] == 1)
		message += "H";
	else if (sign[0] == 1 && sign[1] == 1 && sign[2] == -1 && sign[3] == -1)
		message += "I";
	else if (sign[0] == 1 && sign[1] == 2 && sign[2] == 2 && sign[3] == 2)
		message += "J";
	else if (sign[0] == 2 && sign[1] == 1 && sign[2] == 2 && sign[3] == -1)
		message += "K";
	else if (sign[0] == 1 && sign[1] == 2 && sign[2] == 1 && sign[3] == 1)
		message += "L";
	else if (sign[0] == 2 && sign[1] == 2 && sign[2] == -1 && sign[3] == -1)
		message += "M";
	else if (sign[0] == 2 && sign[1] == 1 && sign[2] == -1 && sign[3] == -1)
		message += "N";
	else if (sign[0] == 2 && sign[1] == 2 && sign[2] == 2 && sign[3] == -1)
		message += "O";
	else if (sign[0] == 1 && sign[1] == 2 && sign[2] == 2 && sign[3] == 1)
		message += "P";
	else if (sign[0] == 2 && sign[1] == 2 && sign[2] == 1 && sign[3] == 2)
		message += "Q";
	else if (sign[0] == 1 && sign[1] == 2 && sign[2] == 1 && sign[3] == -1)
		message += "R";
	else if (sign[0] == 1 && sign[1] == 1 && sign[2] == 1 && sign[3] == -1)
		message += "S";
	else if (sign[0] == 2 && sign[1] == -1 && sign[2] == -1 && sign[3] == -1)
		message += "T";
	else if (sign[0] == 1 && sign[1] == 1 && sign[2] == 2 && sign[3] == -1)
		message += "U";
	else if (sign[0] == 1 && sign[1] == 1 && sign[2] == 1 && sign[3] == 2)
		message += "V";
	else if (sign[0] == 1 && sign[1] == 2 && sign[2] == 2 && sign[3] == -1)
		message += "W";
	else if (sign[0] == 2 && sign[1] == 1 && sign[2] == 1 && sign[3] == 2)
		message += "X";
	else if (sign[0] == 2 && sign[1] == 1 && sign[2] == 2 && sign[3] == 2)
		message += "Y";
	else if (sign[0] == 2 && sign[1] == 2 && sign[2] == 1 && sign[3] == 1)
		message += "Z";
}
