#include "Classes.h"

int windowHeight = 720;
int windowWidth = 1280;
float windowHeight_f = 720.f;
float windowWidth_f = 1280.f;


Bullet::Bullet()
{
	speed = 1.f;
	direction = 3.142;
	shape.setSize(sf::Vector2f(5.f, 5.f));
	shape.setScale(sf::Vector2f(1.f, 1.f));
	shape.setFillColor(sf::Color::Yellow);
}

Bullet::Bullet(sf::Vector2f inputPosition, float inputDirection, float inputSpeed)
{
	shape.setSize(sf::Vector2f(5.f, 5.f));
	shape.setScale(sf::Vector2f(1.f, 1.f));
	shape.setFillColor(sf::Color::Yellow);
	shape.setPosition(inputPosition);

	direction = inputDirection;
	speed = inputSpeed;
}

Bullet::~Bullet()
{

}

void Bullet::update()
{
	shape.move(speed * cos(direction), speed * sin(direction));
}

void Bullet::render(sf::RenderTarget& target)
{
	target.draw(this->shape);
}

const sf::Vector2f Bullet::getPos() const
{
	return shape.getPosition();
}

const sf::FloatRect Bullet::getBounds() const
{
	return shape.getGlobalBounds();
}


void Game::initVariables()
{
	this->window = nullptr;
	
	this->heroMaxHealth = 100.0;
	this->heroHealth = heroMaxHealth;
	this->heroDamage = 20.f;
	this->heroSpeed = 2.5f;
	this->attackSpeedLevel = 1;
	this->projectileNum = 1;
	this->heroInvincible = false;
	this->invincibilityTimer = 0.0;

	this->roundNumber = 1;
	this->gameEnded = false;
	this->miniCountdown = 0;
	this->interludeCountdown = 5;
	this->inShop = false;

	this->reloadTimerMax = 5.f;
	this->reloadTimer = 0.f;

	this->enemySpeed = 2.f;
	this->enemySpawnTimer = 0.0;
	this->enemySpawnTimerMax = 100.0;
	this->enemiesSpawned = 0;
	this->enemiesKilled = 0;
}

void Game::initWindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode(windowWidth, windowHeight), "TopDown Shooter Window", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(60);
}

void Game::initHero()
{
	hero.setSize(sf::Vector2f(20.f, 50.f));
	hero.setScale(sf::Vector2f(0.5f, 0.5f));
	hero.setPosition(sf::Vector2f((windowWidth_f / 2), (windowHeight_f / 2)));
	hero.setFillColor(sf::Color::Green);
}

void Game::initFont()
{
	if (!font.loadFromFile("C:/Users/zephy/source/repos/SFML Projects/TopDownShooter/Fonts/RobotoMono-SemiBold.ttf")) {
		std::cout << "ERROR: Failed to load font." << std::endl;
	}
}

