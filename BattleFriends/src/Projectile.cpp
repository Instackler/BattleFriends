#include <pch.h>
#include <BF.h>


BF::Projectile::Projectile(const sf::Vector2f& pos, const sf::Vector2f& speed)
	:Entity("proj")
{
	health = 1;
	setPosition(pos.x, pos.y);
	setSpeed(speed.x * 10.f, speed.y * 10.f);
}

void BF::Projectile::collide(Entity& other)
{
	other.health -= damage;
	this->health--;
}

void BF::Projectile::collide_with_map()
{
	this->health = 0;
}
