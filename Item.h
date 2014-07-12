#ifndef ITEM_H
#define ITEM_H
#include <SFML/Graphics.hpp>
#include "Collision.h"
#include "Character.h"

class Item
{
private:
	sf::Sprite item;
	int index;
public:
	Item();
	Item(sf::Sprite item, int index);
	void setItem(sf::Sprite item, int index);
	void display(sf::RenderWindow& window);
	void moveItem(float x, float y);
	void setPosition(float x, float y);
	float getXPos();
	float getYPos();
	bool checkCollision(sf::Sprite obj);
	bool checkCollision(Character charc);
	int getIndex();
};

#endif