#pragma once
#include "StartMessage.h"
#include "Game.h"
#include <fstream>
#include <string>

class Manager
{
public:
	Manager();
private:
	sf::RenderWindow* window;
	sf::Font* bigFont;
	int begin();
};
