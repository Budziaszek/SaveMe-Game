#include "Game.h"

Game::Game(sf::RenderWindow * window, sf::Font * font, sf::String name)
	:Message(window, font)
{
	writingClock.restart();
	moveTime = sf::milliseconds(80);
	human = Human(Human::Sex::MALE, name, window->getSize());
	this->window = window;
	this->font = font;
	string = sf::String(L"Okey. This is " + human.getName() + ".\n\t\tHe's stuck here, in this window.\n\t\tHe believes you can save him, so he will obey your orders...");
	basicString = string;
	status = writing;
	actualLineLength = 0;
	text.setPosition(150, 200);
	text.setCharacterSize(20);
	srand(time(NULL));

	pickerR = ColorPicker(sf::Vector2f(10, 0));
	pickerG = ColorPicker(sf::Vector2f(30, 0));
	pickerB = ColorPicker(sf::Vector2f(50, 0));

	pickerR.actualizeValue(sf::Vector2i(0, 255 - rand() % 255));
	pickerG.actualizeValue(sf::Vector2i(0, 255 - rand() % 50));
	pickerB.actualizeValue(sf::Vector2i(0, 255 - rand() % 255));

	morseCode = MorseCodeButton(sf::Vector2f(450, 400));

	maxLineLength = 50;
	lastLineLenght = maxLineLength;
	block = true;
	drawCircle = false;
	gameStatus = G_Choose;
	shake = false;
	counter = 25;
	chance = 35;

	tree = Tree(sf::Vector2f(300, 270));

	windowXSize = sf::VideoMode::getDesktopMode().width;
	windowYSize = sf::VideoMode::getDesktopMode().height;

	balloonRed = Balloon(100, windowYSize, false);
	balloonGreen = Balloon(100, windowYSize);

	background = sf::RectangleShape(sf::Vector2f(windowXSize, windowYSize));
	background.setPosition(0, 0);

	backgroundTexture.loadFromFile("Sprites/free.jpg");
	background.setTexture(&backgroundTexture);

	hexPicker3 = HexPicker(3, sf::Vector2f(630, 200));
	hexPicker6 = HexPicker(6, sf::Vector2f(630, 245));
}

