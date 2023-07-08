#include <pch.h>
#include <Player.h>


BF::Player::Player(const char* filename)
	:Entity(filename)
{
	std::cout << "Created Player" << std::endl;
}

BF::Player::Player(Player&& other) noexcept
	:Entity(std::move(other))
{
	std::cout << "Copied Player" << std::endl;
}

BF::Player::~Player()
{
	std::cout << "Destroyed Player" << std::endl;
}

void BF::Player::update()
{
	setSpeed(0.f, 0.f);
	checkInput();
	Entity::update();
}

void BF::Player::checkInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		m_SpeedY -= 1.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_SpeedX -= 1.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		m_SpeedY += 1.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_SpeedX += 1.f;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		shoot();
	}
}

void BF::Player::shoot()
{

}