void Game::initText()
{
	//Text for health
	this->healthText.setFont(font);
	this->healthText.setString("Health: ");
	this->healthText.setCharacterSize(24);
	this->healthText.setFillColor(sf::Color::Yellow);

	//Text for the end screen
	this->endScreenText.setFont(font);
	this->endScreenText.setCharacterSize(30);
	this->endScreenText.setFillColor(sf::Color::Red);
	this->endScreenText.setPosition(sf::Vector2f(40.f, 100.f));

	//Text for the interlude screen
	this->interludeText.setFont(font);
	this->interludeText.setCharacterSize(30);
	this->interludeText.setFillColor(sf::Color::Red);
	this->interludeText.setPosition(sf::Vector2f(40.f, 100.f));

	//Text for the round count
	this->roundText.setFont(font);
	this->roundText.setCharacterSize(24);
	this->roundText.setFillColor(sf::Color::Yellow);
	this->roundText.setPosition(sf::Vector2f(380.f, 0.f));

	//Text for the money count
	this->moneyText.setFont(font);
	this->moneyText.setCharacterSize(24);
	this->moneyText.setFillColor(sf::Color::Yellow);
	this->moneyText.setPosition(sf::Vector2f(780.f, 0.f));

	//Text for the shop
	damageUpText.setFont(font);
	damageUpText.setCharacterSize(24);
	damageUpText.setFillColor(sf::Color::Blue);
	damageUpText.setPosition(sf::Vector2f(180.f, 140.f));

	attackSpeedUpText.setFont(font);
	attackSpeedUpText.setCharacterSize(24);
	attackSpeedUpText.setFillColor(sf::Color::Blue);
	attackSpeedUpText.setPosition(sf::Vector2f(380.f, 140.f));

	maxHealthUpText.setFont(font);
	maxHealthUpText.setCharacterSize(24);
	maxHealthUpText.setFillColor(sf::Color::Blue);
	maxHealthUpText.setPosition(sf::Vector2f(580.f, 140.f));

	movementSpeedUpText.setFont(font);
	movementSpeedUpText.setCharacterSize(24);
	movementSpeedUpText.setFillColor(sf::Color::Blue);
	movementSpeedUpText.setPosition(sf::Vector2f(780.f, 140.f));

	projectileNumUptext.setFont(font);
	projectileNumUptext.setCharacterSize(24);
	projectileNumUptext.setFillColor(sf::Color::Blue);
	projectileNumUptext.setPosition(sf::Vector2f(280.f, 400.f));

	buyHealthPackText.setFont(font);
	buyHealthPackText.setCharacterSize(24);
	buyHealthPackText.setFillColor(sf::Color::Blue);
	buyHealthPackText.setPosition(sf::Vector2f(680.f, 400.f));

}

void Game::initButtons()
{
	shopButton.set(font, 40, "Shop", sf::Vector2f(115.f, 40.f), sf::Vector2f(1100.f, 200.f));

	dUpButton.set(font, 20, "Damage", sf::Vector2f(100.f, 50.f), sf::Vector2f(180.f, 250.f));
	dUpPrice = dUpPriceCalc();

	asUpButton.set(font, 20, "Attack Speed", sf::Vector2f(100.f, 50.f), sf::Vector2f(380.f, 250.f));
	asUpPrice = asUpPriceCalc();

	mhUpButton.set(font, 20, "Max Health", sf::Vector2f(100.f, 50.f), sf::Vector2f(580.f, 250.f));
	mhUpPrice = mhUpPriceCalc();

	msUpButton.set(font, 20, "Movement Speed", sf::Vector2f(100.f, 50.f), sf::Vector2f(780.f, 250.f));
	msUpPrice = msUpPriceCalc();

	pnUpButton.set(font, 20, "Projectile Speed", sf::Vector2f(100.f, 50.f), sf::Vector2f(280.f, 510.f));
	pnUpPrice = pnUpPriceCalc();

	bhpButton.set(font, 20, "Health Pack", sf::Vector2f(100.f, 50.f), sf::Vector2f(680.f, 510.f));
	bhpPrice = bhpPriceCalc();
}

void Game::initBackgrounds()
{
	this->shopBackground.setSize(sf::Vector2f(1000.f, 600.f));
	this->shopBackground.setScale(sf::Vector2f(1.f, 1.f));
	this->shopBackground.setPosition(sf::Vector2f(100.f, 50.f));
	this->shopBackground.setFillColor(sf::Color(155, 155, 155, 200));
}

Game::Game()
{
	initVariables();
	initHero();
	initFont();
	initText();
	initWindow();
	initButtons();
	initBackgrounds();
}

Game::~Game()
{
	delete this->window;

	for (size_t i = 0; i < bullets.size(); i++) {
		delete bullets[i];
	}

	for (size_t i = 0; i < enemies.size(); i++) {
		delete enemies[i];
	}
}

