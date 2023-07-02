#include <pch.h>

#include <BF.h>



BF::Projectile::Projectile()
	:Entity("resources/proj.png")
{
	health = 1;
}

void BF::Projectile::collide(Entity& other)
{
	other.health -= damage;
	this->health--;
}
