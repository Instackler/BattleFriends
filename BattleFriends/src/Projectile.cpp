#include <pch.h>

#include <BF.h>



BF::Projectile::Projectile(const sf::Vector2f& pos, const sf::Vector2f& speed)
	:Entity("resources/proj.png")
{
	health = 1;
	setPosition(pos.x, pos.y);
	setSpeed(speed.x * 10.f, speed.y * 10.f);
}

BF::Projectile::Projectile(Projectile&& other) noexcept
	:Entity(std::move(other))
{
	std::cout << "Projectile moved\n";
}

BF::Projectile& BF::Projectile::operator=(const Projectile&& other) noexcept
{
	std::cout << "Projectile&& =\n";
	if (this != &other)
	{
		Entity::operator=((const Entity&&)other);
		damage = other.damage;
	}
	return *this;
}

BF::Projectile& BF::Projectile::operator=(const Projectile& other)
{
	std::cout << "Projectile& =\n";
	if (this != &other)
	{
		Entity::operator=((const Entity&)other);
		damage = other.damage;
	}
	return *this;
}

void BF::Projectile::collide(Entity& other)
{
	other.health -= damage;
	this->health--;
}