bool Game::mainLoop()
{
	sf::Vector2i windowPosition = window->getPosition();

	while (window->isOpen())
	{
		sf::Event event;
		window->clear(sf::Color(100, 100, 100)); //grey
		sf::Vector2i windowNewPosition = window->getPosition();

		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
			}
			if (event.type == sf::Event::TextEntered && status == waiting)
			{
				if (event.text.unicode == ';')
				{
					//ignore, because font do not include;
				}
				else if (event.text.unicode == '\b')
				{
					backspaceReaction();
				}
				else if (event.text.unicode < 128 && string.getSize() - basicString.getSize() < 20)
				{
					normalKeyReaction(event.text.unicode);
				}
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
					window->close();
				else if (event.key.code == sf::Keyboard::Return && status == waiting)
				{
					if (gameStatus == G_Write)
					{
						answer = checkAnswer();
						if (answer == 1)
						{
							circle->setFillColor(sf::Color(25, 165, 20));
						}
					}
					if (gameStatus == G_Etirw)
					{
						answer = checkAnswer2();
						if (answer == 1)
						{
							circle->setFillColor(sf::Color(25, 165, 20));
						}
					}
					string = basicString;
				}
				else if (!block)
				{
					moveHuman(event.key.code);
					if (circle->getGlobalBounds().intersects(human.getFeetGlobalBounds()))
					{
						if (circle->getFillColor() == sf::Color(25, 165, 20))
						{
							greenCircleReaction();
						}
						else if (gameStatus == G_IntensivityRGB && 165 - circle->getFillColor().g > -30 && 165 - circle->getFillColor().g < 30)
						{
							greenCircleReaction();
						}
						else if ((gameStatus == G_RGB || gameStatus == G_Hex3 || gameStatus == G_Hex6)
							&& circle->getFillColor().g / 3 - circle->getFillColor().r - circle->getFillColor().b > 0
							&& circle->getFillColor().g / 3 - circle->getFillColor().r - circle->getFillColor().b < 60)
						{
							greenCircleReaction();
						}
						else
						{
							redCircleReaction();
						}
					}
					else if (circle2 != NULL && circle2->getGlobalBounds().intersects(human.getFeetGlobalBounds()))
					{
						greenCircleReaction();
					}
				}
			}
			if (event.type == sf::Event::MouseMoved)
			{
				if (gameStatus == G_Create)
				{
					if (mouseHas != NULL)
					{
						mouseHas->setPosition(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y);
					}
				}
				if (circle != NULL && circle->getGlobalBounds().contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y) && gameStatus == G_Point)
				{
					circle->setFillColor(sf::Color(25, 165, 20));
				}
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					if (morseCode.isLighting() && !morseCode.getButtonGlobalBounds().contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y))
					{
						morseCode.releaseReaction();
					}
					if (pickerR.getGlobalBounds().contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y))
						pickerR.actualizeValue(sf::Mouse::getPosition(*window));
					if (pickerG.getGlobalBounds().contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y))
						pickerG.actualizeValue(sf::Mouse::getPosition(*window));
					if (pickerB.getGlobalBounds().contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y))
						pickerB.actualizeValue(sf::Mouse::getPosition(*window));
					if (gameStatus == G_IntensivityRGB)
						circle->setFillColor(sf::Color(25, pickerG.getValue(), 20));
					if (gameStatus == G_RGB)
						circle->setFillColor(sf::Color(pickerR.getValue(), pickerG.getValue(), pickerB.getValue()));	
				}
			}
			if (event.type == sf::Event::KeyReleased)
			{
				stopHuman(event.key.code);
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (gameStatus == G_Create)
				{
					if (circle->getGlobalBounds().contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y))
						mouseHas = circle;
					else if (circle2->getGlobalBounds().contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y))
						mouseHas = circle2;
				}
				else if (event.mouseButton.button == sf::Mouse::Left && gameStatus == G_Click)
				{
					if (circle->getGlobalBounds().contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y))
					{
						circle->setFillColor(sf::Color(25, 165, 20));
					}
				}
				else if (event.mouseButton.button == sf::Mouse::Left)
				{
					if (tree.getGlobalBounds().contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y))
					{
						shake = true;
						if (gameStatus == G_TreeClick)
							counter--;
					}
					else if (balloonGreen.getGlobalBounds().contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y))
					{
						if (gameStatus == G_Balloon)
						{
							if (chance > 1)
								chance--;
							int i = rand() % chance + 1;
							if (i == 1)
							{
								circle->setPosition(balloonGreen.getPosition());
								circle->setFillColor(sf::Color(25, 165, 20));
								chance = 35;
							}
						}
						balloonGreen.breakTheBalloon();
					}
					else if (balloonRed.getGlobalBounds().contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y))
					{
						if (gameStatus == G_Balloon)
						{
							chance++;
							int i = rand() % chance + 1;
							if (i == 1)
							{
								circle->setPosition(balloonRed.getPosition());
								circle->setFillColor(sf::Color(165, 25, 20));
								chance = 50;
							}
						}
						balloonRed.breakTheBalloon();
					}
					else if (morseCode.getButtonGlobalBounds().contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y))
					{
						morseCode.clickReaction();
					}
					else
					{
						if (hexPicker3.checkAndActualize(sf::Mouse::getPosition(*window)) && gameStatus == G_Hex3)
							circle->setFillColor(hexPicker3.getColor());
						if (hexPicker6.checkAndActualize(sf::Mouse::getPosition(*window)) && gameStatus == G_Hex6)
							circle->setFillColor(hexPicker6.getColor());
					}
				}
			}
			if (event.type == sf::Event::MouseButtonReleased)
			{
				if (gameStatus == G_Create)
				{
					if (mouseHas != NULL)
					{
						mouseHas = NULL;
						if (circle->getGlobalBounds().intersects(circle2->getGlobalBounds()))
						{
							circle->setFillColor(sf::Color(25, 165, 20));
							circle2->setPosition(sf::Vector2f(-20, -20));
						}
					}
				}
				if (morseCode.getButtonGlobalBounds().contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y))
				{
					morseCode.releaseReaction();
				}
			}
		}
		if (windowNewPosition != windowPosition && gameStatus == G_TreeMove)
		{
			windowPosition = windowNewPosition;
			if (gameStatus == G_TreeMove)
				counter--;
			shake = true;
		}
		if (counter <= 0)
		{
			circle->setFillColor(sf::Color(25, 165, 20));
			counter = 25;
			if (gameStatus == G_TreeClick)
				circle->setPosition(340, 390);
			else if (gameStatus == G_TreeMove)
				circle->setPosition(400, 390);
		}
		if (gameStatus == G_Choose)
		{
			circlesRequest();
		}
		if (gameStatus == G_HappyEnd)
		{
			if (movingClock.getElapsedTime() >= moveTime)
			{
				human.walkLeft();
				movingClock.restart();
			}
			if (human.getPosition().x < -100)
				window->close();
		}
		if (gameStatus == G_Died)
		{
			if (human.getLifeStatus() == human.BOMB)
			{
				if (movingClock.getElapsedTime() >= moveTime)
				{
					human.killByBomb();
					movingClock.restart();
				}
			}
			if (stringIterations == string.getSize())
			{
				sf::sleep(sf::milliseconds(100));
				window->close();
			}
		}

		if (stringIterations < string.getSize())
		{
			appendAnotherLetter();
			if (stringIterations == basicString.getSize())
			{
				status = waiting;
			}
		}
		drawEverything();
		window->display();
	}
	return true;
}


