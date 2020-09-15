#pragma once
#include "Message.h"
class StartMessage : public Message
{
public:
	StartMessage(sf::RenderWindow* window = NULL, sf::Font* font = NULL, bool again = false, sf::String name = "");
	bool mainLoop();
};
