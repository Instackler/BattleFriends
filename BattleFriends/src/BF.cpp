#include <BF.h>


BF::Entity::Entity(const char* filename)
	:m_SpeedX(0.f), m_SpeedY(0.f)
{
	m_texture.loadFromFile(filename);
	this->setTexture(m_texture);
}

void BF::Entity::setSpeed(float x, float y)
{
	m_SpeedX = x;
	m_SpeedY = y;
}

void BF::Entity::update()
{
	this->move(m_SpeedX, m_SpeedY);
}

void BF::Entity::bounce()
{
	m_SpeedX = -m_SpeedX;
	m_SpeedY = -m_SpeedY;
}

