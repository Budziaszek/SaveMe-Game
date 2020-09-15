#ifndef __HUMAN_h
#define __HUMAN_h
#include <SFML\Graphics.hpp>
#include <windows.h>

class Human
	:public sf::Drawable, public sf::Transformable
{
public:
	typedef enum { FEMALE = 1, MALE = 0 } Sex;
	typedef enum { NONE = -1, UP = 0, LEFT = 1, DOWN = 2, RIGHT = 3}Direction;
	typedef enum { ALIVE, BOMB } Death;

	Human(Sex givenSex = MALE, sf::String givenName = "Name", sf::Vector2u givenWindowSize = sf::Vector2u(0, 0));	
	
	void setLifeStatus(Death status);
	Human::Death getLifeStatus();	

	sf::FloatRect getGlobalBounds();
	sf::FloatRect getFeetGlobalBounds();
	
	void setWindowSize(sf::Vector2u size);
	void setPosition(int x, int y);
	sf::String getName();
	sf::Vector2f getPosition();

	void walk(Direction x, Direction y);
	void stand(Direction d);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void killByBomb();
	void walkLeft();
	
private:
	Sex sex;
	sf::String name;
	sf::Vector2u windowSize;
	Death lifeStatus;

	sf::VertexArray humanVertex;
	sf::Texture humanTileset;
	sf::Texture bombTileset;
	int tileSize;
	int tileNumber;

	int walkStatus;
	int numberOfWalkStatuses;
	int speed;
	int maxActiveHeight;
	int maxActiveWidth;
	int basicX;
	int basicY;
	
	void setAppearance(int tileNumber, int size = 64);
};
#endif

