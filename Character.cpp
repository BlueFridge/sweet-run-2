#include "Character.h"
#include "Collision.h"

sf::Sprite Character::getCharSprite()
{
	return this->charSprite;
}

Character::Character()
{
	//Do nothing
}

Character::Character(sf::Sprite charSprite, sf::IntRect leftRect, sf::IntRect rightRect)
{
	this->charSprite = charSprite;
	this->leftRect = leftRect;
	this->rightRect = rightRect;
	this->charDir = RIGHT;
	this->charSprite.setTextureRect(this->rightRect);
	this->name = "Nil";
}

Character::Character(sf::Sprite charSprite, sf::IntRect leftRect, sf::IntRect rightRect, Direction dir)
{
	this->charSprite = charSprite;
	this->leftRect = leftRect;
	this->rightRect = rightRect;
	this->charDir = dir;
	changeDirection(this->charDir);
	this->name = "Nil";
}

Character::Character(sf::Sprite charSprite, sf::IntRect leftRect, sf::IntRect rightRect, Direction dir, std::string name)
{
	this->charSprite = charSprite;
	this->leftRect = leftRect;
	this->rightRect = rightRect;
	this->charDir = dir;
	changeDirection(this->charDir);
	setName(name);
}

Character::Character(sf::Sprite charSprite, sf::IntRect leftRect, sf::IntRect rightRect, std::string name)
{
	this->charSprite = charSprite;
	this->leftRect = leftRect;
	this->rightRect = rightRect;
	this->charDir = RIGHT;
	changeDirection(this->charDir);
	setName(name);
}

void Character::setCharacter(sf::Sprite charSprite, sf::IntRect leftRect, sf::IntRect rightRect)
{
	this->charSprite = charSprite;
	this->leftRect = leftRect;
	this->rightRect = rightRect;
	this->charDir = RIGHT;
	this->charSprite.setTextureRect(this->rightRect);
	this->name = "Nil";
}

void Character::setCharacter(sf::Sprite charSprite, sf::IntRect leftRect, sf::IntRect rightRect, Direction dir)
{
	this->charSprite = charSprite;
	this->leftRect = leftRect;
	this->rightRect = rightRect;
	this->charDir = dir;
	changeDirection(this->charDir);
	this->name = "Nil";
}

void Character::setCharacter(sf::Sprite charSprite, sf::IntRect leftRect, sf::IntRect rightRect, Direction dir, std::string name)
{
	this->charSprite = charSprite;
	this->leftRect = leftRect;
	this->rightRect = rightRect;
	this->charDir = dir;
	changeDirection(this->charDir);
	setName(name);
}

void Character::setCharacter(sf::Sprite charSprite, sf::IntRect leftRect, sf::IntRect rightRect, std::string name)
{
	this->charSprite = charSprite;
	this->leftRect = leftRect;
	this->rightRect = rightRect;
	this->charDir = RIGHT;
	changeDirection(this->charDir);
	setName(name);
}

void Character::setPosition(float x, float y)
{
	this->charSprite.setPosition(x, y);
}

void Character::setOrigin(float x, float y)
{
	this->charSprite.setOrigin(x, y);
}

void Character::setName(std::string name)
{
	this->name = name;
}

void Character::move(float x, float y)
{
	this->charSprite.move(x, y);
}

void Character::changeDirection(Direction dir)
{
	this->charDir = dir;
	switch(this->charDir)
	{
		case LEFT:
			this->charSprite.setTextureRect(this->leftRect);
			break;
		case RIGHT:
			this->charSprite.setTextureRect(this->rightRect);
			break;
	}
}

void Character::draw(sf::RenderWindow *window)
{
	window->draw(this->charSprite);
}

bool Character::isColliding(sf::Sprite *obj)
{
	return Collision::PixelPerfectTest(this->charSprite, *obj);
}

bool Character::isColliding(Character *chr)
{
	return Collision::PixelPerfectTest(this->charSprite, chr->getCharSprite());
}

sf::Vector2f Character::getPosition()
{
	return this->charSprite.getPosition();
}

Direction Character::getDirection()
{
	return this->charDir;
}

sf::FloatRect Character::getBounds()
{
	return this->charSprite.getLocalBounds();
}

std::string Character::getName()
{
	return this->name;
}