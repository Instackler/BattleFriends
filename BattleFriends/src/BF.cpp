#include <pch.h>
#include <BF.h>

static std::vector<BF::Entity*> game_Entities;
static bool init = true;


void BF::updateEntities()
{
	for (int i = 0; i < game_Entities.size(); i++)
	{
		if (game_Entities[i]->health > 0)
		{
			if (game_Entities[i]->stationary)
			{
				game_Entities[i]->setSpeed(0.f, 0.f);
			}

			game_Entities[i]->update();
			game_Entities[i]->m_collided = false;
		}
	}
}

void BF::drawEntities(sf::RenderTarget& target)
{
	for (int i = 0; i < game_Entities.size(); i++)
	{
		if (game_Entities[i]->health > 0)
		{
			target.draw(*(game_Entities[i]));
		}
	}
}

void BF::checkCollisions()
{
	for (int i = 0; i < game_Entities.size(); i++)
	{
		if (game_Entities[i]->health > 0)
		{
			for (int j = 0; j < game_Entities.size(); j++)
			{
				if ((game_Entities[j]->health > 0) && i != j)
				{
					if (game_Entities[i]->intersects(*game_Entities[j]))
					{
						game_Entities[i]->collide(*game_Entities[j]);
					}
				}
			}
		}
	}
}

void BF::checkHits()
{
	 
}