void Game::pollEvents()
{
	while (this->window->pollEvent(this->e))
	{
		switch (this->e.type) {
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->e.key.code == sf::Keyboard::Escape) {
				this->window->close();
			}
			break;
		case sf::Event::MouseButtonPressed:
			if (shopButton.isMouseOver(*(this->window))) {
				inShop = !inShop;
			} else if (dUpButton.isMouseOver(*(this->window)) && money > dUpPrice) {
				//Damage Up button pressed and player can afford the upgrade
				money -= dUpPrice;
				heroDamage += 10.f;
				dUpPrice = dUpPriceCalc();
			} else if (asUpButton.isMouseOver(*(this->window)) && money > asUpPrice) {
				//Attack Speed Up button pressed and player can afford the upgrade
				money -= asUpPrice;
				attackSpeedLevel++;
				reloadTimerMax -= 0.5f;
				asUpPrice = asUpPriceCalc();
			} else if (mhUpButton.isMouseOver(*(this->window)) && money > mhUpPrice) {
				//Max Health Up button pressed and player can afford the upgrade
				money -= mhUpPrice;
				heroMaxHealth += 20.f;
				mhUpPrice = mhUpPriceCalc();
			} else if (msUpButton.isMouseOver(*(this->window)) && money > msUpPrice) {
				//Movement Speed Up button pressed and player can afford the upgrade
				money -= msUpPrice;
				heroSpeed += 0.25f;
				msUpPrice = msUpPriceCalc();
			} else if (pnUpButton.isMouseOver(*(this->window)) && money > pnUpPrice) {
				//Projectile Number Up button pressed and player can afford the upgrade
				money -= pnUpPrice;
				projectileNum++;
				pnUpPrice = pnUpPriceCalc();
			} else if (bhpButton.isMouseOver(*(this->window)) && money > bhpPrice) {
				//Purchase health pack button pressed and player can afford it
				money -= bhpPrice;
				heroHealth += 40.f;

				if (heroHealth > heroMaxHealth) { heroHealth = heroMaxHealth; };

				bhpPrice = bhpPriceCalc();
			}
		}
	}
}

void Game::shoot()
{
	sf::Vector2f heroPos = hero.getPosition();
	if (reloadTimer >= reloadTimerMax) {
		for (int i = 0; i < projectileNum; i++) {
			bullets.push_back(new Bullet(sf::Vector2f(heroPos.x + (i * 5), heroPos.y + (i * 5)), mouseDirection(), 10.f));
		}
		reloadTimer = 0.f;
	}
}

float Game::dUpPriceCalc()
{
	return heroDamage * 10;
}

float Game::asUpPriceCalc()
{
	return attackSpeedLevel * 1000;
}

float Game::mhUpPriceCalc()
{
	return heroMaxHealth * 50;
}

float Game::msUpPriceCalc()
{
	return heroSpeed * 2500;
}

float Game::pnUpPriceCalc()
{
	return pow(1000, projectileNum);
}

float Game::bhpPriceCalc()
{
	return 500 * roundNumber;
}

void Game::spawnEnemies()
{
	//Updates the enemy health for every round automatically
	enemyHealth = 100.f + (roundNumber - 1) * 25.f;

	int tempnum = rand() % 4;
	float spawningPos_x = 0.0;
	float spawningPos_y = 0.0;

	switch (tempnum)
	{
	case 0:
		//The enemy will spawn in the north side
		spawningPos_x = rand() % (windowWidth - 10);
		spawningPos_y = rand() % 10;
		break;
	case 1:
		//The enemy will spawn in the east
		spawningPos_x = rand() % 10 + (windowWidth - 20);
		spawningPos_y = rand() % (windowHeight - 10);
		break;
	case 2:
		//The enemy will spawn in the south 
		spawningPos_x = rand() % (windowWidth - 10);
		spawningPos_y = rand() % 10 + (windowHeight - 10);
		break;
	case 3:
		//The enemy will spawn in the west
		spawningPos_x = rand() % 10;
		spawningPos_y = rand() % (windowHeight - 10);
		break;
	}

	//Controls the probability at which each enemy starts appearing
	int enemyProb = rand() % 100;

	if(enemyProb <= 50) {
		enemies.push_back(new Enemy(sf::Vector2f(spawningPos_x, spawningPos_y), enemySpeed, enemyHealth, 10.f));
	} else if(enemyProb <= 80){
		enemies.push_back(new blueEnemy(sf::Vector2f(spawningPos_x, spawningPos_y), enemySpeed, enemyHealth, 10.f));
	} else {
		enemies.push_back(new motherEnemy(sf::Vector2f(spawningPos_x, spawningPos_y), enemySpeed, enemyHealth, 10.f));
	}
}

