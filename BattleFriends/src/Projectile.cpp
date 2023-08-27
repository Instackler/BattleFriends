#include <pch.h>

#include <BF.h>



BF::Projectile::Projectile(const Projectile& other)
	:Entity(other)
{
	damage = other.damage;
}

BF::Projectile::Projectile(const sf::Vector2f& pos, const sf::Vector2f& speed)
	:Entity("proj")
{
	health = 1;
	setPosition(pos.x, pos.y);
	setSpeed(speed.x * 10.f, speed.y * 10.f);
}

BF::Projectile::Projectile(Projectile&& other) noexcept
	:Entity(std::move(other))
{
	// TODO: add logging		std::cout << "Projectile moved\n";
	damage = other.damage;
}

BF::Projectile& BF::Projectile::operator=(Projectile&& other) noexcept
{
	// TODO: add logging		std::cout << "Projectile&& =\n";
	if (this != &other)
	{
		Entity::operator=(std::move(other));
		damage = other.damage;
	}
	return *this;
}

BF::Projectile& BF::Projectile::operator=(const Projectile& other)
{
	// TODO: add logging		std::cout << "Projectile& =\n";
	if (this != &other)
	{
		Entity::operator=(other);
		damage = other.damage;
	}
	return *this;
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
