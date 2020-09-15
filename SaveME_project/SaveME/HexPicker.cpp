#include "HexPicker.h"

void HexPicker::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(background, states);
	for (int i = 0; i <= digits; i++)
	{
		target.draw(letters[i], states);
	}
	
}

HexPicker::HexPicker(int digits, sf::Vector2f position)
{
	this->digits = digits;
	this->position = position;
	font = new sf::Font();
	font->loadFromFile("Fonts/Bodoni.ttf");

	letters = new sf::Text[digits + 1];

	letters[0] = sf::Text("#", *font, 30);
	letters[0].setPosition(position.x, position.y);
	
	for (int i = 1; i <= digits; i++)
	{
		letters[i] = sf::Text(randHex(), *font, 30);
		letters[i].setPosition(position.x + 20 * i, position.y);
	}

	background = sf::RectangleShape(sf::Vector2f((digits+1) * 20 + 20, 45));
	background.setPosition(position.x - 10, position.y - 5);
	background.setFillColor(sf::Color(70, 70, 70));
}

sf::FloatRect HexPicker::getGlobalBounds()
{
	return background.getGlobalBounds();
}

bool HexPicker::checkAndActualize(sf::Vector2i mousePosition)
{
	for (int i = 1; i <= digits; i++)
	{
		if (letters[i].getGlobalBounds().contains(mousePosition.x, mousePosition.y))
		{
			if (letters[i].getString() == "0")
				letters[i].setString("1");
			else if (letters[i].getString() == "1")
				letters[i].setString("2");
			else if (letters[i].getString() == "2")
				letters[i].setString("3");
			else if (letters[i].getString() == "3")
				letters[i].setString("4");
			else if (letters[i].getString() == "4")
				letters[i].setString("5");
			else if (letters[i].getString() == "5")
				letters[i].setString("6");
			else if (letters[i].getString() == "6")
				letters[i].setString("7");
			else if (letters[i].getString() == "7")
				letters[i].setString("8");
			else if (letters[i].getString() == "8")
				letters[i].setString("9");
			else if (letters[i].getString() == "9")
				letters[i].setString("A");
			else if (letters[i].getString() == "A")
				letters[i].setString("B");
			else if (letters[i].getString() == "B")
				letters[i].setString("C");
			else if (letters[i].getString() == "C")
				letters[i].setString("D");
			else if (letters[i].getString() == "D")
				letters[i].setString("E");
			else if (letters[i].getString() == "E")
				letters[i].setString("F");
			else if (letters[i].getString() == "F")
				letters[i].setString("0");

			return true;
		}
	}
	return false;
}

sf::Color HexPicker::getColor()
{
	int r;
	int g;
	int b;

	if (digits == 3)
	{
		r = 17 * convertToDec(1);
		g = 17 * convertToDec(2);
		b = 17 * convertToDec(3);
	}
	else if (digits == 6)
	{
		r = 16 * convertToDec(1) + convertToDec(2);
		g = 16 * convertToDec(3) + convertToDec(4);
		b = 16 * convertToDec(5) + convertToDec(6);
	}
	
	return sf::Color(r, g, b);
}

int HexPicker::convertToDec(int i)
{
	if(letters[i].getString() == "0")
		return 0;
	if (letters[i].getString() == "1")
		return 1;
	if (letters[i].getString() == "2")
		return 2;
	if (letters[i].getString() == "3")
		return 3;
	if (letters[i].getString() == "4")
		return 4;
	if (letters[i].getString() == "5")
		return 5;
	if (letters[i].getString() == "6")
		return 6;
	if (letters[i].getString() == "7")
		return 7;
	if (letters[i].getString() == "8")
		return 8;
	if (letters[i].getString() == "9")
		return 9;
	if (letters[i].getString() == "A")
		return 10;
	if (letters[i].getString() == "B")
		return 11;
	if (letters[i].getString() == "C")
		return 12;
	if (letters[i].getString() == "D")
		return 13;
	if (letters[i].getString() == "E")
		return 14;
	if (letters[i].getString() == "F")
		return 15;
}

sf::String HexPicker::randHex()
{
	int i = rand() % 15;
	if (i == 0)
		return "0";
	if (i == 1)
		return "1";
	if (i == 2)
		return "2";
	if (i == 3)
		return "3";
	if (i == 4)
		return "4";
	if (i == 5)
		return "5";
	if (i == 6)
		return "6";
	if (i == 7)
		return "7";
	if (i == 8)
		return "8";
	if (i == 9)
		return "9";
	if (i == 10)
		return "A";
	if (i == 11)
		return "B";
	if (i == 12)
		return "C";
	if (i == 13)
		return "D";
	if (i == 14)
		return "E";
	if (i == 15)
		return "F";
}
