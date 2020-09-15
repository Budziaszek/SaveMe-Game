#include "StartMessage.h"

StartMessage::StartMessage(sf::RenderWindow * window, sf::Font * font, bool again, sf::String name)
	:Message(window, font)
{	
	if(!again)
		string = sf::String(L"Hello... Player. You might be disapointed, but it is NOT a game.\nSomebody needs your help. Will you give a hand?\n");
	else
		string = sf::String(L"No! You again? Last time you have killed " + name + "!\nBut I will give you one more chance. Will you give a hand?\n");
	
	messageLenght = string.getSize();
	basicMessageLength = messageLenght;
	questionEnd = string.getSize();
}

bool StartMessage::mainLoop()
{
	while (window->isOpen() && !end)
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
			}
			if (status == messageStatus::waiting)
			{
				if (event.type == sf::Event::KeyPressed)
				{
					if (event.key.code == sf::Keyboard::Return)
					{
						answer = checkAnswer();

						//we add final message
						status = messageStatus::writingResponse;
						if (answer == -1)
						{
							std::string bye = "\nI'm sorry to hear that... Bye";
							messageLenght += bye.size();
							string += bye;
						}
						else if (answer == 0)
						{
							std::string bye = "\nHmm... I assume that means no.\nI'm sorry to hear that... Bye";
							messageLenght += bye.size();
							string += bye;
						}
						else if (answer == 1)
						{
							std::string great = "\nGreat! I'll introduce you to someone";
							messageLenght += great.size();
							string += great;
						}
						writingClock.restart();
					}
				}
				else if (event.type == sf::Event::TextEntered)
				{
					if (event.text.unicode == ';')
					{
						//ignore, because font do not include;
					}
					else if (event.text.unicode == '\b')
					{
						backspaceReaction();
					}
					else if (event.text.unicode < 128)
					{
						normalKeyReaction(event.text.unicode);
					}
				}
			}
		}

		if (status == messageStatus::writing || status == messageStatus::writingResponse)
		{
			sf::sleep(sf::Time(sf::milliseconds(100)));
			appendAnotherLetter();
		}
		else
		{
			stringIterations = messageLenght;
		}
		showText();
		window->clear(sf::Color(100, 100, 100)); //grey
		window->draw(text);
		window->display();
	}
	if (answer == -1 || answer == 0)
		return false;
	else
		return true;
}

