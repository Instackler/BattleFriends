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
	// TODO: add logging		std::cout << "Moved Player" << std::endl;
}

BF::Player& BF::Player::operator=(const Player& other)
{
	if (this != &other)
	{
		Entity::operator=(other);
		cooldown = other.cooldown;
		counter = other.counter;
	}
	return *this;
}

BF::Player& BF::Player::operator=(Player&& other) noexcept
{
	if (this != &other)
	{
		Entity::operator=(std::move(other));
		cooldown = other.cooldown;
		counter = other.counter;
	}
	return *this;
}

void BF::Player::collide(Entity& other)
{
	sf::Vector2f pos_diff{ other.getPosition().x - this->getPosition().x, other.getPosition().y - this->getPosition().y };
	float d = hypotf(pos_diff.x, pos_diff.y);
	sf::Vector2f pos_diff_normalized{ pos_diff.x / d, pos_diff.y / d };

	float nvx = (other.m_SpeedX - this->m_SpeedX) * pos_diff_normalized.x;
	float nvy = (other.m_SpeedY - this->m_SpeedY) * pos_diff_normalized.y;

	float impulse = nvx + nvy;
	this->m_SpeedX += impulse * pos_diff_normalized.x;
	this->m_SpeedY += impulse * pos_diff_normalized.y;
	other.m_SpeedX -= impulse * pos_diff_normalized.x;
	other.m_SpeedY -= impulse * pos_diff_normalized.y;

	float penetration = this->radius + other.radius - d;
	float penX = penetration * pos_diff_normalized.x;
	float penY = penetration * pos_diff_normalized.y;

	this->move(penX * -0.5f, penY * -0.5f);
	other.move(penX * 0.5f, penY * 0.5f);
}

void BF::Player::collide_with_map(Entity& other)
{
	sf::Vector2f pos_diff{ other.getPosition().x - this->getPosition().x, other.getPosition().y - this->getPosition().y };

	if (this->m_SpeedX > 0 && this->getPosition().x < other.getGlobalBounds().left)
	{
		this->move(pos_diff.x - this->radius - other.getGlobalBounds().width * 0.5f, 0.f);
		return;
	}
	if (this->m_SpeedX < 0 && this->getPosition().x > other.getGlobalBounds().left + other.getGlobalBounds().width)
	{
		this->move(pos_diff.x + this->radius + other.getGlobalBounds().width * 0.5f, 0.f);
		return;
	}
	if (this->m_SpeedY > 0 && this->getPosition().y < other.getGlobalBounds().top)
	{
		this->move(0.f, pos_diff.y - this->radius - other.getGlobalBounds().height * 0.5f);
		return;
	}
	if (this->m_SpeedY < 0 && this->getPosition().y > other.getGlobalBounds().top + other.getGlobalBounds().height)
	{
		this->move(0.f, pos_diff.y + this->radius + other.getGlobalBounds().height * 0.5f);
		return;
	}
}

void BF::Player::update(const player_inputs& inputs)
{
	setSpeed(0.f, 0.f);
	applyInputs(inputs);
	Entity::update();
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