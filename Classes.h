#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <ctime> //Included to use time as a seed for the RNG
#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

extern int windowHeight;
extern int windowWidth;
extern float windowHeight_f;
extern float windowWidth_f;

//Forward declaration of game so that there are no issues with enemy methods
class Game;

class Button
{
protected:
	sf::Text text;
	sf::RectangleShape background;
	bool selected = false;

public:
	Button() {};
	~Button() {};

	void set(sf::Font& font, int fontSize, std::string str, sf::Vector2f size, sf::Vector2f pos);
	void setColor(sf::Color color);

	void updateButton(sf::RenderWindow& targetWindow);

	void setSelection(bool inputBool);
	bool isMouseOver(sf::RenderWindow& inputWindow);
	void renderButton(sf::RenderWindow& targetWindow);
};

class purchaseButton : public Button
{
public:
	void renderButton(sf::RenderWindow& targetWindow, float price);
};

class Bullet
{
private:
	sf::RectangleShape shape;

	float direction;
	float speed;

public:
	Bullet();
	Bullet(sf::Vector2f inputPosition, float inputDirection, float inputSpeed);
	virtual ~Bullet();

	void update();
	void render(sf::RenderTarget& target);

	const sf::Vector2f getPos() const;
	const sf::FloatRect getBounds() const;
};

class Enemy
{
protected:
	sf::RectangleShape body;

	float direction;
	float speed;

	float Health;
	float Damage;

public:
	Enemy();
	Enemy(sf::Vector2f inputPosition, float inputSpeed, float inputHealth, float inputDamage);
	virtual ~Enemy();

	void update(sf::Vector2f heroPos);
	void render(sf::RenderTarget& target);

	virtual void death(Game& parentGame);

	const sf::Vector2f getPos() const;
	const float getHealth() const;
	void setHealth(const float damage);
	const sf::FloatRect getBounds() const;
	const float getDamage() const;
	const float getSpeed() const;
	const float getDirection() const;
};

class blueEnemy : public Enemy
{
public:
	blueEnemy(sf::Vector2f inputPosition, float inputSpeed, float inputHealth, float inputDamage);
	virtual ~blueEnemy();
};

class motherEnemy : public Enemy
{
public:
	motherEnemy(sf::Vector2f inputPosition, float inputSpeed, float inputHealth, float inputDamage);
	virtual ~motherEnemy();

	void death(Game& parentGame);
};

class babyEnemy : public Enemy
{
public:
	babyEnemy(sf::Vector2f inputPosition, float inputSpeed, float inputHealth, float inputDamage);
	virtual ~babyEnemy();
};

class Game
{
private:
	sf::RenderWindow* window;
	sf::Event e;

	//Fonts
	sf::Font font;

	//Texts
	sf::Text healthText;
	sf::Text endScreenText;
	sf::Text interludeText;
	sf::Text roundText;
	sf::Text moneyText;

	sf::Text damageUpText;
	sf::Text attackSpeedUpText;
	sf::Text maxHealthUpText;
	sf::Text movementSpeedUpText;
	sf::Text projectileNumUptext;
	sf::Text buyHealthPackText;

	//Buttons
	Button shopButton;

	purchaseButton dUpButton; //Possible optimization by making the float a part of the purchase button
	float dUpPrice;

	purchaseButton asUpButton;
	float asUpPrice;

	purchaseButton mhUpButton;
	float mhUpPrice;

	purchaseButton msUpButton;
	float msUpPrice;

	purchaseButton pnUpButton;
	float pnUpPrice;

	purchaseButton bhpButton;
	float bhpPrice;

	//Shop background initialised in the general functions
	sf::RectangleShape shopBackground;

	sf::RectangleShape hero;
	float heroMaxHealth;
	float heroHealth;
	float heroDamage;
	float heroSpeed;
	int attackSpeedLevel;
	int projectileNum;
	float money;
	bool heroInvincible;
	float invincibilityTimer;
	
	int roundNumber;
	bool gameEnded;
	int miniCountdown;
	int interludeCountdown;
	bool roundEnded;
	bool inShop;

	std::vector<Bullet*> bullets;
	float reloadTimerMax;
	float reloadTimer;

	std::vector<Enemy*> enemies;
	float enemyHealth;
	float enemySpeed;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int enemiesSpawned;
	int enemiesKilled;

public:
	std::multimap<int, sf::Vector2f> enemiesToAdd;

	void initVariables();
	void initWindow();
	void initHero();
	void initFont();
	void initText();
	void initButtons();
	void initBackgrounds();

	Game();
	virtual ~Game();

	void pollEvents();
	void shoot();

	float dUpPriceCalc();
	float asUpPriceCalc();
	float mhUpPriceCalc();
	float msUpPriceCalc();
	float pnUpPriceCalc();
	float bhpPriceCalc();

	void spawnEnemies();
	void spawnControl();

	//Enemies that spawn outside of the regular pattern.
	void spawnExtraEnemies();

	void updateHero();
	void updateBullets();
	void updateEnemies();
	void updateCombat();
	void updateText();
	void updateInterlude();
	void updateGameLogic();
	void updateButtons();
	void update();
	
	void renderHero(sf::RenderTarget& target);
	void renderHealthText(sf::RenderTarget& target);
	void renderEndText(sf::RenderTarget& target);
	void renderInterludeText(sf::RenderTarget& target);
	void renderRoundText(sf::RenderTarget& target);
	void renderMoneyText(sf::RenderTarget& target);
	void renderShopText(sf::RenderTarget& target);
	void render();

	const bool getWindowIsOpen() const;
	float mouseDirection();
};