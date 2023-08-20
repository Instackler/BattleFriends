#include <pch.h>
#include <Player.h>
#include <BF.h>


BF::Player::Player(const std::string& textureID)
	:Entity(textureID)
{
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
		shoot(inputs.mouse_pos);
	}
}

void BF::Player::shoot(const sf::Vector2i mouse_pos)
{
	if (counter == 0)
	{
		counter = cooldown;
		sf::Vector2i mouse_pos_crel(mouse_pos.x - BF::default_target->getSize().x / 2, mouse_pos.y - BF::default_target->getSize().y / 2); //TODO: change getSize() to equivalent sf::RenderWindow method
		float length = std::hypot(mouse_pos_crel.x, mouse_pos_crel.y);
		sf::Vector2f speed = { mouse_pos_crel.x / length, mouse_pos_crel.y / length };
		projectiles.emplace_back(getPosition() + speed * radius * 1.1f, speed * 0.8f);
	}
	counter--;
}