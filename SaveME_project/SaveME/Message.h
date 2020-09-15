#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

enum messageStatus
{
	writing,
	waiting,
	writingResponse
};

class Message
{
protected:
	//window realization
	sf::RenderWindow* window;
	sf::Font* font;
	sf::Text text;
	bool end;

	//message content management
	sf::String string;
	messageStatus status;
	int stringIterations;	
	int basicMessageLength;
	int messageLenght;

	//message lines management
	int actualLineLength;
	int maxLineLength;
	int lastLineLenght;

	//writing management
	sf::Clock writingClock;
	sf::Clock cursorClock;
	sf::Time elapsed1;
	sf::Time writingTime;
	int roundsToWait;
	int waitedRounds;
	int answer;
	int questionEnd;
	
	virtual void backspaceReaction();
	void normalKeyReaction(int key);
	void makeNewLine();
	void appendAnotherLetter();
	void showText();

	virtual int checkAnswer();

public:
	Message(sf::RenderWindow* window, sf::Font* font, sf::String s = "");
	virtual bool mainLoop() = 0;
	
};
