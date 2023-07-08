#pragma once
#include <loadTexture.h>
#include <Entity.h>
#include <Player.h>
#include <Projectile.h>


namespace BF
{
	extern std::vector<BF::Entity> entities;
	extern std::vector<BF::Player> players;
	extern std::vector<BF::Projectile> projectiles;

	void updateEntities();
	void updatePlayers();

	void drawEntities(sf::RenderTarget& target);
	void drawPlayers(sf::RenderTarget& target);

	void init();
	void update();
	void draw_world(sf::RenderTarget& target);
	void draw_minimap(sf::RenderTarget& target);
	void clear();

	void checkCollisions();
	void checkHits();
}
