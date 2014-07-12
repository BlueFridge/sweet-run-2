#include "Item.h"
#include "Collision.h"

Item::Item()
{
	//Do nothing
}

Item::Item(sf::Sprite item, int index)
{
	this->item = item;
	this->index = index;
	
	this->item.setOrigin(this->item.getLocalBounds().width*0.5, this->item.getLocalBounds().height*0.5);
}

void Item::setItem(sf::Sprite item, int index)
{
	this->item = item;
	this->index = index;
	
	this->item.setOrigin(this->item.getLocalBounds().width*0.5, this->item.getLocalBounds().height*0.5);
}

void Item::display(sf::RenderWindow& window)
{
	window.draw(item);
}

void Item::moveItem(float x, float y)
{
	this->item.move(x, y);
}

void Item::setPosition(float x, float y)
{
	this->item.setPosition(x, y);
}

float Item::getXPos()
{
	return this->item.getPosition().x;
}

float Item::getYPos()
{
	return this->item.getPosition().y;
}

bool Item::checkCollision(sf::Sprite obj)
{
	return Collision::PixelPerfectTest(obj, this->item);
}

bool Item::checkCollision(Character charc)
{
	return charc.isColliding(&(this->item));
}

int Item::getIndex()
{
	return this->index;
}