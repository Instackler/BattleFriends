#include <pch.h>
#include <BF.h>
#include <minimap.h>


std::vector<BF::Entity> BF::entities;
std::vector<BF::Player> BF::players;
std::vector<BF::Projectile> BF::projectiles;


void BF::updateEntities()
{
	for (auto&& entity : entities)
	{
		entity.update();
	}
}

void BF::updatePlayers()
{
	for (auto&& player : players)
	{
		player.update();
	}
}

void BF::init()
{
	entities.reserve(128);
	players.reserve(16);
	projectiles.reserve(128);

	minimap::init();
}

void BF::clear()
{
	entities.clear();
	players.clear();
	projectiles.clear();
}

void BF::update()
{
	updateEntities();
	updatePlayers();

	minimap::init();
}

void BF::draw_world(sf::RenderTarget& target)
{
	drawEntities(target);
	drawPlayers(target);
}

void BF::draw_minimap(sf::RenderTarget& target)
{
	minimap::draw(target);
}

void BF::drawEntities(sf::RenderTarget& target)
{
	for (auto&& entity : entities)
	{
		target.draw(entity);
	}
}

void BF::drawPlayers(sf::RenderTarget& target)
{
	for (auto&& player : players)
	{
		target.draw(player);
	}
}

void BF::checkCollisions()
{
	for (auto first = entities.begin(); first != entities.end() - 1; ++first)
	{
		// check first->health
		for (auto second = std::next(first); second != entities.end(); ++second)   //for each unique pair
		{
			// check second->health
			if (first->intersects(*second))
				first->collide(*second);
		}
	}
}

void BF::checkHits()
{
	 
}

