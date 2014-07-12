#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "LevelParser.h"
#include "Collision.h"
#include "Character.h"
#include "Item.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

const int WIDTH = 800;
const int HEIGHT = 500;
const int BPP = 32;
const int MOVSPEED = 100;
enum Players {SALAH, MOEEZ};

//Textures, fonts and sprites
sf::Texture bFrameTex;
sf::Texture bStageTex;
sf::Texture itemsTex;
sf::Font font;
sf::Texture smallCharacters;
sf::Texture bigCharacters;
sf::Sprite player;
sf::Sprite enemy;
sf::Sprite shadyBig;
sf::Sprite ninjaBig;
sf::Sprite cyborgBig;
sf::Sprite blueBig;
sf::Sprite devilBig;
sf::Sprite bFrameSprite;
sf::Sprite bStageSprite;
sf::Sprite bullet;
sf::Sprite candyPurple;
sf::Sprite sweet;
sf::Sprite candyYellow;
sf::Sprite healthPotion;
Item CandyItemP;
Item CandyItemY;
Item SweetItem;
Item hpPotionItem;

//Converts something to its string representation
template<class T>
std::string toStr(T something)
{
	std::stringstream ss;
	std::string str;
	ss << something;
	ss >> str;
	return str;
}

//Gets the levels available from the configuration file
std::vector<std::string> getLevelList(std::string levelStr)
{
	std::vector<std::string> outputVect;
	std::ifstream levelFileStream(levelStr.c_str());
	std::string temp;
	while(std::getline(levelFileStream, temp))
	{
		outputVect.push_back(temp);
	}
	levelFileStream.close();
	return outputVect;
}

//Loads the stage and sets a given sf::Texture to it.
int setStage(sf::Texture *bgTex, std::string stagePath)
{
	if(!bgTex->loadFromFile(stagePath))
		return 1;
	return 0;
}

//Updates the stage sprite
void updateStage(sf::Texture *bgTex, sf::Sprite *bgSpr)
{
	bgSpr->setTexture(*bgTex);
}

//Checks whether an element is present in a vector or not
template<class T>
bool isPresent(std::vector<T> vect, T element)
{
	for(int i = 0; i < vect.size(); i++)
	{
		if(vect[i] == element)
			return true;
	}
	return false;
}

//Returns a vector containing items
std::vector<Item> getItems(LevelParser *parser)
{
	std::vector<Item> tempItems;
	tempItems.push_back(CandyItemP);
	tempItems.push_back(CandyItemY);
	tempItems.push_back(SweetItem);
	tempItems.push_back(hpPotionItem);
	
	std::vector<Item> itemVect;
	
	int nItems = parser->getRows();
	int itemsInTempVect = tempItems.size();
	
	std::vector<int> yCoors = parser->getYCoors();
	std::vector<std::pair<int, int> > xCoors = parser->getXCoors();
	int xCoorsSize = xCoors.size();
	std::vector<int> takenYCoorsN;
	
	for(int i = 0; i < nItems; i++)
	{
		int randomNum = std::rand()%itemsInTempVect;
		itemVect.push_back(tempItems[randomNum]);
		
		std::pair<int, int> xCoorP = xCoors[i];
		
		int xCoor = (std::rand()%(std::abs(xCoorP.first-xCoorP.second)))+std::min(xCoorP.first, xCoorP.second);
		int yCoor = yCoors[i];
		
		itemVect.back().setPosition(xCoor,yCoor);
		
		randomNum = std::rand()%itemsInTempVect;
		
		itemVect.push_back(tempItems[randomNum]);
		
		xCoor = (std::rand()%(std::abs(xCoorP.first-xCoorP.second)))+std::min(xCoorP.first, xCoorP.second);
		
		itemVect.back().setPosition(xCoor,yCoor);
	}
	
	return itemVect;
}

