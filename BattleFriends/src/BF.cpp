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
		if(!entity.is_dead())
			entity.update();
	}
}

void BF::updatePlayers()
{
	for (auto&& player : players)
	{
		if (!player.is_dead())
			player.update();
	}
}

void BF::updateProjectiles()
{
	for (auto&& projectile : projectiles)
	{
		if (!projectile.is_dead())
			projectile.update();
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
	updateProjectiles();
	checkCollisions();
	checkHits();
}

void BF::draw(sf::RenderTarget& target)
{
	drawEntities(target);
	drawPlayers(target);
	drawProjectiles(target);
	minimap::draw(target);
}

void BF::drawEntities(sf::RenderTarget& target)
{
	for (auto&& entity : entities)
	{
		//if (entity.health > 0)
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

void BF::drawProjectiles(sf::RenderTarget& target)
{
	for (auto&& projectile : projectiles)
	{
		target.draw(projectile);
	}
}

void BF::checkCollisions()
{
	for (int i = 0; i < ((int)entities.size() - 1); i++)
	{
		for (int j = i + 1; j < entities.size() ; j++)   //for each unique pair
		{
			if (entities[i].intersects(entities[j]))
				entities[i].collide(entities[j]);
		}
	}
}

void BF::checkHits()
{
	for (auto&& entity : entities)
	{
		for (auto&& projectile : projectiles)
		{
			if (entity.intersects(projectile))
			{
				projectile.collide(entity);
			}
		}
	}

	std::erase_if(projectiles,[](BF::Projectile& proj) { return proj.is_dead(); });
}

void BF::spawn_random_ent()
{
	srand(time(NULL));
	BF::players.emplace_back("resources/logo.png");
	for (int i = 0; i < ENTITY_NUM; i++)
	{
		BF::entities.emplace_back("resources/logo.png");
		if (i < 4)
		{
			BF::entities[i].move((i + 2) * 220.f, 100.f);
			BF::entities[i].setSpeed(((rand() % 11) - 5) / 20.0f, (rand() % 11) / 10.0f);
		}
		else
		{
			BF::entities[i].move((i - 3) * 210.f, 600.f);
			BF::entities[i].setSpeed(((rand() % 11) - 5) / 20.0f, ((rand() % 11) - 10) / 10.0f);
		}
	}
	BF::entities[1].stationary = true;
	BF::entities[4].stationary = true;
}

size_t BF::get_Entity_count()
{
	return entities.size();
}

size_t BF::get_Projectile_count()
{
	return projectiles.size();
}
