#include <pch.h>
#include <Player.h>
#include <BF.h>


/*
BF::Player::Player(const char* filename)
	:Entity(filename)
{
	// TODO: add logging		std::cout << "Created Player" << std::endl;
}
*/

BF::Player::Player(int textureID)
	:Entity(textureID)
{
	game_inputs.emplace_back(); // TODO: fix potential bug
	// TODO: add logging		std::cout << "Created Player" << std::endl;
}

BF::Player::Player(const Player& other)
	:Entity(other)
{
	cooldown = other.cooldown;
	counter = other.counter;
}

BF::Player::Player(Player&& other) noexcept
	:Entity(std::move(other))
{
	cooldown = other.cooldown;
	counter = other.counter;
	// TODO: add logging		std::cout << "Copied Player" << std::endl;
}

BF::Player::~Player()
{
	// TODO: add logging		std::cout << "Destroyed Player" << std::endl;
}

void BF::Player::update(const player_inputs& inputs)
{
	if (out_of_bounds())
	{
		bounce();
	}
	else
	{
		setSpeed(0.f, 0.f);
		applyInputs(inputs);
		Entity::update();
	}
}

void BF::Player::applyInputs(const player_inputs& inputs)
{
	if (inputs.up)
	{
		m_SpeedY -= 2.f;
	}
	if (inputs.left)
	{
		m_SpeedX -= 2.f;
	}
	if (inputs.down)
	{
		m_SpeedY += 2.f;
	}
	if (inputs.right)
	{
		m_SpeedX += 2.f;
	}
	if (inputs.shoot)
	{
		shoot();
	}
}

void BF::Player::shoot()
{
	if (counter == 0)
	{
		counter = cooldown;
		sf::Vector2i sf_mouse_pos = sf::Mouse::getPosition(*(sf::RenderWindow*)BF::default_target);
		sf::Vector2i mouse_pos (sf_mouse_pos.x - BF::default_target->getSize().x / 2, sf_mouse_pos.y - BF::default_target->getSize().y / 2);
		float length = std::hypot(mouse_pos.x, mouse_pos.y);
		sf::Vector2f speed = { mouse_pos.x / length, mouse_pos.y / length};
		projectiles.emplace_back(getPosition() + speed * radius * 1.1f, speed * 0.8f);
	}
	counter--;
}