//Returns a vector containing enemies
std::vector<Character> getEnemies(LevelParser *parser)
{
	//Character devil coordinates
	sf::IntRect devilRight(185,8,32,31);
	sf::IntRect devilLeft(190,53,30,31);
	//--//
	//Character blue coordinates
	sf::IntRect blueRight(52,8,29,30);
	sf::IntRect blueLeft(52,54,29,30);
	//---//
	//Character cyborg coordinates
	sf::IntRect cyborgRight(96,7,31,31);
	sf::IntRect cyborgLeft(98,53,31,31);
	//--//
	
	int nEnemies = parser->getRows()/2;
	std::vector<int> yCoors = parser->getYCoors();
	std::vector<std::pair<int, int> > xCoors = parser->getXCoors();
	std::vector<int> takenYCoorsN;
	takenYCoorsN.push_back(-1);
	std::vector<Character> enemyVect;
	
	for(int i = 0; i < nEnemies; i++)
	{
		Character enemyChar;
		int randomNum = std::rand()%3;
		switch(randomNum)
		{
			case 0:
			{
				enemyChar.setCharacter(enemy, devilLeft, devilRight);
				enemyChar.setName("Devil");
				break;
			}
			case 1:
			{
				enemyChar.setCharacter(enemy, blueLeft, blueRight);
				enemyChar.setName("Blue");
				break;
			}
			case 2:
			{
				enemyChar.setCharacter(enemy, cyborgLeft, cyborgRight);
				enemyChar.setName("Cyborg");
				break;
			}
		}
		
		int vectSize = yCoors.size();
		randomNum = std::rand()%vectSize;
		while(isPresent(takenYCoorsN, randomNum))
		{
			randomNum = std::rand()%vectSize;
		}
		
		int yCoor = yCoors[randomNum];
		std::pair<int, int> xCoorP = xCoors[randomNum];
		int xCoor = (std::rand()%(std::abs(xCoorP.first-xCoorP.second)))+std::min(xCoorP.first, xCoorP.second);
		
		enemyChar.setOrigin(enemyChar.getBounds().width*0.5, enemyChar.getBounds().height*0.5);
		enemyChar.setPosition(xCoor, yCoor);
		
		enemyVect.push_back(enemyChar);
		takenYCoorsN.push_back(randomNum);
	}
	
	return enemyVect;
}

