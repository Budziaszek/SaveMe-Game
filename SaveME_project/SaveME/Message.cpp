#include "Message.h"
#include <iostream>

Message::Message(sf::RenderWindow* window, sf::Font* font, sf::String s)
{
	this->window = window;
	this->font = font;

	string = sf::String("");
	text = sf::Text("", *font, 30);
	text.setPosition(20, 170);

	writingTime = sf::milliseconds(100);
	stringIterations = 0;
	waitedRounds = 0;
	roundsToWait = 8;
	status = messageStatus::writing;

	messageLenght = string.getSize();
	basicMessageLength = messageLenght;
	questionEnd = string.getSize();

	actualLineLength = 0;
	maxLineLength = 46;
	lastLineLenght = maxLineLength;

	answer = false;
	end = false;
}

void Message::backspaceReaction()
{
	if (string.getSize() > basicMessageLength)
	{
		if (string[string.getSize() - 1] == '\n')
		{
			actualLineLength = lastLineLenght;
		}
		else if (lastLineLenght + actualLineLength - 2 < maxLineLength)
		{
			std::string stringToFind = string;
			int lastNewLine = stringToFind.rfind('\n');
			string.erase(lastNewLine);
			messageLenght--;

			actualLineLength += lastLineLenght - 1;
			lastLineLenght = maxLineLength;
		}
		else
		{
			actualLineLength--;
		}
		messageLenght--;
		string.erase(string.getSize() - 1, 1);
	}
}

void Message::normalKeyReaction(int key)
{
	string += static_cast<char>(key);
	actualLineLength++;
	messageLenght++;

	if (actualLineLength > maxLineLength)
	{
		makeNewLine();
	}
}

void Message::makeNewLine()
{
	std::string stringToFind = string;
	int lastWord = stringToFind.rfind(' ');
	if (string.substring(lastWord + 1).getSize() < maxLineLength)
	{
		int wordLenght = string.substring(lastWord + 1).getSize();
		lastLineLenght = actualLineLength - wordLenght;
		actualLineLength = wordLenght;

		string.insert(lastWord + 1, "\n");
		messageLenght++;
	}
	else
	{
		string += "\n";
		messageLenght++;
		actualLineLength = 0;
	}
	
}

void Message::appendAnotherLetter()
{
	if (writingClock.getElapsedTime() >= writingTime)
	{
		//we have finished writing message
		if (stringIterations == string.getSize())
		{
			if (status == messageStatus::writing)
			{
				status = messageStatus::waiting;
			}
			else
			{
				//said no
				if (!answer)
				{
					sf::sleep(sf::Time(sf::milliseconds(800)));
					window->close();
				}
				//said yes
				else
				{
					end = true;
				}
			}
		}
		//wait to make it look naturally
		if (stringIterations > 1 && (string[stringIterations-1] == '.' || string[stringIterations-1] == '?'
			|| string[stringIterations - 1] == '!' || string[stringIterations - 1] == ','))
		{
			waitedRounds++;
			if (waitedRounds == roundsToWait)
			{
				waitedRounds = 0;
				stringIterations++;
			}
		}
		else
		{
			stringIterations++;
		}	
		writingClock.restart();
	}
}

void Message::showText()
{
	if (cursorClock.getElapsedTime().asMilliseconds() < 500)
		text.setString(string.substring(0, stringIterations) + "|");
	else if (cursorClock.getElapsedTime().asMilliseconds() < 1000)
		text.setString(string.substring(0, stringIterations));
	else
		cursorClock.restart();
}

int Message::checkAnswer()
{
	std::string smallLetters = string.substring(questionEnd, string.getSize());
	for (int i = 0; i < smallLetters.size(); ++i)
		smallLetters[i] = tolower(smallLetters[i]);

	if (smallLetters.find("yes") != std::string::npos || smallLetters.find("yep") != std::string::npos
		|| smallLetters.find("yea") != std::string::npos || smallLetters.find("sure") != std::string::npos)
	{
		return 1;
	}
	else if (smallLetters.find("no") != std::string::npos || smallLetters.find("nope") != std::string::npos
		|| smallLetters.find("never") != std::string::npos || smallLetters.find("not") != std::string::npos 
		|| smallLetters.find("nay") != std::string::npos)
	{
		return -1;
	}
	else {
		return 0;
	}
}
