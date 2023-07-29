#include <pch.h>
#include <Entity.h>
#include <BF.h>


/*
BF::Entity::Entity(const char* filename)
{
	m_Texture.loadFromFile(filename);
	setTexture(m_Texture);

	sf::FloatRect bounding_box = getLocalBounds();
	setOrigin(bounding_box.width / 2.f, bounding_box.height / 2.f);
	radius = bounding_box.width < bounding_box.height ?
		bounding_box.width / 2.f : bounding_box.height / 2.f;

	std::cout << "Created Entity" << std::endl;
}
*/

BF::Entity::Entity(int textureID)
	:m_textureID(textureID)
{
	setTexture(textures[textureID]);

	sf::FloatRect bounding_box = getLocalBounds();
	setOrigin(bounding_box.width / 2.f, bounding_box.height / 2.f);
	radius = bounding_box.width < bounding_box.height ?
		bounding_box.width / 2.f : bounding_box.height / 2.f;

	std::cout << "Created Entity" << std::endl;
}

BF::Entity::~Entity()
{
	std::cout << "Destroyed Entity" << std::endl;
}


BF::Entity::Entity(Entity&& other) noexcept
	:sf::Sprite(std::move(other))
{
	std::cout << "Entity moved\n";
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
	std::cout << "Entity& =\n";
	if (this != &other)
	{
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

BF::Entity& BF::Entity::operator=(const Entity&& other) noexcept
{
	std::cout << "Entity&& =\n";
	if (this != &other)
	{
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

void BF::Entity::setSpeed(float x, float y)
{
	m_SpeedX = x;
	m_SpeedY = y;
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

bool BF::Entity::is_dead()
{
	return health > 0 ? false : true;
}

bool BF::Entity::out_of_bounds(sf::FloatRect area)
{
	sf::Vector2f pos = getPosition();
	return !area.contains(pos.x + radius / 2, pos.y + radius / 2);
}

/*
void BF::Entity::collide(Entity& other)
{
	if (this->stationary)
	{
		setSpeed(other.m_SpeedX * -1.f, other.m_SpeedY * -1.f);
	}

	if (other.stationary)
	{
		other.setSpeed(this->m_SpeedX * -1.f, this->m_SpeedY * -1.f);
	}

	float dx = other.getPosition().x - this->getPosition().x;
	float dy = other.getPosition().y - this->getPosition().y;
	float d = hypotf(fabsf(dx), fabsf(dy));
	float nx = dx / d;
	float ny = dy / d;
	float p = 2 * (this->m_SpeedX * nx + this->m_SpeedY * ny - other.m_SpeedX * nx - other.m_SpeedY * ny) / (this->radius + other.radius);
	this->m_SpeedX -= p * other.radius * nx;
	this->m_SpeedY -= p * other.radius * ny;
	other.m_SpeedX += p * this->radius * nx;
	other.m_SpeedY += p * this->radius * ny;
	this->move(this->m_SpeedX + 0.0001f, this->m_SpeedY + 0.0001f);
	other.move(other.m_SpeedX + 0.0001f, other.m_SpeedY + 0.0001f);
}
*/

void BF::Entity::collide(Entity& other)
{
	float dx = other.getPosition().x - this->getPosition().x;
	float dy = other.getPosition().y - this->getPosition().y;
	float d = hypotf(dx, dy);
	float nvx = (other.m_SpeedX - this->m_SpeedX) * dx / d;
	float nvy = (other.m_SpeedY - this->m_SpeedY) * dy / d;
	if (d < this->radius + other.radius && nvx + nvy < 0)
	{
		float impulse = nvx + nvy;
		this->m_SpeedX += impulse * dx / d;
		this->m_SpeedY += impulse * dy / d;
		other.m_SpeedX -= impulse * dx / d;
		other.m_SpeedY -= impulse * dy / d;
		float penetration = this->radius + other.radius - d;
		float penX = penetration * dx / d;
		float penY = penetration * dy / d;

		if (this->stationary)
		{
			other.move(penX, penY);
		}
		else if (other.stationary)
		{
			this->move(-penX, -penY);
		}
		else
		{
			this->move(penX * -0.5f, penY * -0.5f);
			other.move(penX * 0.5f, penY * 0.5f);
		}
	}
}