//The battle stage
int battleStage(sf::RenderWindow *window, sf::Clock *frameClock, Players plr, std::string enemyName, int &hp)
{
	sf::Text playerText;
	sf::Text enemyText;
	playerText.setFont(font);
	enemyText.setFont(font);
	playerText.setString("Your HP: ");
	enemyText.setString("Enemy HP:");
	playerText.setCharacterSize(16);
	enemyText.setCharacterSize(16);
	playerText.setColor(sf::Color::White);
	enemyText.setColor(sf::Color::White);
	playerText.setPosition(21,21);
	enemyText.setPosition(21,playerText.getLocalBounds().height+30);
	
	int enemyHP = 100;
	
	sf::RectangleShape hpBarPlayer;
	hpBarPlayer.setSize(sf::Vector2f(hp*2,15));
	hpBarPlayer.setFillColor(sf::Color::Red);
	hpBarPlayer.setPosition(playerText.getPosition().x+playerText.getLocalBounds().width, playerText.getPosition().y);
	
	sf::RectangleShape hpBarEnemy;
	hpBarEnemy.setSize(sf::Vector2f(enemyHP*2,15));
	hpBarEnemy.setFillColor(sf::Color::Yellow);
	hpBarEnemy.setPosition(enemyText.getPosition().x+enemyText.getLocalBounds().width+3, enemyText.getPosition().y);
	
	sf::Sprite playerSprite;
	sf::Sprite enemySprite;
	
	sf::RectangleShape divider;
	divider.setSize(sf::Vector2f(1,HEIGHT));
	divider.setOrigin(divider.getLocalBounds().width*0.5, divider.getLocalBounds().height*0.5);
	divider.setPosition(WIDTH*0.5, HEIGHT*0.5);
	divider.setFillColor(sf::Color::Transparent);
	
	switch(plr)
	{
		case SALAH:
			playerSprite = shadyBig;
			break;
		case MOEEZ:
			playerSprite = ninjaBig;
			break;
	}
	
	if(enemyName == "Devil")
		enemySprite = devilBig;
	else if(enemyName == "Blue")
		enemySprite = blueBig;
	else if(enemyName == "Cyborg")
		enemySprite = cyborgBig;
	
	playerSprite.setOrigin(0, playerSprite.getLocalBounds().height);
	enemySprite.setOrigin(enemySprite.getLocalBounds().width, enemySprite.getLocalBounds().height);
	int yPosChar = 480;
	playerSprite.setPosition(50,yPosChar);
	enemySprite.setPosition(760,yPosChar);
	
	float delta;
	
	sf::Clock enemyMovClock;
	int enemyMovDir = 0;
	
	std::vector<sf::Sprite> bulletVector;
	sf::Clock bulletTimer;
	
	std::vector<sf::Sprite> enemyBulletVector;
	sf::Clock enemyBulletTimer;
	
	sf::Clock gravityTimer;
	bool inJump = false;
	float upDownVelocity = 500;
	int jumpHeight = 25;
	
	sf::Clock enemyGravityTimer;
	bool enemyInJump = false;
	bool shouldJump = false;
	
	while(window->isOpen())
	{
		delta = frameClock->restart().asSeconds();
		sf::Event ev;
		while(window->pollEvent(ev))
		{
			switch(ev.type)
			{
				case sf::Event::Closed:
				{
					return -1;
					break;
				}
				case sf::Event::KeyPressed:
				{	//Player bullet handling
					if((ev.key.code == sf::Keyboard::Space) && bulletTimer.getElapsedTime().asSeconds() >= 0.5)
					{
						bulletVector.push_back(bullet);
						bulletVector.back().setPosition(playerSprite.getPosition().x+playerSprite.getLocalBounds().width*0.5,
														playerSprite.getPosition().y-playerSprite.getLocalBounds().height*0.5-2);
						bulletTimer.restart();
					}
					else if((ev.key.code == sf::Keyboard::Up) && !inJump)
					{
						inJump = true;
						gravityTimer.restart();
					}
				}
			}
		}
		
		//Player's character movement handling
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && (playerSprite.getPosition().x+playerSprite.getLocalBounds().width < divider.getPosition().x))
		{
			playerSprite.move(MOVSPEED*delta*1.5,0);
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && (playerSprite.getPosition().x > 20))
		{
			playerSprite.move(-MOVSPEED*delta*1.5,0);
		}
		
		//Enemy movement handling
		if(enemyMovClock.getElapsedTime().asSeconds() >= 1)
		{
			enemyMovDir = std::rand()%3;
			enemyMovClock.restart();
		}
		
		//If 0, move left
		if((enemyMovDir == 0) && (enemySprite.getPosition().x-enemySprite.getLocalBounds().width > divider.getPosition().x))
		{
			enemySprite.move(-MOVSPEED*delta*1.5, 0);
		}
		//If 1, move right
		else if((enemyMovDir == 1) && (enemySprite.getPosition().x < WIDTH-20))
		{
			enemySprite.move(MOVSPEED*delta*1.5, 0);
		}
		
		//Player jump handling
		if(inJump)
		{
			float elapsedTime = gravityTimer.getElapsedTime().asSeconds();
			if(elapsedTime <= jumpHeight/100.0)
			{
				playerSprite.move(0, -upDownVelocity*delta);
			}
			else if(elapsedTime > jumpHeight/100.0 && elapsedTime <= 2*(jumpHeight/100.0))
			{
				playerSprite.move(0, upDownVelocity*delta);
			}
			else
			{
				inJump = false;
				playerSprite.setPosition(playerSprite.getPosition().x, yPosChar);
				gravityTimer.restart();
			}
		}
		
		//Enemy jump handling (dodge bullets)
		if(shouldJump && !enemyInJump)
		{
			enemyInJump = true;
			shouldJump = false;
			enemyGravityTimer.restart();
		}
		if(enemyInJump)
		{
			float elapsedTime = enemyGravityTimer.getElapsedTime().asSeconds();
			if(elapsedTime <= jumpHeight/100.0)
			{
				enemySprite.move(0, -upDownVelocity*delta);
			}
			else if(elapsedTime > jumpHeight/100.0 && elapsedTime <= 2*(jumpHeight/100.0))
			{
				enemySprite.move(0, upDownVelocity*delta);
			}
			else
			{
				enemyInJump = false;
				shouldJump = false;
				enemySprite.setPosition(enemySprite.getPosition().x, yPosChar);
				enemyGravityTimer.restart();
			}
		}
		
		//Player bullet handling
		for(int i = 0; i < bulletVector.size(); i++)
		{
			bulletVector[i].move(MOVSPEED*4*delta, 0);
			
			float bulletXPos = bulletVector[i].getPosition().x;
			int dodgeBullet = std::rand()%100;
			
			if((bulletXPos > WIDTH*0.5) && (bulletXPos < enemySprite.getPosition().x-enemySprite.getLocalBounds().width) && dodgeBullet <= 70)
			{
				shouldJump = true;
			}
			else if(bulletXPos > WIDTH)
			{
				bulletVector.erase(bulletVector.begin()+i);
			}
			else if(Collision::CircleTest(bulletVector[i], enemySprite))
			{
				enemyHP -= 10;
				hpBarEnemy.setSize(sf::Vector2f(enemyHP*2,15));
				bulletVector.erase(bulletVector.begin()+i);
				if(enemyHP == 0)
					return 1;
			}
		}
		
		//Enemy bullet handling
		if(enemyBulletTimer.getElapsedTime().asSeconds() >= 2)
		{
			enemyBulletVector.push_back(bullet);
			enemyBulletVector.back().setPosition(enemySprite.getPosition().x-enemySprite.getLocalBounds().width*0.5,
														enemySprite.getPosition().y-enemySprite.getLocalBounds().height*0.5-2);
			enemyBulletTimer.restart();
		}
		for(int i = 0; i < enemyBulletVector.size(); i++)
		{
			enemyBulletVector[i].move(-MOVSPEED*4*delta, 0);
			if(enemyBulletVector[i].getPosition().x < 0)
			{
				enemyBulletVector.erase(enemyBulletVector.begin()+i);
			}
			else if(Collision::CircleTest(enemyBulletVector[i], playerSprite))
			{
				hp -= 10;
				hpBarPlayer.setSize(sf::Vector2f(hp*2,15));
				enemyBulletVector.erase(enemyBulletVector.begin()+i);
				if(hp == 0)
					return 0;
			}
		}
		
		//Draw stuff on the screen
		window->clear(sf::Color::Black);
		window->draw(bStageSprite);
		for(int i = 0; i < bulletVector.size(); i++)
		{
			window->draw(bulletVector[i]);
		}
		for(int i = 0; i < enemyBulletVector.size(); i++)
		{
			window->draw(enemyBulletVector[i]);
		}
		window->draw(divider);
		window->draw(bFrameSprite);
		window->draw(playerSprite);
		window->draw(enemySprite);
		window->draw(playerText);
		window->draw(enemyText);
		window->draw(hpBarPlayer);
		window->draw(hpBarEnemy);
		window->display();
	}
	
	return 0;
}

