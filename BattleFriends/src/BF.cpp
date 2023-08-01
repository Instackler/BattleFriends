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
sf::Sprite background;
std::unordered_map<int, sf::Texture> BF::textures;
std::mutex update_mutex;

void BF::updateEntities()
{
	for (auto&& entity : entities)
	{
		if (entity.out_of_bounds())
		{
			entity.bounce();
		}
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

void BF::loadTextures()
{
	sf::Texture tex;
	EnumResourceNamesA(NULL, "PNG",
		[](HMODULE hModule, LPCTSTR lpType, LPTSTR lpName, LONG_PTR lParam) -> BOOL
		{
			BF::loadResource((int)lpName, lpType, *((sf::Texture*)lParam));
			textures.emplace((int)lpName, *((sf::Texture*)lParam));
			return true; // Continue enumeration
		},
		(LONG_PTR)&tex);
}

void BF::updateProjectiles()
{
	for (auto&& projectile : projectiles)
	{
		projectile.update();
	}
}

void BF::init(sf::RenderTarget* target)
{
	entities.reserve(100);
	players.reserve(32);
	projectiles.reserve(1000);

	loadTextures();

	float background_scale = 10.f;
	background.setTextureRect(sf::IntRect{0, 0, MAP_WIDTH / (int)background_scale, MAP_HEIGHT / (int)background_scale});
	textures[bedrock].setRepeated(true);
	background.setTexture(textures[bedrock]);
	background.setScale(background_scale, background_scale);
	background.setColor(sf::Color(80, 80, 80, 255));

	screen_params = sf::VideoMode::getDesktopMode();
	default_target = target;
	default_view = target->getDefaultView();
	player_view.setSize(sf::Vector2f(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
	player_view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

	minimap::init();
}

void BF::clear()
{
	entities.clear();
	players.clear();
	projectiles.clear();
	textures.clear();
}

void BF::update()
{
	const std::lock_guard<std::mutex> update_lock(update_mutex);
	updateEntities();
	updatePlayers();
	updateProjectiles();
	checkCollisions();
	checkHits();
}

void BF::draw(sf::RenderTarget& target)
{
	//const std::lock_guard<std::mutex> draw_lock(update_mutex);
	while (true)
	{
		bool lockAcquired = update_mutex.try_lock();
		if (lockAcquired)
		{
			player_view.setCenter(players.size() > 0 ? players[0].getPosition() : sf::Vector2f{MAP_WIDTH / 2.f, MAP_HEIGHT / 2.f});
			target.setView(player_view);
			target.draw(background);
			drawEntities(target);
			drawPlayers(target);
			drawProjectiles(target);
			target.setView(default_view);
			minimap::draw(target);

			update_mutex.unlock();
			break;
		}
		// wait before trying again
	}
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

	for (auto&& player : players)
	{
		for (auto&& entity : entities)
		{
			if (player.intersects(entity))
			{
				entity.collide(player);
			}
		}
	}
}

void BF::checkHits()
{
	std::erase_if(projectiles, [](BF::Projectile& projectile) { return projectile.out_of_bounds(); });
	//std::erase_if(players, [](BF::Player& player) { return player.out_of_bounds(); });
	//std::erase_if(entities, [](BF::Entity& entity) { return entity.out_of_bounds(); });

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
	players.emplace_back(logo);
	players[0].move(MAP_WIDTH / 2, MAP_HEIGHT / 2);
	for (int i = 0; i < ENTITY_NUM; i++)
	{
		entities.emplace_back(logo);
		entities[i].setColor(sf::Color{255, 100, 100});
		entities[i].move(rand() % MAP_WIDTH, rand() % MAP_HEIGHT);
		if (!(rand() % 10))
		{
			entities[i].stationary = true;
		}
		else
		{
			entities[i].setSpeed(((rand() % 11) - 5) / 20.0f, (rand() % 11) / 10.0f);
		}
	}
}

size_t BF::get_Entity_count()
{
	return entities.size();
}

size_t BF::get_Projectile_count()
{
	return projectiles.size();
}
