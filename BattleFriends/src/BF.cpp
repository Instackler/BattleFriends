#include <pch.h>
#include <BF.h>


std::vector<BF::Entity> BF::entities;


void BF::init()
{
	entities.reserve(256);
}

void BF::updateEntities()
{
	for (auto&& entity : entities)
	{
		if (entity.health > 0)
		{
			if (entity.stationary)
			{
				entity.setSpeed(0.f, 0.f);
			}

			entity.update();
		}
	}
}

void BF::drawEntities(sf::RenderTarget& target)
{
	for (auto&& entity : entities)
	{
		if (entity.health > 0)
		{
			target.draw(entity);
		}
	}
}

void BF::checkCollisions()
{
	for (auto first = entities.begin(); first != entities.end() - 1; ++first)
	{
		// check first->health
		for (auto second = std::next(first); second != entities.end(); ++second)
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