void Game::spawnControl()
{
	//This is what controls how many enemies will be spawned every round
	if (enemySpawnTimer > enemySpawnTimerMax && enemiesSpawned < (roundNumber + 5) && roundEnded == false) {
		spawnEnemies();
		enemiesSpawned++;
		enemySpawnTimer = 0.0;
	} else if (enemiesSpawned < (roundNumber + 5)) {
		enemySpawnTimer += 1.0;
	}
}

void Game::spawnExtraEnemies()
{
	//Spawn enemies that are waiting to be spawned
	for (auto it = enemiesToAdd.begin(); it != enemiesToAdd.end(); it++) {
		//Change for a switch later on if needed
		if (it->first == 3) {
			enemies.push_back(new babyEnemy(it->second, enemySpeed, enemyHealth, 10.f));
		}
	}
	//Empty the list behind. To be filled the following game loop
	enemiesToAdd.clear();
}

void Game::updateHero()
{
	sf::Vector2f heroMovementDirection = sf::Vector2f(0.f, 0.f);

	if (invincibilityTimer > 50) {
		heroInvincible = false;
		invincibilityTimer = 0.0;
	} else if (heroInvincible) {
		invincibilityTimer += 1.0;
	}
	
	if (heroHealth <= 0) {
		gameEnded = true;
	}

	//Smooth hero movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		heroMovementDirection.y += -1.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		heroMovementDirection.x += -1.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		heroMovementDirection.y += 1.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		heroMovementDirection.x += 1.f;
	}

	hero.move(sf::Vector2f(heroMovementDirection.x * heroSpeed, heroMovementDirection.y * heroSpeed));
}

void Game::updateBullets()
{
	for (size_t i = 0; i < bullets.size(); i++) {

		sf::Vector2f pos = bullets[i]->getPos();

		bullets[i]->update();

		if (pos.x > windowWidth || pos.x < 0.f || pos.y > windowHeight || pos.y < 0.f) {
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
		}
	}
}

void Game::updateEnemies()
{
	unsigned counter = 0;
	for (std::vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); /* No increment here */)
	{
		(*it)->update(hero.getPosition());

		sf::Vector2f pos = (*it)->getPos();

		//Enemy player collision
		if ((*it)->getBounds().intersects(this->hero.getGlobalBounds()) && !heroInvincible) {
			heroHealth -= (*it)->getDamage();
			heroInvincible = true;
		}

		//Enemy culling
		if ((pos.x > windowWidth || pos.x < 0.f || pos.y > windowHeight || pos.y < 0.f || (*it)->getHealth() <= 0)) {

			//When the enemy dies, execute its death method
			(*it)->death(*this);

			//Delete enemy
			delete* it;
			it = enemies.erase(it);
			enemiesKilled++;
			money += (100 + 10 * roundNumber);
			continue;
		} else {
			it++;
		}
	}
}

void Game::updateCombat()
{
	for (size_t i = 0; i < enemies.size(); i++) {

		for (size_t j = 0; j < bullets.size(); j++) {

			if (enemies[i]->getBounds().intersects(bullets[j]->getBounds())) {

				//This controls how much health the enemies lose from each bullet
				enemies[i]->setHealth(heroDamage);

				delete bullets[j];
				bullets.erase(bullets.begin() + j);

				money += 10;
			}
		}
	}
}

void Game::updateText()
{
	std::stringstream ss;
	ss << "Health: " << heroHealth;
	healthText.setString(ss.str());
}

void Game::updateInterlude()
{
	if (miniCountdown > 59) {
		miniCountdown = 0;
		interludeCountdown--;
	}
	miniCountdown++;
}

