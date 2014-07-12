#ifndef CHAR_H
#define CHAR_H
#include <SFML/Graphics.hpp>
#include <string>

enum Direction {LEFT, RIGHT};

class Character
{
private:
	sf::Sprite charSprite;
	sf::IntRect leftRect;
	sf::IntRect rightRect;
	std::string name;
	Direction charDir;
	sf::Sprite getCharSprite();
public:
	Character();
	Character(sf::Sprite charSprite, sf::IntRect leftRect, sf::IntRect rightRect);
	Character(sf::Sprite charSprite, sf::IntRect leftRect, sf::IntRect rightRect, Direction dir);
	Character(sf::Sprite charSprite, sf::IntRect leftRect, sf::IntRect rightRect, Direction dir, std::string name);
	Character(sf::Sprite charSprite, sf::IntRect leftRect, sf::IntRect rightRect, std::string name);
	void setCharacter(sf::Sprite charSprite, sf::IntRect leftRect, sf::IntRect rightRect);
	void setCharacter(sf::Sprite charSprite, sf::IntRect leftRect, sf::IntRect rightRect, Direction dir);
	void setCharacter(sf::Sprite charSprite, sf::IntRect leftRect, sf::IntRect rightRect, Direction dir, std::string name);
	void setCharacter(sf::Sprite charSprite, sf::IntRect leftRect, sf::IntRect rightRect, std::string name);
	void setPosition(float x, float y);
	void setOrigin(float x, float y);
	void setName(std::string name);
	void move(float x, float y);
	void changeDirection(Direction dir);
	void draw(sf::RenderWindow *window);
	bool isColliding(sf::Sprite *obj);
	bool isColliding(Character *chr);
	sf::Vector2f getPosition();
	Direction getDirection();
	sf::FloatRect getBounds();
	std::string getName();
};
#endif