#include <pch.h>
#include <Entity.h>
#include <BF.h>


BF::Entity::Entity(const std::string& textureID)
	:m_textureID(textureID)
{
	setTexture(textures[textureID]);

	sf::FloatRect bounding_box = getLocalBounds();
	setOrigin(bounding_box.width / 2.f, bounding_box.height / 2.f);
	radius = bounding_box.width < bounding_box.height ?
		bounding_box.width / 2.f : bounding_box.height / 2.f;

	// TODO: add logging		std::cout << "Created Entity" << std::endl;
}


BF::Entity::Entity(const Entity& other)
	:sf::Sprite(other)
{
	m_textureID = other.m_textureID;
	setTexture(textures[m_textureID]);
	radius = other.radius;
	setPosition(other.getPosition());
	setSpeed(other.m_SpeedX, other.m_SpeedY);
	health = other.health;
	stationary = other.stationary;
}

BF::Entity::Entity(Entity&& other) noexcept
	:sf::Sprite(std::move(other))
{
	// TODO: add logging		std::cout << "Entity moved\n";
	m_textureID = other.m_textureID;
	setTexture(textures[m_textureID]);
	radius = other.radius;
	setPosition(other.getPosition());
	setSpeed(other.m_SpeedX, other.m_SpeedY);
	health = other.health;
	stationary = other.stationary;
}

BF::Entity& BF::Entity::operator=(const Entity& other)
{
	// TODO: add logging		std::cout << "Entity& =\n";
	if (this != &other)
	{
		sf::Sprite::operator=(other);
		radius = other.radius;
		m_textureID = other.m_textureID;
		setTexture(textures[m_textureID]);
		setPosition(other.getPosition());
		setSpeed(other.m_SpeedX, other.m_SpeedY);
		health = other.health;
		stationary = other.stationary;
	}
	return *this;
}

BF::Entity& BF::Entity::operator=(Entity&& other) noexcept
{
	// TODO: add logging		std::cout << "Entity&& =\n";
	if (this != &other)
	{
		sf::Sprite::operator=(std::move(other));
		radius = other.radius;
		m_textureID = other.m_textureID;
		setTexture(textures[m_textureID]);
		setPosition(other.getPosition());
		setSpeed(other.m_SpeedX, other.m_SpeedY);
		health = other.health;
		stationary = other.stationary;
	}
	return *this;
}


void BF::Entity::bounce()
{
	move(-m_SpeedX, -m_SpeedY);
	m_SpeedX = -m_SpeedX;
	m_SpeedY = -m_SpeedY;
}

void BF::Entity::setSpeed(float x, float y)
{
	m_SpeedX = x;
	m_SpeedY = y;
}

bool BF::Entity::set_radius()
{
	sf::FloatRect bounding_box = getGlobalBounds();
	this->radius = bounding_box.width <= bounding_box.height ?
		bounding_box.width / 2.f : bounding_box.height / 2.f;
	return true;
}

void BF::Entity::update()
{
	if (stationary)
	{
		setSpeed(0.f, 0.f);
	}
	else
	{
		move(m_SpeedX, m_SpeedY);
	}
}

bool BF::Entity::intersects(const Entity& other)
{
	float x = getPosition().x;
	float y = getPosition().y;

	float other_x = other.getPosition().x;
	float other_y = other.getPosition().y;

	if (std::hypot(fabsf(x - other_x), fabsf(y - other_y)) < radius + other.radius)
	{
		return true;
	}

	return false;
}

bool BF::Entity::intersects_map(const Entity& other)
{
	if (this->getGlobalBounds().intersects(other.getGlobalBounds()))
	{
		return true;
	}
	return false;
}

bool BF::Entity::is_dead()
{
	return health > 0 ? false : true;
}

bool BF::Entity::out_of_bounds(sf::FloatRect area)
{
	sf::Vector2f pos = getPosition();
	return !area.contains(pos.x + radius / 2, pos.y + radius / 2);
}