void Game::updateGameLogic()
{
	//Make sure enough enemies have been killed and all the enemies on screen have been killed.
	//Added this because the babies that are spawned cause problems
	if (enemiesKilled >= roundNumber + 5 && enemies.size() == 0) {
		//Reset numbers as a new round will begin
		enemiesKilled = 0;
		enemiesSpawned = 0;
		roundEnded = true;
	}
	if (interludeCountdown <= 0) {
		//Update the round numbers and reset the interlude countdown
		roundNumber++;
		interludeCountdown = 5;
		//No longer in the interlude
		roundEnded = false;
	}
}

void Game::updateButtons()
{
	shopButton.updateButton(*(this->window));

	dUpButton.updateButton(*(this->window));
	asUpButton.updateButton(*(this->window));
	mhUpButton.updateButton(*(this->window));
	msUpButton.updateButton(*(this->window));
	pnUpButton.updateButton(*(this->window));
	bhpButton.updateButton(*(this->window));
}

void Game::update()
{
	pollEvents();
	
	if (gameEnded == false && inShop == false) {

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			shoot();
		}

		//Spawn enemies waiting in the toAdd multimap
		spawnExtraEnemies();

		//Update the bullets and enemies
		updateBullets();
		updateEnemies();
		updateCombat();

		updateHero();

		spawnControl();
		updateGameLogic();

		if (roundEnded == true) {
			updateInterlude();
		}

		updateText();

		reloadTimer += 0.5f;
	}
	updateButtons();
}

void Game::renderHero(sf::RenderTarget& target)
{
	target.draw(hero);
}

void Game::renderHealthText(sf::RenderTarget& target)
{
	target.draw(healthText);
}

void Game::renderEndText(sf::RenderTarget& target)
{
	std::stringstream ss;
	ss << "          Game Over!" << std::endl << std::endl << "Got to round: " << roundNumber << std::endl << std::endl << "Press Escape to close the game.";
	endScreenText.setString(ss.str());
	target.draw(this->endScreenText);
}

void Game::renderInterludeText(sf::RenderTarget& target)
{
	std::stringstream ss;
	ss << "          Round cleared!" << std::endl << std::endl << "Next round will start in: " << interludeCountdown;
	endScreenText.setString(ss.str());
	target.draw(this->endScreenText);
}

void Game::renderRoundText(sf::RenderTarget& target)
{
	std::stringstream ss;
	ss << "Round Number: " << roundNumber;
	roundText.setString(ss.str());
	target.draw(this->roundText);
}

void Game::renderMoneyText(sf::RenderTarget& target)
{
	std::stringstream ss;
	ss << "Money: " << money;
	moneyText.setString(ss.str());
	target.draw(this->moneyText);
}

void Game::renderShopText(sf::RenderTarget& target)
{
	std::stringstream ss;
	ss << "Damage \n \nCurrent: " << heroDamage;
	damageUpText.setString(ss.str());
	ss.clear();
	ss.str("");

	ss << "Attack Speed \n \nCurrent: " << attackSpeedLevel;
	attackSpeedUpText.setString(ss.str());
	ss.clear();
	ss.str("");

	ss << "Max Health \n \nCurrent: " << heroMaxHealth;
	maxHealthUpText.setString(ss.str());
	ss.clear();
	ss.str("");

	ss << "Movement Speed \n \nCurrent: " << heroSpeed;
	movementSpeedUpText.setString(ss.str());
	ss.clear();
	ss.str("");

	ss << "Number of Projectiles \n \nCurrent: " << projectileNum;
	projectileNumUptext.setString(ss.str());
	ss.clear();
	ss.str("");

	ss << "Buy health pack \n \nHeals 50 points";
	buyHealthPackText.setString(ss.str());
	ss.clear();
	ss.str("");

	target.draw(damageUpText);
	target.draw(attackSpeedUpText);
	target.draw(maxHealthUpText);
	target.draw(movementSpeedUpText);
	target.draw(projectileNumUptext);
	target.draw(buyHealthPackText);
}

