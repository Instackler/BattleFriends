#include <pch.h>
#include <BF.h>
#include <minimap.h>

sf::RenderTarget* BF::default_target = nullptr;
std::vector<BF::Entity> BF::entities;
std::vector<BF::Player> BF::players;
std::vector<BF::Projectile> BF::projectiles;
std::vector<BF::player_inputs> BF::game_inputs;
sf::VideoMode BF::screen_params;
sf::View player_view;
sf::View default_view;
sf::Sprite background;
std::unordered_map<int, sf::Texture> BF::textures;
std::mutex update_mutex;
std::atomic_flag BF::running;
std::atomic<sf::Time> BF::physics_time;
std::thread* physics_thread_ptr;


void BF::physics_loop()
{
	static sf::Clock physics_clock;
	do
	{
		BF::update();
		sf::sleep(sf::milliseconds(1));
		sf::sleep(sf::milliseconds(1));
		physics_time.store(physics_clock.restart());
	} while (running.test());
}

void BF::checkInputs()
{
	game_inputs[0].up = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
	game_inputs[0].left = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	game_inputs[0].down = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
	game_inputs[0].right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	game_inputs[0].shoot = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	game_inputs[0].mouse_pos = sf::Mouse::getPosition();
}

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
	int i = 0;
	for (auto&& player : players)
	{
		player.update(game_inputs[i++]);
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
	game_inputs.reserve(32);

	BF::loadTextures();

	///////////////////////////////////// load the map
	float background_scale = 10.f;
	background.setTextureRect(sf::IntRect{0, 0, MAP_WIDTH / (int)background_scale, MAP_HEIGHT / (int)background_scale});
	textures[bedrock].setRepeated(true);
	background.setTexture(textures[bedrock]);
	background.setScale(background_scale, background_scale);
	background.setColor(sf::Color(80, 80, 80, 255));
	/////////////////////////////////////////////////////

	screen_params = sf::VideoMode::getDesktopMode();
	default_target = target;
	default_view = target->getDefaultView();
	player_view.setSize(sf::Vector2f(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
	player_view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

	minimap::init();

	running.test_and_set();
	static std::thread physics_thread(BF::physics_loop);
	physics_thread_ptr = &physics_thread;
}

void BF::clear()
{
	running.clear();
	physics_thread_ptr->join();

	entities.clear();
	players.clear();
	projectiles.clear();
	game_inputs.clear();
	textures.clear();
}

void BF::update()
{
	const std::lock_guard<std::mutex> update_lock(update_mutex);
	BF::checkInputs();
	updatePlayers();
	updateEntities();
	updateProjectiles();
	checkCollisions();
	checkHits();
}

void BF::draw(sf::RenderTarget& target)
{
	/*
	const std::lock_guard<std::mutex> draw_lock(update_mutex);
	player_view.setCenter(players.size() > 0 ? players[0].getPosition() : sf::Vector2f{ MAP_WIDTH / 2.f, MAP_HEIGHT / 2.f });
	target.setView(player_view);
	target.draw(background);
	drawEntities(target);
	drawPlayers(target);
	drawProjectiles(target);
	target.setView(default_view);
	minimap::draw(target);
	*/

	while (true)
	{
		if (update_mutex.try_lock())
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
	for (int i = 0; i < (entities.size() - 1); i++)
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

	for (int i = 0; i < (players.size() - 1); i++)
	{
		for (int j = i + 1; j < players.size(); j++)   //for each unique pair
		{
			if (players[i].intersects(players[j]))
				players[i].collide(players[j]);
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
	players.emplace_back(logo);
	players[0].move(MAP_WIDTH / 2.f, MAP_HEIGHT / 2.f);
	players[1].move(MAP_WIDTH / 2.f, MAP_HEIGHT / 2.f);
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

void BF::draw_debug_hud(sf::RenderTarget& target)
{
	static bool init = true;
	static int frames = 0;
	static sf::Font font;
	static sf::Text fpsCounter;
	static sf::Text info;
	static sf::Text Esc_hint;
	static sf::Time frame_end;
	static sf::Clock fps_clock;

	if (init)
	{
		BF::loadResource(Raleway_Semibold, "TTF", font);
		fpsCounter.setFont(font);
		fpsCounter.setString("Initializing...");

		info.setFont(font);
		info.move(0.f, sf::VideoMode::getDesktopMode().height / 20.f * 0.7f);
		info.setString("Initializing...");

		Esc_hint.setFont(font);
		Esc_hint.setCharacterSize(40);
		Esc_hint.move(0.f, sf::VideoMode::getDesktopMode().height - 141.f);
		Esc_hint.setString("Move: W, A, S, D\nShoot: LMB\nPress Esc to exit");

		init = false;
	}

	target.draw(fpsCounter);
	target.draw(info);
	target.draw(Esc_hint);

	frames++;
	if (frames == 10)
	{
		frame_end = fps_clock.restart();
		fpsCounter.setString(std::to_string(10.f / frame_end.asSeconds()));
		frames = 0;
		info.setString("Frame time: " + std::to_string(frame_end.asMicroseconds() / 10) + "us" + "\n" +
			"Physics time: " + std::to_string(physics_time.load().asMicroseconds()) + "us" + "\n" +
			"Entities: " + std::to_string(BF::get_Entity_count()) + "\n" +
			"Projectiles: " + std::to_string(BF::get_Projectile_count()));
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
