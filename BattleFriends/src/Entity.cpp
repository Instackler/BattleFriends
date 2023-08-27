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

	if (std::hypotf(std::abs(x - other_x), std::abs(y - other_y)) < radius + other.radius)
	{
		return true;
	}

	return false;
}

bool BF::Entity::intersects_map(const Entity& other)
{
	sf::Vector2f nearest{};
	float x_min = other.getGlobalBounds().left;
	float x_max = x_min + other.getGlobalBounds().width;
	float y_min = other.getGlobalBounds().top;
	float y_max = y_min + other.getGlobalBounds().height;
	nearest.x = std::clamp(getPosition().x, x_min, x_max);
	nearest.y = std::clamp(getPosition().y, y_min, y_max);
	sf::Vector2f diff{ nearest.x - getPosition().x, nearest.y - getPosition().y };
	float overlap = radius - std::hypotf(std::abs(diff.x), std::abs(diff.y));

	if (overlap > 0.f)
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
