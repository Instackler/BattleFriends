#pragma once
#include <loadTexture.h>
#include <Entity.h>
#include <Player.h>
#include <Projectile.h>


namespace BF
{
	extern std::vector<BF::Entity> entities;

	void init();
	void updateEntities();
	void drawEntities(sf::RenderTarget& target);
	void checkCollisions();
	void checkHits();
}
