#include "Manager.h"


Manager::Manager()
{
	sf::Image icon;
	icon.loadFromFile("Sprites/poker.png");

	window = new sf::RenderWindow(sf::VideoMode(800, 600), "SaveME", sf::Style::Close);
	window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	bigFont = new sf::Font();
	bigFont->loadFromFile("Fonts/Caveat.ttf");

	begin();
}

int Manager::begin()
{
	StartMessage message;
	std::ifstream file;
	file.open("status_info.txt");
	std::string name;
	file >> name;
	file.close();
	srand(time(NULL));

	std::string names [] = { "Alan", "Alexander", "Andrew", "Anthony", "Arthur", "Barry", 
		"Bart", "Ben", "Bob", "Calvin", "Carey", "Carl", "Chris", "Daley", "Dan", "David",
		"Denis", "Desmond", "Dex", "Don", "Dorian", "Dylan", "Ed", "Edward", "Eric", "Ethan", 
		"Felix", "Gary", "Harris", "Hector", "Henry", "Ian", "Jack", "Jacob", "James", "Jerry", 
		"Jervis", "Joe", "John", "Julian", "Karl", "Kenny", "Kevin", "Kris", "Larry", "Leo", 
		"Manuel", "Mark", "Martie", "Mathew", "Max", "Mike", "Ned", "Nevil", "Nick", "Norman",
		"Oliver", "Oscar", "Patrick", "Paul", "Peter", "Phil", "Rian", "Rick", "Rob", "Roy", 
		"Ryan", "Sam", "Stan", "Stew", "Ted", "Thomas", "Troy", "Will", "Zack"};

	if(name.size()>0)
		message = StartMessage(window, bigFont, true, name);
	else
		message = StartMessage(window, bigFont);

	name = names[rand() % 75];
 
	if (message.mainLoop())
	{
		Game game = Game(window, bigFont, sf::String(name));
		game.mainLoop();
	}
	//system("PAUSE");

	return 0;
}

