#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "LevelParser.h"
#include "Collision.h"
#include "Character.h"
#include "Item.h"
#include <iostream>
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
const int MAXHP = 100;
enum Players {SALAH, MOEEZ};
std::string buildNumber("Copyright(c) 2014 - Ahnaf Tahmid. Build 170714.3");

//Textures, fonts, sounds and sprites
sf::Texture bFrameTex;
sf::Texture bStageTex;
sf::Texture itemsTex;
sf::Texture backgroundTexture;
sf::Texture menuBgTex;
sf::Texture smallCharacters;
sf::Texture bigCharacters;
sf::Font font;
sf::Sprite backgroundSprite;
sf::Sprite player;
sf::Sprite enemy;
sf::Sprite shadyBig;
sf::Sprite ninjaBig;
sf::Sprite cyborgBig;
sf::Sprite blueBig;
sf::Sprite devilBig;
sf::Sprite bossBig;
sf::Sprite bFrameSprite;
sf::Sprite bStageSprite;
sf::Sprite bullet;
sf::Sprite candyPurple;
sf::Sprite sweet;
sf::Sprite candyYellow;
sf::Sprite healthPotion;
sf::Music menuMusic;
sf::Music battleMusic;
sf::Music bgMusic;
sf::Music overMusic;
sf::Music storyMusic;
sf::SoundBuffer hitSB;
sf::SoundBuffer fightSB;
sf::SoundBuffer lcSB;
sf::SoundBuffer ocSB;
sf::SoundBuffer pickSB;
sf::SoundBuffer dvSB;
sf::Sound hitSound;
sf::Sound fightVoice;
sf::Sound levelcompleteSound;
sf::Sound optionSound;
sf::Sound pickSound;
sf::Sound demonVoice;
Item CandyItemP;
Item CandyItemY;
Item SweetItem;
Item hpPotionItem;
bool playSound = true;

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
int setStage(sf::Texture &bgTex, std::string stagePath)
{
	if(!bgTex.loadFromFile(stagePath))
		return 1;
	else
		return 0;
}