//Loads the required items (files)
bool loadStuff()
{
	//Load the textures
	if(!bFrameTex.loadFromFile("sprites/battlestage.png"))
		return false;
	if(!bStageTex.loadFromFile("sprites/battlebg.png"))
		return false;
	if(!smallCharacters.loadFromFile("sprites/charsmall.png"))
		return false;
	if(!bigCharacters.loadFromFile("sprites/charsbig.png"))
		return false;
	if(!itemsTex.loadFromFile("sprites/items.png"))
		return false;
	//--//
	
	//Load the fonts
	if(!font.loadFromFile("fonts/CrusoeText-Regular.ttf"))
		return false;
	//--//
	return true;
}

//Sets the sprites to their respective textures and returns the Player character
void setSprites(Character &Player, Players whichPlayer)
{
	shadyBig.setTexture(bigCharacters);
	ninjaBig.setTexture(bigCharacters);
	devilBig.setTexture(bigCharacters);
	cyborgBig.setTexture(bigCharacters);
	blueBig.setTexture(bigCharacters);
	enemy.setTexture(smallCharacters);
	player.setTexture(smallCharacters);
	bStageSprite.setTexture(bStageTex);
	bFrameSprite.setTexture(bFrameTex);
	bullet.setTexture(itemsTex);
	candyPurple.setTexture(itemsTex);
	sweet.setTexture(itemsTex);
	candyYellow.setTexture(itemsTex);
	healthPotion.setTexture(itemsTex);
	
	shadyBig.setTextureRect(sf::IntRect(22,27,100,102));
	devilBig.setTextureRect(sf::IntRect(617,18,101,110));
	cyborgBig.setTextureRect(sf::IntRect(325,25,105,102));
	blueBig.setTextureRect(sf::IntRect(471,29,96,98));
	ninjaBig.setTextureRect(sf::IntRect(163,9,112,118));
	bullet.setTextureRect(sf::IntRect(3,5,27,26));
	candyPurple.setTextureRect(sf::IntRect(34,2,24,24));
	sweet.setTextureRect(sf::IntRect(64,2,28,28));
	candyYellow.setTextureRect(sf::IntRect(100,2,24,24));
	healthPotion.setTextureRect(sf::IntRect(131,2,23,28));
	
	CandyItemP.setItem(candyPurple, 0);
	CandyItemY.setItem(candyYellow, 1);
	SweetItem.setItem(sweet, 2);
	hpPotionItem.setItem(healthPotion, -1);
	
	player.setPosition(35,35);
	
	//Character Salah coordinates
	sf::IntRect salahRight(3,8,32,29);
	sf::IntRect salahLeft(5,54,32,36);
	//--//
	//Character Moeez coordinates
	sf::IntRect moeezRight(141,10,32,29);
	sf::IntRect moeezLeft(143,56,33,30);
	//--//
	
	switch(whichPlayer)
	{
		case SALAH:
			Player.setCharacter(player, salahLeft, salahRight);
			break;
		case MOEEZ:
			Player.setCharacter(player, moeezLeft, moeezRight);
			break;
	}
}