void Game::render()
{
	this->window->clear();

	renderHero(*(this->window));
	renderHealthText(*(this->window));
	renderRoundText(*(this->window));
	renderMoneyText(*(this->window));

	//Render the bullets
	for (size_t i = 0; i < bullets.size(); i++) {
		bullets[i]->render(*(this->window));
	}

	//Render the enemies
	for (size_t i = 0; i < enemies.size(); i++) {
		enemies[i]->render(*(this->window));
	}

	if (roundEnded == true) {
		renderInterludeText(*(this->window));
	}

	if (gameEnded == true) {
		renderEndText(*(this->window));
	}

	shopButton.renderButton(*(this->window));

	if (inShop) {
		(*(this->window)).draw(shopBackground);
		renderShopText(*(this->window));
		//Render the purchase buttons

		dUpButton.renderButton(*(this->window), dUpPrice);
		asUpButton.renderButton(*(this->window), asUpPrice);
		mhUpButton.renderButton(*(this->window), mhUpPrice);
		msUpButton.renderButton(*(this->window), msUpPrice);
		pnUpButton.renderButton(*(this->window), pnUpPrice);
		bhpButton.renderButton(*(this->window), bhpPrice);
	}

	this->window->display();
}

const bool Game::getWindowIsOpen() const
{
	return this->window->isOpen();
}

float Game::mouseDirection()
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
	sf::Vector2f heroPos = hero.getPosition();

	if (mousePos.x - heroPos.x < 0) {
		return (3.142 + atan((mousePos.y - heroPos.y) / (mousePos.x - heroPos.x)));
	} else {
		return atan((mousePos.y - heroPos.y) / (mousePos.x - heroPos.x));
	}
}

Enemy::Enemy()
{
	speed = 1.f;
	direction = 3.142;

	body.setSize(sf::Vector2f(15.f, 15.f));
	body.setScale(sf::Vector2f(1.f, 1.f));
	body.setFillColor(sf::Color::Red);
	body.setPosition(sf::Vector2f(0.f, 0.f));

	Health = 100;
	Damage = 10;
}

Enemy::Enemy(sf::Vector2f inputPosition, float inputSpeed, float inputHealth, float inputDamage)
{
	speed = 1.f;
	direction = 3.142;

	body.setPosition(inputPosition);
	body.setSize(sf::Vector2f(15.f, 15.f));
	body.setScale(sf::Vector2f(1.f, 1.f));
	body.setFillColor(sf::Color::Red);

	speed = inputSpeed;
	Health = inputHealth;
	Damage = inputDamage;
}

Enemy::~Enemy()
{

}

void Enemy::update(sf::Vector2f heroPos)
{
	//Finds the direction it needs to go in to get to the hero
	//Sets direction equal to this
	sf::Vector2f enemyPos = body.getPosition();
	if (heroPos.x - enemyPos.x < 0) {
		direction = 3.142 + atan((heroPos.y - enemyPos.y) / (heroPos.x - enemyPos.x));
	}
	else {
		direction = atan((heroPos.y - enemyPos.y) / (heroPos.x - enemyPos.x));
	}

	//Move the enemy towards the hero
	body.move(sf::Vector2f(speed * cos(direction), speed * sin(direction)));
}

void Enemy::render(sf::RenderTarget& target)
{
	target.draw(body);
}

void Enemy::death(Game &parentGame)
{
	//Does nothing for now
}

const sf::Vector2f Enemy::getPos() const
{
	return body.getPosition();
}

const float Enemy::getHealth() const
{
	return Health;
}

void Enemy::setHealth(const float damage)
{
	Health -= damage;
}

const sf::FloatRect Enemy::getBounds() const
{
	return body.getGlobalBounds();
}

const float Enemy::getDamage() const
{
	return Damage;
}

const float Enemy::getSpeed() const
{
	return speed;
}

const float Enemy::getDirection() const
{
	return direction;
}