int Game::checkAnswer()
{
	std::string smallLetters = string;
	for (int i = 0; i < smallLetters.size(); ++i)
		smallLetters[i] = tolower(smallLetters[i]);

	if (smallLetters.find("green") != std::string::npos)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int Game::checkAnswer2()
{
	std::string smallLetters = string;
	for (int i = 0; i < smallLetters.size(); ++i)
		smallLetters[i] = tolower(smallLetters[i]);

	if (smallLetters.find("neerg") != std::string::npos)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void Game::createPrice()
{
	std::ofstream filePrice;
	filePrice.open("Message/ForYou.jpg", std::ofstream::out | std::ofstream::trunc);
	filePrice.close();

	sf::RenderTexture renderTexture;
	renderTexture.create(250, 200);
	renderTexture.clear(sf::Color(155, 255, 155));
	sf::Font* voucherFont = new sf::Font();
	voucherFont->loadFromFile("Fonts/Bodoni.ttf");


	sf::Text text = sf::Text("Gift Voucher", *voucherFont, 30);
	text.setFillColor(sf::Color::Green);
	text.setPosition(sf::Vector2f(10, 190));
	text.scale(-1.f, 1.f);
	text.setRotation(180);
	renderTexture.draw(text);

	sf::Text text2 = sf::Text("For\nFrom\nThis coupon entitles to", *voucherFont, 20);
	text2.setFillColor(sf::Color(50, 150, 50));
	text2.setPosition(sf::Vector2f(20, 150));
	text2.scale(-1.f, 1.f);
	text2.setRotation(180);
	renderTexture.draw(text2);

	sf::Text text3 = sf::Text(L"<change in code ;)>\n   <change in code ;)>", *font, 20);
	text3.setFillColor(sf::Color(0, 0, 0));
	text3.setPosition(sf::Vector2f(60, 150));
	text3.scale(-1.f, 1.f);
	text3.setRotation(180);
	renderTexture.draw(text3);

	sf::Text text4 = sf::Text("<change in code ;)>", *voucherFont, 15);
	text4.setFillColor(sf::Color(50, 100, 50));
	text4.setPosition(sf::Vector2f(20, 70));
	text4.scale(-1.f, 1.f);
	text4.setRotation(180);
	renderTexture.draw(text4);

	sf::Text text5 = sf::Text("NO 000001", *voucherFont, 17);
	text5.setFillColor(sf::Color(20, 50, 20));
	text5.setPosition(sf::Vector2f(150, 20));
	text5.scale(-1.f, 1.f);
	text5.setRotation(180);
	renderTexture.draw(text5);

	sf::Texture capturedTexture = renderTexture.getTexture();
	sf::Image image = capturedTexture.copyToImage();

	image.saveToFile("Message/ForYou.jpg");
}

void Game::moveHuman(sf::Keyboard::Key key)
{
	if (movingClock.getElapsedTime() >= moveTime)
	{
		if (key == sf::Keyboard::Up)
			human.walk(Human::Direction::NONE, Human::Direction::UP);		
		else if (key == sf::Keyboard::Left)
			human.walk(Human::Direction::LEFT, Human::Direction::NONE);
		else if (key == sf::Keyboard::Down)
			human.walk(Human::Direction::NONE, Human::Direction::DOWN);
		else if (key == sf::Keyboard::Right)
			human.walk(Human::Direction::RIGHT, Human::Direction::NONE);
		movingClock.restart();
	}
}

void Game::stopHuman(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Up)
		human.stand(Human::Direction::UP);
	else if (key == sf::Keyboard::Down)
		human.stand(Human::Direction::DOWN);
	else if (key == sf::Keyboard::Left)
		human.stand(Human::Direction::LEFT);
	else if (key == sf::Keyboard::Right)
		human.stand(Human::Direction::RIGHT);
}

void Game::drawEverything()
{
	if (shake)
	{
		if (gameStatus == G_TreeClick)
			shake = tree.shake();
		else
			shake = tree.shake();
	}
	if (gameStatus == G_HappyEnd)
	{
		window->draw(background);
		showText();
		window->draw(background);
		window->draw(human);
		window->draw(text);
	}
	else
	{
		window->draw(tree);
		window->draw(hexPicker3);
		window->draw(hexPicker6);

		window->draw(pickerR);
		window->draw(pickerG);
		window->draw(pickerB);

		if (gameStatus == G_Morse && morseCode.actualize().toAnsiString() == "GREEN")
		{
			circle->setFillColor(sf::Color(25, 165, 20));
		}
		window->draw(morseCode);

		if (gameStatus != G_Choose)
			window->draw(*circle);
		if (drawCircle && gameStatus == G_Choose)
		{
			window->draw(*circle);
			window->draw(*circle2);
		}
		if (gameStatus == G_Create)
			window->draw(*circle2);

		showText();
		window->draw(human);
		window->draw(tree);

		balloonRed.fly();
		window->draw(balloonRed);

		balloonGreen.fly();
		window->draw(balloonGreen);

		window->draw(text);
	}
}

void Game::circlesRequest()
{
	static double transparent = 0;
	if (drawCircle && transparent<255 && stringIterations>50)
	{
		circle->setFillColor(sf::Color(165, 25, 20, transparent));
		circle2->setFillColor(sf::Color(25, 165, 20, transparent));
		transparent += 0.05;
	}
	if (movingClock.getElapsedTime() >= moveTime && human.getPosition().x < 130 && block)
	{
		human.walk(Human::Direction::RIGHT, Human::Direction::NONE);
		movingClock.restart();
	}
	else if (human.getPosition().x == 130 && block == true)
	{
		if (status == waiting)
		{
			block = false;

			circle = new sf::CircleShape(10);
			randCirclePosition();
			circle2 = new sf::CircleShape(10);
			randCirclePosition(false);

			text.setPosition(200, 200);
			string = "Somebody who has placed here " + human.getName() + " has also left some tips.\nHere is the first: Red or green?\n";
			basicString = string;
			status = writing;
			stringIterations = 0;
			basicMessageLength = string.getSize();
			messageLenght = string.getSize();
			actualLineLength = 0;

			drawCircle = true;
			circle->setFillColor(sf::Color(165, 25, 20, transparent));
			circle2->setFillColor(sf::Color(25, 165, 20, transparent));
		}
		human.stand(Human::Direction::DOWN);
	}
}

void Game::redCircleReaction()
{
	human.setPosition(human.getPosition().x - 115 + 32, human.getPosition().y - 200 + 64);
	human.setLifeStatus(Human::BOMB);
	block = true;
	human.killByBomb();
	gameStatus = G_Died;

	std::ofstream file;
	file.open("status_info.txt", std::ofstream::out | std::ofstream::trunc);
	file << human.getName().toAnsiString();
	file.close();

	finalMessage();
}

void Game::greenCircleReaction()
{
	nextLevel();
	circle2 = NULL;
	randCirclePosition();
	circle->setFillColor(sf::Color(165, 25, 20));
	drawCircle = false;
	sf::String s = "";

	if (gameStatus == G_Point)
	{
		s = "You made a correct decision! " + human.getName() +" is still alive.\nTip: Point it.\n";
	}
	else if (gameStatus == G_Click)
	{
		s = "Tip: Like a button.\n";
	}
	else if (gameStatus == G_IntensivityRGB)
	{
		s = "Tip: Red, green, blue... maybe only green.\nBut it must be perfectly as it was!\n";
		circle->setFillColor(sf::Color(25, pickerG.getValue(), 20));
	}
	else if (gameStatus == G_RGB)
	{
		pickerG.actualizeValue(sf::Vector2i(0, 255 - rand() % 50));
		s = "Tip: Red, green, blue.\n";
		circle->setFillColor(sf::Color(pickerR.getValue(), pickerG.getValue(), pickerB.getValue()));
	}
	else if (gameStatus == G_TreeClick)
	{
		s = "Tip: Drop.\n";
		circle->setPosition(-20, -20);
	}
	else if (gameStatus == G_TreeMove)
	{
		s = "Tip: Drop more.\nBut clicking is useless.\n";
		circle->setPosition(-20, -20);
	}
	else if (gameStatus == G_Hex3)
	{
		s = "Tip: Hex triplet (tree digit)\n";
		circle->setFillColor(hexPicker3.getColor());
	}
	else if (gameStatus == G_Hex6)
	{
		s = "Tip: Hex triplet (six digit)\n";
		circle->setFillColor(hexPicker6.getColor());
	}
	else if (gameStatus == G_Write)
	{
		s = "Tip: You need just a word.\n";
	}
	else if (gameStatus == G_Etirw)
	{
		s = "Tip: You need just a drow.\n";
	}
	else if (gameStatus == G_Balloon)
	{
		s = "Tip: Pop some balloons.\n";
		circle->setPosition(-20, -20);
	}
	else if (gameStatus == G_Morse)
	{
		s = "Tip: Do it like a scout.\n Use electrical telegraph system.\n";
	}
	else if (gameStatus == G_Create)
	{
		s = "Tip: Connect and create.\n";
		circle->setFillColor(sf::Color::Yellow);
		circle2 = new sf::CircleShape(10);
		circle2->setFillColor(sf::Color::Blue);
		randCirclePosition(false);
		randCirclePosition();
	}
	else if (gameStatus == G_HappyEnd)
	{
		s = "You made it! " + human.getName() + " is free now.\nI have something for you. Check out the 'Message' folder :)\n";
		
		createPrice();
		
		window->create(sf::VideoMode(windowXSize, windowYSize), human.getName() + " is free", sf::Style::Fullscreen);
		text.setPosition(2 * windowXSize / 3, windowYSize / 3);
		human.setPosition(windowYSize - 64, windowXSize / 2);
		human.setWindowSize(sf::Vector2u(windowXSize, windowYSize));

		std::ofstream file;
		file.open("status_info.txt", std::ofstream::out | std::ofstream::trunc);
		file.close();

		block = true;
	}
	string = s;
	basicString = s;
	status = writing;
	stringIterations = 0;
	basicMessageLength = s.getSize();
	messageLenght = s.getSize();
	actualLineLength = 0;
}

void Game::randCirclePosition(bool basicCircle)
{
	if (basicCircle)
		do {
			circle->setPosition(rand() % (window->getSize().x - 84) + 64, rand() % (window->getSize().y - 84) + 64);
		} while (
			circle2 != NULL && circle2->getGlobalBounds().intersects(circle->getGlobalBounds()) ||
			human.getGlobalBounds().intersects(circle->getGlobalBounds()) ||
			tree.getGlobalBounds().intersects(circle->getGlobalBounds()) ||
			pickerB.getGlobalBounds().intersects(circle->getGlobalBounds()) ||
			pickerG.getGlobalBounds().intersects(circle->getGlobalBounds()) ||
			pickerR.getGlobalBounds().intersects(circle->getGlobalBounds()) ||
			hexPicker3.getGlobalBounds().intersects(circle->getGlobalBounds()) ||
			hexPicker6.getGlobalBounds().intersects(circle->getGlobalBounds()) ||
			morseCode.getGlobalBounds().intersects(circle->getGlobalBounds())
			);
	else
		do {
			circle2->setPosition(rand() % (window->getSize().x - 84) + 64, rand() % (window->getSize().y - 84) + 64);
		} while (
			circle->getGlobalBounds().intersects(circle2->getGlobalBounds()) ||
			human.getGlobalBounds().intersects(circle2->getGlobalBounds()) ||
			tree.getGlobalBounds().intersects(circle2->getGlobalBounds()) ||
			pickerB.getGlobalBounds().intersects(circle2->getGlobalBounds()) ||
			pickerG.getGlobalBounds().intersects(circle2->getGlobalBounds()) ||
			pickerR.getGlobalBounds().intersects(circle2->getGlobalBounds()) ||
			hexPicker3.getGlobalBounds().intersects(circle2->getGlobalBounds()) ||
			hexPicker6.getGlobalBounds().intersects(circle2->getGlobalBounds()) ||
			morseCode.getGlobalBounds().intersects(circle->getGlobalBounds())
			);

}

void Game::nextLevel()
{
	if (gameStatus == G_Choose)
	{
		gameStatus = G_Point;
		//gameStatus = G_HappyEnd;
		return;
	}
	else if (gameStatus == G_Point)
	{
		gameStatus = G_Write;
		return;
	}
	else if (gameStatus == G_Write)
	{
		gameStatus = G_Click;
		return;
	}		
	else if (gameStatus == G_Click)
	{
		gameStatus = G_IntensivityRGB;
		return;
	}
	else if (gameStatus == G_IntensivityRGB)
	{
		gameStatus = G_Create;
		return;
	}
	else if (gameStatus == G_Create)
	{
		gameStatus = G_RGB;
		return;
	}
	else if (gameStatus == G_RGB)
	{
		gameStatus = G_TreeClick;
		return;
	}	
	else if (gameStatus == G_TreeClick)
	{
		gameStatus = G_Hex3;
		return;
	}
	else if (gameStatus == G_Hex3)
	{
		gameStatus = G_Balloon;
		return;
	}
	else if (gameStatus == G_Balloon)
	{
		gameStatus = G_TreeMove;
		return;
	}
	else if (gameStatus == G_TreeMove)
	{
		gameStatus = G_Hex6;
		return;
	}
	else if (gameStatus == G_Hex6)
	{
		gameStatus = G_Etirw;
		return;
	}
	else if (gameStatus == G_Etirw)
	{
		gameStatus = G_Morse;
		return;
	}
	else if (gameStatus == G_Morse)
	{
		gameStatus = G_HappyEnd;
		return;
	}
}

void Game::finalMessage()
{
	string = "Oh no! You've just killed him!\nI can not look at this :( Bye ";
	status = writing;
	stringIterations = 0;
}

