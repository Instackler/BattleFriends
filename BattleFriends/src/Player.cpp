#include <pch.h>
#include <Player.h>
#include <BF.h>


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
	checkInput();
	Entity::update();
}

void BF::Player::checkInput()
{
	setSpeed(0.f, 0.f);

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
	if (counter == 0)
	{
		counter = cooldown;
		sf::Vector2f player_pos = this->getPosition();
		sf::Vector2i mouse_pos = sf::Mouse::getPosition();
		auto diff = sf::Vector2f { mouse_pos.x - player_pos.x, mouse_pos.y - player_pos.y };
		float length = std::hypot(diff.x, diff.y);
		BF::projectiles.emplace_back(player_pos, sf::Vector2f {diff.x / length, diff.y / length} );
	}
	counter--;
}