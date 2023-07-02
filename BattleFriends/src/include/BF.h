#pragma once
#include <Entity.h>
#include <Player.h>
#include <Projectile.h>


namespace BF
{
	void updateEntities();
	void drawEntities(sf::RenderTarget& target);
	void checkCollisions();
	void checkHits();
}