//Updates the stage sprite
void updateStage(sf::Texture &bgTex, sf::Sprite &bgSpr)
{
	bgSpr.setTexture(bgTex);
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

//Character chooser and story line
int storyLine(sf::RenderWindow &window, Players* playerE)
{
	sf::Texture scrollTex;
	if(!scrollTex.loadFromFile("sprites/scroll.png"))
		return 1;
	
	sf::Sprite scrollSprite;
	scrollSprite.setTexture(scrollTex);
	
	sf::Sprite bgSprite;
	bgSprite.setTexture(menuBgTex);
	
	std::string story("Welcome young demon warrior. Thy king hath called thou as duty hath called.\nThy realm of Sweetria hath been infiltrated by the Demons from thy Artificial Hell\nPit.Their motives aren't certain but we believe they want to stealeth thy sweet\nsupplies.Then they shalt let their leader Gerefeis The Dark Lord in thy realm to\ncorrupt it.We can't let this happen. So thou shalt get into thy realm and slayeth any\ndemon that gets in thou's way. While thou is at it, collect all the sweets.\n\nThy king hath written thy instructions for thou: \nNormal levels:\nMovement = Arrow Keys\n\nBattle stage:\nMovement: Left/Right arrow keys\nJump: Up arrow key\nHit: Space\n\n\t\tMay good luck be with thou, young demon warrior!");
	
	int storyLength = story.length();
	int nchar = 0;
	
	sf::Text storyText;
	sf::Text headingText;
	sf::Text moeezText;
	sf::Text salahText;
	
	storyText.setFont(font);
	storyText.setCharacterSize(15);
	storyText.setColor(sf::Color::Black);
	storyText.setPosition(80,85);
	
	headingText.setFont(font);
	headingText.setCharacterSize(50);
	headingText.setColor(sf::Color::Black);
	headingText.setString("Choose character: ");
	headingText.setOrigin(headingText.getLocalBounds().width*0.5, headingText.getLocalBounds().height*0.5);
	headingText.setPosition(WIDTH*0.5, HEIGHT*0.5-100);
	
	moeezText.setFont(font);
	moeezText.setColor(sf::Color::Black);
	moeezText.setCharacterSize(18);
	moeezText.setString("Moeez");
	
	salahText.setFont(font);
	salahText.setColor(sf::Color::Black);
	salahText.setCharacterSize(18);
	salahText.setString("Salah");
	
	
	sf::Sprite charSpriteOne;
	sf::Sprite charSpriteTwo;
	charSpriteOne.setTexture(bigCharacters);
	charSpriteTwo.setTexture(bigCharacters);
	charSpriteOne.setTextureRect(sf::IntRect(22,27,100,102));
	charSpriteTwo.setTextureRect(sf::IntRect(163,9,112,118));
	
	charSpriteOne.setPosition(142, 280);
	charSpriteTwo.setPosition(532, charSpriteOne.getPosition().y-5);
	salahText.setPosition(charSpriteOne.getPosition().x+25, charSpriteOne.getPosition().y+charSpriteOne.getLocalBounds().height+5);
	moeezText.setPosition(charSpriteTwo.getPosition().x+25, charSpriteTwo.getPosition().y+charSpriteTwo.getLocalBounds().height+5);
	
	sf::RectangleShape chooser;
	chooser.setSize(sf::Vector2f(120,150));
	chooser.setFillColor(sf::Color::Transparent);
	chooser.setOutlineThickness(2);
	chooser.setOutlineColor(sf::Color::Red);
	chooser.setPosition(charSpriteOne.getPosition().x-5, charSpriteOne.getPosition().y-10);
	int chooserPos = 0;
	
	int menuState = 0;
	
	sf::Text playText;
	playText.setFont(font);
	playText.setString("Play now");
	playText.setColor(sf::Color::Black);
	playText.setCharacterSize(30);
	playText.setPosition(585,455);
	
	sf::Clock storyClock;
	
	if(playSound)
		storyMusic.play();
	
	while(window.isOpen())
	{
		sf::Event ev;
		while(window.pollEvent(ev))
		{
			switch(ev.type)
			{
				case sf::Event::Closed:
				{
					storyMusic.stop();
					return 1;
					break;
				}
				case sf::Event::KeyPressed:
				{
					if(ev.key.code == sf::Keyboard::Right && chooserPos == 0 && menuState == 0)
					{
						if(playSound)
							optionSound.play();
						chooserPos = 1;
						chooser.setPosition(charSpriteTwo.getPosition().x, charSpriteTwo.getPosition().y);
					}
					else if(ev.key.code == sf::Keyboard::Left && chooserPos == 1 && menuState == 0)
					{
						if(playSound)
							optionSound.play();
						chooserPos = 0;
						chooser.setPosition(charSpriteOne.getPosition().x-5, charSpriteOne.getPosition().y-10);
					}
					else if(ev.key.code == sf::Keyboard::Return && menuState == 0)
					{
						switch(chooserPos)
						{
							case 0:
								*playerE = SALAH;
								break;
							case 1:
								*playerE = MOEEZ;
								break;
						}
						
						chooser.setSize(sf::Vector2f(playText.getLocalBounds().width, playText.getLocalBounds().height+12));
						chooser.setPosition(playText.getPosition().x, playText.getPosition().y);
						menuState = 1;
					}
					else if(ev.key.code == sf::Keyboard::Return && menuState == 1)
					{
						storyMusic.stop();
						return 0;
					}
					break;
				}
			}
		}
		
		if(menuState == 0)
		{
			window.clear(sf::Color::Black);
			window.draw(bgSprite);
			window.draw(scrollSprite);
			window.draw(headingText);
			window.draw(charSpriteOne);
			window.draw(charSpriteTwo);
			window.draw(moeezText);
			window.draw(salahText);
			window.draw(chooser);
			window.display();
		}
		else if(menuState == 1)
		{
			if((storyClock.getElapsedTime().asSeconds() > 0.06) && (nchar < storyLength))
			{
				nchar++;
				storyText.setString(story.substr(0,nchar));
				storyClock.restart();
			}
		
			window.clear(sf::Color::Black);
			window.draw(bgSprite);
			window.draw(scrollSprite);
			window.draw(storyText);
			window.draw(playText);
			window.draw(chooser);
			window.display();
		}
	}
	
	return 1;
}

//The main menu
int menuScreen(sf::RenderWindow &window)
{
	sf::Texture headingTex;
	if(!headingTex.loadFromFile("sprites/heading.PNG"))
		return 1;
	
	sf::Sprite bgSprite;
	sf::Sprite headingSprite;
	bgSprite.setTexture(menuBgTex);
	headingSprite.setTexture(headingTex);
	headingSprite.setOrigin(headingSprite.getLocalBounds().width*0.5,0);
	headingSprite.setPosition(WIDTH*0.5,50);
	
	sf::Text playText;
	sf::Text optionText;
	sf::Text exitTxt;
	sf::Text soundOnText;
	sf::Text soundOffText;
	
	playText.setFont(font);
	optionText.setFont(font);
	exitTxt.setFont(font);
	soundOnText.setFont(font);
	soundOffText.setFont(font);
	
	playText.setString("Play game");
	optionText.setString("Options");
	exitTxt.setString("Exit game");
	soundOnText.setString("On");
	soundOffText.setString("Off");
	
	playText.setColor(sf::Color::Black);
	optionText.setColor(sf::Color::Black);
	exitTxt.setColor(sf::Color::Black);
	soundOnText.setColor(sf::Color::Black);
	soundOffText.setColor(sf::Color::Black);
	
	playText.setCharacterSize(38);
	optionText.setCharacterSize(38);
	exitTxt.setCharacterSize(38);
	soundOnText.setCharacterSize(38);
	soundOffText.setCharacterSize(38);
	
	playText.setOrigin(playText.getLocalBounds().width*0.5, playText.getLocalBounds().height*0.5);
	optionText.setOrigin(optionText.getLocalBounds().width*0.5, optionText.getLocalBounds().height*0.5);
	exitTxt.setOrigin(exitTxt.getLocalBounds().width*0.5, exitTxt.getLocalBounds().height*0.5);
	soundOnText.setOrigin(soundOnText.getLocalBounds().width*0.5, soundOnText.getLocalBounds().height*0.5);
	soundOffText.setOrigin(soundOffText.getLocalBounds().width*0.5, soundOffText.getLocalBounds().height*0.5);

	playText.setPosition(WIDTH*0.5,230);
	optionText.setPosition(WIDTH*0.5,playText.getPosition().y+playText.getLocalBounds().height*2);
	exitTxt.setPosition(WIDTH*0.5,optionText.getPosition().y+optionText.getLocalBounds().height*2);
	soundOnText.setPosition(WIDTH*0.5-soundOnText.getLocalBounds().width,optionText.getPosition().y);
	soundOffText.setPosition(WIDTH*0.5+soundOffText.getLocalBounds().width,soundOnText.getPosition().y);
	
	sf::Vector2f chooserPositions[3];
	chooserPositions[0] = sf::Vector2f(playText.getPosition().x, playText.getPosition().y+playText.getLocalBounds().height*0.5-2);
	chooserPositions[1] = sf::Vector2f(optionText.getPosition().x, optionText.getPosition().y+optionText.getLocalBounds().height*0.5-2);
	chooserPositions[2] = sf::Vector2f(exitTxt.getPosition().x, exitTxt.getPosition().y+exitTxt.getLocalBounds().height*0.5-2);
	
	sf::Vector2f soundPositions[2];
	sf::FloatRect soLB = soundOnText.getLocalBounds();
	sf::FloatRect sofLB = soundOffText.getLocalBounds();
	soundPositions[0] = sf::Vector2f(soundOnText.getPosition().x+soLB.width+soLB.width*0.5-4, soundOnText.getPosition().y+soLB.height*0.5-4);
	soundPositions[1] = sf::Vector2f(soundOffText.getPosition().x+sofLB.width+7, soundOffText.getPosition().y+sofLB.height*0.5-4);
	
	short soundOption = 0;
	int option = 0;
	
	sf::Vector2f menuChooserSize(226,55);
	sf::Vector2f soundChooserSize(soundOffText.getLocalBounds().width+4,55);
	
	sf::RectangleShape chooser;
	chooser.setSize(menuChooserSize);
	chooser.setFillColor(sf::Color::Transparent);
	chooser.setOutlineThickness(3);
	chooser.setOutlineColor(sf::Color(176,0,0));
	chooser.setOrigin(chooser.getLocalBounds().width*0.5, chooser.getLocalBounds().height*0.5);
	chooser.setPosition(chooserPositions[option]);
	
	sf::Text buildText;
	buildText.setFont(font);
	buildText.setCharacterSize(14);
	buildText.setColor(sf::Color::Black);
	buildText.setString(buildNumber);
	buildText.setOrigin(0,buildText.getLocalBounds().height);
	buildText.setPosition(0,HEIGHT-10);
	
	bool showSoundMenu = false;
	
	menuMusic.play();
	
	while(window.isOpen())
	{
		sf::Event ev;
		while(window.pollEvent(ev))
		{
			switch(ev.type)
			{
				case sf::Event::Closed:
				{
					menuMusic.stop();
					window.close();
					break;
				}
				case sf::Event::KeyPressed:
				{
					if(ev.key.code == sf::Keyboard::Up && option > 0)
					{
						option--;
						chooser.setPosition(chooserPositions[option]);
						if(playSound)
							optionSound.play();
					}
					else if(ev.key.code == sf::Keyboard::Down && option < 2)
					{
						option++;
						chooser.setPosition(chooserPositions[option]);
						if(playSound)
							optionSound.play();
					}
					else if(ev.key.code == sf::Keyboard::Left && soundOption == 1 && showSoundMenu)
					{
						soundOption = 0;
						chooser.setPosition(soundPositions[soundOption]);
						if(playSound)
							optionSound.play();
					}
					else if(ev.key.code == sf::Keyboard::Right && soundOption == 0 && showSoundMenu)
					{
						soundOption = 1;
						chooser.setPosition(soundPositions[soundOption]);
						if(playSound)
							optionSound.play();
					}
					else if(ev.key.code == sf::Keyboard::Return && showSoundMenu)
					{
						if(soundOption == 0)
						{
							playSound = true;
							menuMusic.play();
						}
						else
						{
							playSound = false;
							menuMusic.stop();
						}
						showSoundMenu = false;
						chooser.setSize(menuChooserSize);
						chooser.setPosition(chooserPositions[option]);
					}
					else if(ev.key.code == sf::Keyboard::Return && option != 1)
					{
						menuMusic.stop();
						return option;
					}
					else if(ev.key.code == sf::Keyboard::Return && option == 1)
					{
						showSoundMenu = true;
						chooser.setSize(soundChooserSize);
						chooser.setPosition(soundPositions[soundOption]);
					}
					break;
				}
			}
		}
		
		window.clear(sf::Color::Black);
		window.draw(bgSprite);
		window.draw(headingSprite);
		window.draw(buildText);
		if(showSoundMenu)
		{
			window.draw(soundOnText);
			window.draw(soundOffText);
			window.draw(chooser);
			window.display();
			continue;
		}
		window.draw(playText);
		window.draw(optionText);
		window.draw(exitTxt);
		window.draw(chooser);
		window.display();
	}
	
	return 0;
}

//The battle stage
int battleStage(sf::RenderWindow *window, sf::Clock *frameClock, Players plr, std::string enemyName, int &hp)
{
	//Set up the HP labels
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
	
	//Enemy HP
	int enemyHP = MAXHP;
	if(enemyName == "Boss")
		enemyHP += 100;
	
	//Player HP bar
	sf::RectangleShape hpBarPlayer;
	hpBarPlayer.setSize(sf::Vector2f(hp*2,15));
	hpBarPlayer.setFillColor(sf::Color::Red);
	hpBarPlayer.setPosition(playerText.getPosition().x+playerText.getLocalBounds().width, playerText.getPosition().y);
	
	//Enemy HP bar
	sf::RectangleShape hpBarEnemy;
	hpBarEnemy.setSize(sf::Vector2f(enemyHP*2,15));
	hpBarEnemy.setFillColor(sf::Color::Yellow);
	hpBarEnemy.setPosition(enemyText.getPosition().x+enemyText.getLocalBounds().width+3, enemyText.getPosition().y);
	
	sf::Sprite playerSprite;
	sf::Sprite enemySprite;
	
	//The divider (invisible) - separates the player from the enemy
	sf::RectangleShape divider;
	divider.setSize(sf::Vector2f(1,HEIGHT));
	divider.setOrigin(divider.getLocalBounds().width*0.5, divider.getLocalBounds().height*0.5);
	divider.setPosition(WIDTH*0.5, HEIGHT*0.5);
	divider.setFillColor(sf::Color::Transparent);
	
	//Set the player sprite
	switch(plr)
	{
		case SALAH:
			playerSprite = shadyBig;
			break;
		case MOEEZ:
			playerSprite = ninjaBig;
			break;
	}
	
	//Set the enemy sprite
	if(enemyName == "Devil")
		enemySprite = devilBig;
	else if(enemyName == "Blue")
		enemySprite = blueBig;
	else if(enemyName == "Cyborg")
		enemySprite = cyborgBig;
	else if(enemyName == "Boss")
		enemySprite = bossBig;
	
	//Set the character positions
	playerSprite.setOrigin(0, playerSprite.getLocalBounds().height);
	enemySprite.setOrigin(enemySprite.getLocalBounds().width, enemySprite.getLocalBounds().height);
	int yPosChar = 480;
	playerSprite.setPosition(50,yPosChar);
	enemySprite.setPosition(760,yPosChar);
	
	float delta;
	
	//Used for moving the enemy
	sf::Clock enemyMovClock;
	int enemyMovDir = 0;
	
	//Used for controlling the bullets
	std::vector<sf::Sprite> bulletVector;
	sf::Clock bulletTimer;
	
	//Same as above but for enemy bullets
	std::vector<sf::Sprite> enemyBulletVector;
	sf::Clock enemyBulletTimer;
	
	//Used to handle jump (player)
	sf::Clock gravityTimer;
	bool inJump = false;
	float upDownVelocity = 500;
	int jumpHeight = 25;
	
	//Used to handle jump (enemy)
	sf::Clock enemyGravityTimer;
	bool enemyInJump = false;
	bool shouldJump = false;
	
	//Used to display the "FIGHT! message"
	sf::Text fightText;
	fightText.setFont(font);
	if(!(enemyName == "Boss"))
		fightText.setString("FIGHT!");
	else
		fightText.setString("BOSS FIGHT!");
	fightText.setCharacterSize(50);
	fightText.setColor(sf::Color::Red);
	fightText.setOrigin(fightText.getLocalBounds().width*0.5, fightText.getLocalBounds().height*0.5);
	fightText.setPosition(WIDTH*0.5, HEIGHT*0.5);
	
	sf::Clock fightClock;
	
	if(playSound)
	{
		if(!(enemyName == "Boss"))
			fightVoice.play();
		else
			demonVoice.play();
		battleMusic.play();
	}
	
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
					battleMusic.stop();
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
					break;
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
		if((enemyMovDir == 0) && (enemySprite.getPosition().x-enemySprite.getLocalBounds().width > divider.getPosition().x) && !enemyInJump)
		{
			enemySprite.move(-MOVSPEED*delta*1.5, 0);
		}
		//If 1, move right
		else if((enemyMovDir == 1) && (enemySprite.getPosition().x < WIDTH-20) && !enemyInJump)
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
				if(playSound)
					hitSound.play();
				enemyHP -= 10;
				hpBarEnemy.setSize(sf::Vector2f(enemyHP*2,15));
				bulletVector.erase(bulletVector.begin()+i);
				if(enemyHP == 0)
				{	
					battleMusic.stop();
					return 1;
				}
			}
		}
		
		bool enemyBulletTimerBool;
		if(!(enemyName == "Boss"))
			enemyBulletTimerBool = enemyBulletTimer.getElapsedTime().asSeconds() >= 2;
		else
			enemyBulletTimerBool = enemyBulletTimer.getElapsedTime().asSeconds() >= 1;
		
		//Enemy bullet handling
		if(enemyBulletTimerBool)
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
				if(playSound)
					hitSound.play();
				hp -= 10;
				hpBarPlayer.setSize(sf::Vector2f(hp*2,15));
				enemyBulletVector.erase(enemyBulletVector.begin()+i);
				if(hp == 0)
				{
					battleMusic.stop();
					return 0;
				}
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
		if(fightClock.getElapsedTime().asSeconds() <= 2)
		{
			window->draw(fightText);
		}
		window->display();
	}
	
	return 0;
}

