#pragma once
#include <SFML\Graphics.hpp>
#include "Human.h"
#include "Message.h"
#include "ColorPicker.h"
#include <cstdlib>
#include <ctime>
#include "Tree.h"
#include "Balloon.h"
#include <iostream>
#include "HexPicker.h"
#include <fstream>
#include "MorseCodeButton.h"
#include <iostream>
#include <cmath>

class Game
	:public Message
{
public:
	Game(sf::RenderWindow* window, sf::Font* font, sf::String name);
	bool mainLoop();

	
private:
	typedef enum {
		G_Choose, G_Point, G_Click, G_IntensivityRGB, G_RGB, G_TreeClick, G_TreeMove, G_Hex3, G_Hex6, G_Write, G_Etirw,
		G_Balloon, G_Morse, G_Create, G_HappyEnd, G_Died} GameStatus;

	//0 ONLY CHOOSE
	//1 POINT
	//2 CLICK
	//3 PICK GREEN INTENSIVITY
	//4 PICK CORRECT COLOR
	//5 SHAKE THE TREE BY CLICK
	//6 SHAKE THE WINDOW
	//7 HEX TRIPLET (tree digit)
	//8 HEX TRIPLET (six digit)
	//9 WRITE THE WORD
	//10 POP BALLOONS
	//11 WRITE GREEN IN MORSE
	//12 CONNECT YELLOW AND BLUE
	//13 d³ugoœæ fali elektromagnetycznej 487–570 nm

	sf::RenderWindow* window;
	sf::Font* font;

	sf::Clock movingClock;
	sf::Time moveTime;
	Human human;
	sf::String basicString;

	GameStatus gameStatus;

	bool block;
	int counter;
	
	bool drawCircle;
	sf::CircleShape* circle;
	sf::CircleShape* circle2;

	sf::CircleShape* mouseHas;
	
	
	Tree tree;
	bool shake;

	ColorPicker pickerR;
	ColorPicker pickerG;
	ColorPicker pickerB;

	HexPicker hexPicker3;
	HexPicker hexPicker6;

	Balloon balloonRed;
	Balloon balloonGreen;
	int chance;

	MorseCodeButton morseCode;

	void moveHuman(sf::Keyboard::Key key);
	void stopHuman(sf::Keyboard::Key key);
	void drawEverything();

	sf::RectangleShape background;
	sf::Texture backgroundTexture;

	int windowXSize;
	int windowYSize;

	void circlesRequest();
	void redCircleReaction();
	void greenCircleReaction();
	void randCirclePosition(bool basicCircle = true);
	void nextLevel();
	int checkAnswer() override;
	int checkAnswer2();
	void createPrice();

	void finalMessage();
};