blueEnemy::blueEnemy(sf::Vector2f inputPosition, float inputSpeed, float inputHealth, float inputDamage)
{
	speed = 2.f;
	direction = 3.142;

	body.setPosition(inputPosition);
	body.setSize(sf::Vector2f(15.f, 15.f));
	body.setScale(sf::Vector2f(0.75f, 0.75f));
	body.setFillColor(sf::Color::Cyan);

	//Blue enemies will have these modifications compared to the base red enemy
	speed = inputSpeed * 2;
	Health = inputHealth * 0.5;
	Damage = inputDamage * 0.5;
}

blueEnemy::~blueEnemy()
{

}

motherEnemy::motherEnemy(sf::Vector2f inputPosition, float inputSpeed, float inputHealth, float inputDamage)
{
	speed = 1.f;
	direction = 3.142;

	body.setPosition(inputPosition);
	body.setSize(sf::Vector2f(15.f, 15.f));
	body.setScale(sf::Vector2f(2.f, 2.f));
	body.setFillColor(sf::Color::Magenta);

	//Mother enemies will have these modifications compared to the base red enemy
	speed = inputSpeed * 0.5;
	Health = inputHealth * 2;
	Damage = inputDamage;
}

motherEnemy::~motherEnemy()
{

}

void motherEnemy::death(Game &parentGame)
{
	sf::Vector2f motherPos = body.getPosition();
	parentGame.enemiesToAdd.insert({ 3, sf::Vector2f((motherPos.x + 10.f), (motherPos.y - 10.f)) });
	parentGame.enemiesToAdd.insert({ 3, sf::Vector2f((motherPos.x - 10.f), (motherPos.y + 10.f)) });
	parentGame.enemiesToAdd.insert({ 3, sf::Vector2f((motherPos.x + 10.f), (motherPos.y + 10.f)) });
}

babyEnemy::babyEnemy(sf::Vector2f inputPosition, float inputSpeed, float inputHealth, float inputDamage)
{
	speed = 1.f;
	direction = 3.142;

	body.setPosition(inputPosition);
	body.setSize(sf::Vector2f(15.f, 15.f));
	body.setScale(sf::Vector2f(0.5f, 0.5f));
	body.setFillColor(sf::Color::Magenta);

	//Baby enemies will have these modifications compared to the base red enemy
	speed = inputSpeed * 2;
	Health = inputHealth * 0.33;
	Damage = inputDamage * 0.5;
}

babyEnemy::~babyEnemy()
{

}

void Button::set(sf::Font& font, int fontSize, std::string str, sf::Vector2f size, sf::Vector2f pos)
{
	text.setFont(font);
	text.setString(str);
	text.setCharacterSize(fontSize);
	text.setPosition(pos);
	text.setFillColor(sf::Color::Yellow);

	background.setSize(size);
	background.setPosition(pos);
	background.setFillColor(sf::Color::Transparent);
}

void Button::setColor(sf::Color color)
{
	text.setFillColor(color);
}

void Button::updateButton(sf::RenderWindow& targetWindow)
{
	if (this->isMouseOver(targetWindow)) {
		this->setColor(sf::Color::Green);
	}
	else {
		this->setColor(sf::Color::Yellow);
	}
}

void Button::setSelection(bool inputBool)
{
	selected = inputBool;
}

bool Button::isMouseOver(sf::RenderWindow& inputWindow)
{
	float mousePos_x = sf::Mouse::getPosition(inputWindow).x;
	float mousePos_y = sf::Mouse::getPosition(inputWindow).y;

	if (background.getGlobalBounds().contains(sf::Vector2f(mousePos_x, mousePos_y))) {
		return true;
	} else {
		return false;
	}
}


void Button::renderButton(sf::RenderWindow& targetWindow)
{
	targetWindow.draw(background);
	targetWindow.draw(text);
}

void purchaseButton::renderButton(sf::RenderWindow& targetWindow, float price)
{
	std::stringstream ss;
	ss << "Costs: " << price;
	text.setString(ss.str());

	targetWindow.draw(background);
	targetWindow.draw(text);
}
