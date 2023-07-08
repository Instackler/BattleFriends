#include <pch.h>
#include <Entity.h>


int BF::Entity::s_num = 0;

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

BF::Entity::~Entity()
{
	std::cout << "Destroyed Entity" << std::endl;
}

BF::Entity::Entity(Entity&& other) noexcept
{
	m_Texture = other.m_Texture;
	std::cout << "Copied Entity" << std::endl;
}

void BF::Entity::setSpeed(float x, float y)
{
	m_SpeedX = x;
	m_SpeedY = y;
}

void BF::Entity::update()
{
	move(m_SpeedX, m_SpeedY);
}

bool BF::Entity::intersects(const Entity& other)
{
	float x = getPosition().x;
	float y = getPosition().y;

	float other_x = other.getPosition().x;
	float other_y = other.getPosition().y;

	if (hypotf(fabsf(x - other_x), fabsf(y - other_y)) < radius + other.radius)
	{
		return true;
	}

	return false;
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
	if (d < this->radius + other.radius && nvx + nvy < 0) {
		float impulse = nvx + nvy;
		this->m_SpeedX += impulse * dx / d;
		this->m_SpeedY += impulse * dy / d;
		other.m_SpeedX -= impulse * dx / d;
		other.m_SpeedY -= impulse * dy / d;
		float penetration = this->radius + other.radius - d;
		float penX = penetration * dx / d;
		float penY = penetration * dy / d;
		this->move(penX * -0.5f, penY * -0.5f);
		other.move(penX * 0.5f, penY * 0.5f);
	}
}