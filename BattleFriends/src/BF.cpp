#include <pch.h>
#include <BF.h>
#include <minimap.h>

sf::RenderTarget* BF::default_target = nullptr;
std::vector<BF::Entity> BF::entities;
std::vector<BF::Player> BF::players;
std::vector<BF::Projectile> BF::projectiles;
sf::VideoMode BF::screen_params;
sf::View player_view;
sf::View default_view;

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

void BF::init(sf::RenderTarget* target)
{
	entities.reserve(64);
	players.reserve(16);
	projectiles.reserve(128);

	minimap::init();

	screen_params = sf::VideoMode::getDesktopMode();
	default_target = target;
	default_view = target->getDefaultView();
	player_view.setSize(sf::Vector2f(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
	player_view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
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
	player_view.setCenter(players[0].getPosition());
	target.setView(player_view);
	drawEntities(target);
	drawPlayers(target);
	drawProjectiles(target);
	target.setView(default_view);
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
	for (auto&& player : players)
	{
		for (auto&& projectile : projectiles)
		{
			if (player.intersects(projectile))
			{
				projectile.collide(player);
			}
		}
	}

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

	std::erase_if(projectiles,[](BF::Projectile& projectile) { return projectile.is_dead(); });
	std::erase_if(players, [](BF::Player& player) { return player.is_dead(); });
	std::erase_if(entities, [](BF::Entity& entity) { return entity.is_dead(); });
}

void BF::spawn_random_ent()
{
	srand(time(NULL));
	players.emplace_back("resources/logo.png");
	players.emplace_back("resources/logo.png");
	players[1].move(200.f, 200.f);
	for (int i = 0; i < ENTITY_NUM; i++)
	{
		entities.emplace_back("resources/logo.png");
		if (i < 4)
		{
			entities[i].move((i + 2) * 220.f, 100.f);
			entities[i].setSpeed(((rand() % 11) - 5) / 20.0f, (rand() % 11) / 10.0f);
		}
		else
		{
			entities[i].move((i - 3) * 210.f, 600.f);
			entities[i].setSpeed(((rand() % 11) - 5) / 20.0f, ((rand() % 11) - 10) / 10.0f);
		}
	}
	entities[1].stationary = true;
	entities[4].stationary = true;
}

size_t BF::get_Entity_count()
{
	return entities.size();
}

size_t BF::get_Projectile_count()
{
	return projectiles.size();
}