//The gameover screen
int gameOverScreen(sf::RenderWindow *window, unsigned int &score, int flag)
{
	int option = 0;
	
	sf::Text detailsText;
	detailsText.setString("Score: "+toStr(score));
	detailsText.setFont(font);
	detailsText.setCharacterSize(30);
	detailsText.setPosition(0,0);
	detailsText.setColor(sf::Color::Black);
	
	sf::Text overText;
	if(flag == 0)
		overText.setString("Game Over!");
	else if(flag == 1)
		overText.setString("Game Cleared!");
	overText.setFont(font);
	overText.setCharacterSize(42);
	overText.setColor(sf::Color::Black);
	overText.setOrigin(overText.getLocalBounds().width*0.5, overText.getLocalBounds().height*0.5);
	overText.setPosition(WIDTH*0.5, HEIGHT*0.5);
	
	sf::Text playAgainText;
	sf::Text exitText;
	
	playAgainText.setFont(font);
	exitText.setFont(font);
	
	playAgainText.setColor(sf::Color::Black);
	exitText.setColor(sf::Color::Black);
	
	playAgainText.setString("Play again");
	exitText.setString("Exit game");
	
	playAgainText.setCharacterSize(36);
	exitText.setCharacterSize(36);
	
	sf::FloatRect playBounds = playAgainText.getLocalBounds();
	sf::FloatRect exitBounds = exitText.getLocalBounds();
	
	playAgainText.setOrigin(playBounds.width*0.5, playBounds.height*0.5);
	exitText.setOrigin(exitBounds.width*0.5, exitBounds.height*0.5);
	playAgainText.setPosition(WIDTH*0.5, HEIGHT*0.5+playBounds.height*3);
	exitText.setPosition(WIDTH*0.5, playAgainText.getPosition().y+playBounds.height*0.5+exitBounds.height+10);
	
	sf::RectangleShape chooser;
	chooser.setFillColor(sf::Color::Transparent);
	chooser.setOutlineColor(sf::Color::Red);
	chooser.setOutlineThickness(3);
	chooser.setSize(sf::Vector2f(playBounds.width+8, playBounds.height+20));
	chooser.setOrigin(chooser.getLocalBounds().width*0.5, chooser.getLocalBounds().height*0.5);
	chooser.setPosition(playAgainText.getPosition().x+3, playAgainText.getPosition().y+playBounds.height*0.5-5);
	
	if(playSound)
		overMusic.play();
	
	while(window->isOpen())
	{
		sf::Event ev;
		while(window->pollEvent(ev))
		{
			switch(ev.type)
			{
				case sf::Event::Closed:
				{
					overMusic.stop();
					return -1;
					break;
				}
				case sf::Event::KeyReleased:
				{
					if((ev.key.code == sf::Keyboard::Down) && option == 0)
					{
						if(playSound)
							optionSound.play();
						chooser.setPosition(exitText.getPosition().x+3, exitText.getPosition().y+exitBounds.height*0.5-5);
						option = 1;
					}
					else if((ev.key.code == sf::Keyboard::Up) && option == 1)
					{
						if(playSound)
							optionSound.play();
						chooser.setPosition(playAgainText.getPosition().x+3, playAgainText.getPosition().y+playBounds.height*0.5-5);
						option = 0;
					}
					else if(ev.key.code == sf::Keyboard::Return)
					{
						overMusic.stop();
						return option;
					}
					break;
				}
			}
		}
		
		window->clear(sf::Color::White);
		window->draw(detailsText);
		window->draw(overText);
		window->draw(playAgainText);
		window->draw(exitText);
		window->draw(chooser);
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
	if(!backgroundTexture.loadFromFile("sprites/mainbg.png"))
		return false;
	if(!menuBgTex.loadFromFile("sprites/menubg.png"))
		return false;
	//--//
	
	//Load the sounds
	if(!menuMusic.openFromFile("sounds/menumusic.ogg"))
		return false;
	if(!battleMusic.openFromFile("sounds/battlemusic.ogg"))
		return false;
	if(!bgMusic.openFromFile("sounds/ingamemusic.ogg"))
		return false;
	if(!overMusic.openFromFile("sounds/gameovermusic.ogg"))
		return false;
	if(!storyMusic.openFromFile("sounds/storylinemusic.ogg"))
		return false;
	if(!hitSB.loadFromFile("sounds/hit.wav"))
		return false;
	if(!fightSB.loadFromFile("sounds/fight.ogg"))
		return false;
	if(!lcSB.loadFromFile("sounds/levelcomplete.wav"))
		return false;
	if(!ocSB.loadFromFile("sounds/optionchange.wav"))
		return false;
	if(!pickSB.loadFromFile("sounds/pick.wav"))
		return false;
	if(!dvSB.loadFromFile("sounds/demonvoice.ogg"))
		return false;
	//--//
	
	//Load the fonts
	if(!font.loadFromFile("fonts/CrusoeText-Regular.ttf"))
		return false;
	//--//
	return true;
}

//Set the sounds to its buffer and let the loops of music
void setMusic()
{
	menuMusic.setLoop(true);
	battleMusic.setLoop(true);
	bgMusic.setLoop(true);
	overMusic.setLoop(true);
	storyMusic.setLoop(true);
	hitSound.setBuffer(hitSB);
	fightVoice.setBuffer(fightSB);
	levelcompleteSound.setBuffer(lcSB);
	optionSound.setBuffer(ocSB);
	pickSound.setBuffer(pickSB);
	demonVoice.setBuffer(dvSB);
}

//Sets the sprites to their respective textures and returns the Player character
void setSprites(Character &Player, Players whichPlayer)
{
	shadyBig.setTexture(bigCharacters);
	ninjaBig.setTexture(bigCharacters);
	devilBig.setTexture(bigCharacters);
	cyborgBig.setTexture(bigCharacters);
	blueBig.setTexture(bigCharacters);
	bossBig.setTexture(bigCharacters);
	enemy.setTexture(smallCharacters);
	player.setTexture(smallCharacters);
	bStageSprite.setTexture(bStageTex);
	bFrameSprite.setTexture(bFrameTex);
	bullet.setTexture(itemsTex);
	candyPurple.setTexture(itemsTex);
	sweet.setTexture(itemsTex);
	candyYellow.setTexture(itemsTex);
	healthPotion.setTexture(itemsTex);
	backgroundSprite.setTexture(backgroundTexture);
	
	shadyBig.setTextureRect(sf::IntRect(22,27,100,102));
	devilBig.setTextureRect(sf::IntRect(617,18,101,110));
	cyborgBig.setTextureRect(sf::IntRect(325,25,105,102));
	blueBig.setTextureRect(sf::IntRect(471,29,96,98));
	ninjaBig.setTextureRect(sf::IntRect(163,9,112,118));
	bossBig.setTextureRect(sf::IntRect(763,20,96,105));
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
	setMusic();
	
	int menuReturnVal = menuScreen(window);
	Players plr;
	
	switch(menuReturnVal)
	{
		case 0:
		{
			int storyReturnVal = storyLine(window, &plr);
			if(storyReturnVal == 1)
				return 0;
			break;
		}
		case 2:
		{
			return 0;
			break;
		}
	}
	
	Character Player;
	int hp = MAXHP;
	setSprites(Player, plr);
	
	//Set up containers and stuff to hold level info
	std::vector<std::string> levelVector = getLevelList("levellist.gcf");
	short level = 0;
	LevelParser lvlParser;
	lvlParser.setLevelFile(levelVector[level]);
	lvlParser.parseFile();
	//--//
	
	//Set the initial level
	sf::Texture *stageTextures = new sf::Texture[levelVector.size()];
	
	//Fill the stageTextures array
	for(int i = 0; i < levelVector.size(); i++)
	{
		sf::Texture tempTex;
		stageTextures[i] = tempTex;
	}
	
	sf::Sprite stageSprite;
	if(setStage(stageTextures[level], lvlParser.getStageFile()) == 1)
	{
		delete[] stageTextures;
		return 1;
	}
	updateStage(stageTextures[level], stageSprite);
	//--//
	
	//Level name message display stuff
	sf::Clock levelNameDisplayClock; //Used to display "level cleared" message
	sf::Text levelNameText;
	bool displayLevelText = true;
	levelNameText.setFont(font);
	levelNameText.setString(lvlParser.getLevelName());
	levelNameText.setCharacterSize(50);
	levelNameText.setOrigin(levelNameText.getLocalBounds().width*0.5, levelNameText.getLocalBounds().height*0.5);
	levelNameText.setPosition(WIDTH*0.5, HEIGHT*0.5);
	levelNameText.setColor(sf::Color::Red);
	//--//
	
	//"Level cleared" message display stuff
	sf::Clock levelClearClock; //Used to display "level cleared" message
	sf::Text levelClearedText;
	bool displayClearedText = false;
	levelClearedText.setFont(font);
	levelClearedText.setString("Level Cleared!");
	levelClearedText.setCharacterSize(50);
	levelClearedText.setOrigin(levelClearedText.getLocalBounds().width*0.5, levelClearedText.getLocalBounds().height*0.5);
	levelClearedText.setPosition(WIDTH*0.5, HEIGHT*0.5);
	levelClearedText.setColor(sf::Color::Red);
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
	
	hpText.setString("HP: "+toStr((hp/(MAXHP*1.0))*100)+"%");
	hpText.setFont(font);
	hpText.setCharacterSize(20);
	hpText.setColor(sf::Color(255,100,6));
	
	scoreText.setString("Score: "+toStr(score));
	scoreText.setFont(font);
	scoreText.setCharacterSize(20);
	scoreText.setColor(sf::Color(255,100,6));
	
	hpText.setPosition(0,0);
	scoreText.setPosition(0,hpText.getPosition().y+hpText.getLocalBounds().height+2);
	//--//
	
	//Enemy random movement stuff
	sf::Clock enemyClock;
	std::vector<int> enemyMovVect;
	for(int i = 0; i < enemyVector.size(); i++)
	{
		int movDir = std::rand()%3;
		enemyMovVect.push_back(movDir);
	}
	//--//
	
	//Temporary enemy vector
	std::vector<Character> tempEnemyVector = enemyVector;
	
	if(playSound)
		bgMusic.play();
	
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
		
		//Player movement
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
		
		window.clear(sf::Color::Black);
		window.draw(backgroundSprite);
		window.draw(stageSprite);
		Player.draw(&window);
		
		//Item handling
		for(int i = 0; i < itemVector.size();)
		{
			if(itemVector[i].checkCollision(Player))
			{
				if(playSound)
					pickSound.play();
				int itemIndex = itemVector[i].getIndex();
				switch(itemIndex)
				{
					case -1:
					{
						if(hp < MAXHP)
						{
							hp += (MAXHP/10)*2;
							if(hp > MAXHP)
								hp = MAXHP;
							hpText.setString("HP: "+toStr((hp/(MAXHP*1.0))*100)+"%");
						}
						score += 5;
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
			else
			{
				itemVector[i].display(&window);
				i++;
			}
		}
		
		//Enemy handling
		if(enemyClock.getElapsedTime().asSeconds() > 1)
		{
			enemyMovVect.clear();
			for(int i = 0; i < enemyVector.size(); i++)
			{
				int movDir = std::rand()%3;
				enemyMovVect.push_back(movDir);
			}
			enemyClock.restart();
		}
		
		for(int i = 0; i < enemyVector.size();)
		{
			if(enemyMovVect[i] == 1)
			{
				enemyVector[i].changeDirection(LEFT);
				enemyVector[i].move(-MOVSPEED/2*delta,0);
				if(enemyVector[i].isColliding(&stageSprite))
					enemyVector[i].move(MOVSPEED/2*delta,0);
				if(enemyVector[i].getPosition().x < 0)
					enemyVector[i].setPosition(tempEnemyVector[i].getPosition().x, tempEnemyVector[i].getPosition().y);
			}
			else if(enemyMovVect[i] == 2)
			{
				enemyVector[i].changeDirection(RIGHT);
				enemyVector[i].move(MOVSPEED/2*delta,0);
				if(enemyVector[i].isColliding(&stageSprite))
					enemyVector[i].move(-MOVSPEED/2*delta,0);
				if(enemyVector[i].getPosition().x > WIDTH)
					enemyVector[i].setPosition(tempEnemyVector[i].getPosition().x, tempEnemyVector[i].getPosition().y);
			}
			
			int battleResult;
			enemyVector[i].draw(&window);
			if(Player.isColliding(&enemyVector[i]))
			{
				bgMusic.stop();
				battleResult = battleStage(&window, &frameTimer, plr, enemyVector[i].getName(), hp);
				hpText.setString("HP: "+toStr((hp/(MAXHP*1.0))*100)+"%");
				if(battleResult == 0)
				{
					bgMusic.stop();
					int overOption = gameOverScreen(&window, score, 0);
					if(overOption == 0)
					{
						if(playSound)
							bgMusic.play();
						hp = MAXHP/2;
						score = score/2;
						hpText.setString("HP: "+toStr((hp/(MAXHP*1.0))*100)+"%");
						scoreText.setString("Score: "+toStr(score));
						Player.setPosition(31,31);
						enemyVector = tempEnemyVector;
						itemVector = getItems(&lvlParser);
						break;
					}
					else
					{
						window.close();
					}
				}
				else if(battleResult == 1)
				{
					
					if(playSound)
						bgMusic.play();
					enemyVector.erase(enemyVector.begin()+i);
					enemyMovVect.erase(enemyMovVect.begin()+i);
					continue;
				}
				else
				{
					window.close();
				}
			}
			i++;
		}
		
		window.draw(hpText);
		window.draw(scoreText);
		
		if(itemVector.size() == 0 && enemyVector.size() == 0 && !displayClearedText)
		{
			levelClearClock.restart();
			displayClearedText = true;
			if(playSound)
				levelcompleteSound.play();
		}
		else if(displayClearedText && levelClearClock.getElapsedTime().asSeconds() <= 2)
		{
			window.draw(levelClearedText);
		}
		else if(itemVector.size() == 0 && enemyVector.size() == 0 && level < levelVector.size()-1)
		{
			level++;
			lvlParser.setLevelFile(levelVector[level]);
			lvlParser.parseFile();
			enemyVector = getEnemies(&lvlParser);
			tempEnemyVector = enemyVector;
			itemVector = getItems(&lvlParser);
			if(setStage(stageTextures[level], lvlParser.getStageFile()) == 1)
			{
				delete[] stageTextures;
				return 1;
			}
			updateStage(stageTextures[level], stageSprite);
			displayClearedText = false;
			displayLevelText = true;
			levelNameText.setString(lvlParser.getLevelName());
			levelNameDisplayClock.restart();
			Player.setPosition(31,31);
		}
		else if(itemVector.size() == 0 && enemyVector.size() == 0 && level == levelVector.size()-1)
		{
			bgMusic.stop();
			int battleRes = battleStage(&window, &frameTimer, plr, "Boss", hp);
			if(battleRes == 1)
			{
				int overOption = gameOverScreen(&window, score, 1);
				if(overOption == 0)
				{
					if(playSound)
						bgMusic.play();
					hp = MAXHP;
					score = 0;
					scoreText.setString("Score: "+toStr(score));
					hpText.setString("HP: "+toStr((hp/(MAXHP*1.0))*100)+"%");
					level = 0;
					lvlParser.setLevelFile(levelVector[level]);
					lvlParser.parseFile();
					enemyVector = getEnemies(&lvlParser);
					itemVector = getItems(&lvlParser);
					if(setStage(stageTextures[level], lvlParser.getStageFile()) == 1)
					{
						delete[] stageTextures;
						return 1;
					}
					updateStage(stageTextures[level], stageSprite);
					displayClearedText = false;
					displayLevelText = true;
					levelNameText.setString(lvlParser.getLevelName());
					levelNameDisplayClock.restart();
					Player.setPosition(31,31);
				}
				else
				{
					window.close();
				}
			}
			else if(battleRes == 0)
			{
				bgMusic.stop();
				int overOption = gameOverScreen(&window, score, 0);
				if(overOption == 0)
				{
					if(playSound)
						bgMusic.play();
					hp = MAXHP/2;
					score = score/2;
					hpText.setString("HP: "+toStr((hp/(MAXHP*1.0))*100)+"%");
					scoreText.setString("Score: "+toStr(score));
					Player.setPosition(31,31);
					enemyVector = tempEnemyVector;
					itemVector = getItems(&lvlParser);
				}
				else
				{
					window.close();
				}
			}
			else
			{
				window.close();
			}
		}
		
		if(displayLevelText && levelNameDisplayClock.getElapsedTime().asSeconds() <= 2)
		{
			window.draw(levelNameText);
		}
		else
		{
			displayLevelText = false;
		}

		window.display();
	}
	
	delete[] stageTextures;
	
	return 0;
}