int main()
{
	std::srand(std::time(NULL));
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT, BPP), "Sweet Run 2");
	window.setVerticalSyncEnabled(true);
	
	//Load the textures and fonts
	if(!loadStuff())
		return 1;
		
	Character Player;
	int hp = 100;
	Players plr = SALAH;
	setSprites(Player, plr);
	
	//Set up containers and stuff to hold level info
	std::vector<std::string> levelVector = getLevelList("levellist.gcf");
	short level = 0;
	LevelParser lvlParser;
	lvlParser.setLevelFile(levelVector[level]);
	lvlParser.parseFile();
	//--//
	
	//Set the initial level
	sf::Texture stageTexture;
	sf::Sprite stageSprite;
	setStage(&stageTexture, lvlParser.getStageFile());
	updateStage(&stageTexture, &stageSprite);
	//--//
	
	//Get the enemy vector
	std::vector<Character> enemyVector = getEnemies(&lvlParser);
	
	//Get the item vectors
	std::vector<Item> itemVector = getItems(&lvlParser);
	
	//Game framerate control
	sf::Clock frameTimer;
	float delta;
	//--//
	
	//Score and HP display
	unsigned int score = 0;
	sf::Text hpText;
	sf::Text scoreText;
	
	hpText.setString("HP: "+toStr(hp)+"%");
	hpText.setFont(font);
	hpText.setCharacterSize(12);
	hpText.setColor(sf::Color::Blue);
	
	scoreText.setString("Score: "+toStr(score));
	scoreText.setFont(font);
	scoreText.setCharacterSize(12);
	scoreText.setColor(sf::Color::Blue);
	
	hpText.setPosition(0,0);
	scoreText.setPosition(0,hpText.getPosition().y+hpText.getLocalBounds().height);
	//--//
	
	while(window.isOpen())
	{
		delta = frameTimer.restart().asSeconds();
		sf::Event e;
		while(window.pollEvent(e))
		{
			switch(e.type)
			{
				case sf::Event::Closed:
					window.close();
					break;
			}
		}
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			if(Player.getDirection() == LEFT)
			{
				Player.changeDirection(RIGHT);
			}
			Player.move(MOVSPEED*delta,0);
			if(Player.isColliding(&stageSprite))
				Player.move(-MOVSPEED*delta,0);
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			if(Player.getDirection() == RIGHT)
			{
				Player.changeDirection(LEFT);
			}
			Player.move(-MOVSPEED*delta,0);
			if(Player.isColliding(&stageSprite))
				Player.move(MOVSPEED*delta,0);
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			Player.move(0,-MOVSPEED*delta);
			if(Player.isColliding(&stageSprite))
				Player.move(0,MOVSPEED*delta);
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			Player.move(0,MOVSPEED*delta);
			if(Player.isColliding(&stageSprite))
				Player.move(0,-MOVSPEED*delta);
		}
		
		window.clear(sf::Color::White);
		window.draw(stageSprite);
		Player.draw(&window);
		for(int i = 0; i < itemVector.size(); i++)
		{
			if(itemVector[i].checkCollision(Player))
			{
				int itemIndex = itemVector[i].getIndex();
				switch(itemIndex)
				{
					case -1:
					{
						hp = 100;
						hpText.setString("HP: "+toStr(hp)+"%");
						break;
					}
					case 0:
					{
						score += 5;
						break;
					}
					case 1:
					{
						score += 10;
						break;
					}
					case 2:
					{
						score += 15;
						break;
					}
				}
				itemVector.erase(itemVector.begin()+i);
				scoreText.setString("Score: "+toStr(score));
			}
			itemVector[i].display(window);
		}
		for(int i = 0; i < enemyVector.size(); i++)
		{
			int battleResult;
			enemyVector[i].draw(&window);
			if(Player.isColliding(&enemyVector[i]))
			{
				battleResult = battleStage(&window, &frameTimer, plr, enemyVector[i].getName(), hp);
				hpText.setString("HP: "+toStr(hp)+"%");
				if(battleResult == 0)
					window.close(); //Game over screen
				else if(battleResult == 1)
					enemyVector.erase(enemyVector.begin()+i);
			}
		}
		window.draw(hpText);
		window.draw(scoreText);
		window.display();
	}
	
	return 0;
}