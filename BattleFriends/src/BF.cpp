#include <pch.h>
#include <BF.h>
#include <minimap.h>

// Game state and inputs
std::vector<BF::Entity> BF::entities;
std::vector<BF::Player> BF::players;
std::vector<BF::Projectile> BF::projectiles;
std::vector<BF::player_inputs> BF::game_inputs;

// renderer variables
sf::RenderTarget* BF::default_target = nullptr;
sf::Sprite background;
std::unordered_map<int, sf::Texture> BF::textures;
std::atomic_flag has_focus;
BF::Player* BF::local_player = nullptr;

// Physics thread vars
std::atomic_flag running;
std::atomic<sf::Time> physics_time;
std::thread* physics_thread_ptr = nullptr;
std::mutex BF::update_mutex;


void BF::physics_loop()
{
	static sf::Clock physics_clock;
	do
	{
		BF::update();
		sf::sleep(sf::milliseconds(3));
		physics_time.store(physics_clock.restart());
	} while (running.test());
}

void BF::process_event(sf::Event& event, sf::RenderWindow& window)
{
	if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
	{
		window.close();
	}
	if (event.type == sf::Event::LostFocus)
	{
		has_focus.clear();
	}
	if (event.type == sf::Event::GainedFocus)
	{
		has_focus.test_and_set();
	}
}

void BF::checkInputs()
{
	if (has_focus.test())
	{
		game_inputs[0].up = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
		game_inputs[0].left = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
		game_inputs[0].down = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
		game_inputs[0].right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
		game_inputs[0].shoot = sf::Mouse::isButtonPressed(sf::Mouse::Left);
		game_inputs[0].mouse_pos = sf::Mouse::getPosition();
	}
	else
	{
		game_inputs[0].up = false;
		game_inputs[0].left = false;
		game_inputs[0].down = false;
		game_inputs[0].right = false;
		game_inputs[0].shoot = false;
	}
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

	default_target = target;

	minimap::init();

	running.test_and_set();
	static std::thread physics_thread(BF::physics_loop);
	physics_thread_ptr = &physics_thread;
	has_focus.test_and_set();
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
	static sf::View player_view({ 0.f, 0.f , 1920, 1080 });

	while (true)
	{
		if (update_mutex.try_lock())
		{
			player_view.setCenter(players.size() > 0 ? players[0].getPosition() : player_view.getCenter());
			target.setView(player_view);
			target.draw(background);
			drawEntities(target);
			drawPlayers(target);
			drawProjectiles(target);
			target.setView(target.getDefaultView());
			minimap::draw(target);
			draw_debug_hud();

			update_mutex.unlock();
			break;
		}
		//TODO: wait before trying again
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
	for (int i = 0; i < (int)entities.size() - 1; i++)
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

	for (int i = 0; i < (int)players.size() - 1; i++)
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
	//players.emplace_back(logo);
	players[0].move(MAP_WIDTH / 2.f, MAP_HEIGHT / 2.f);
	//players[1].move(MAP_WIDTH / 2.f, MAP_HEIGHT / 2.f);
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

bool init_debug_hud(sf::Text& info, sf::Text& Esc_hint)
{
	static sf::Font font;
	BF::loadResource(Raleway_Semibold, "TTF", font);

	info.setFont(font);
	info.setCharacterSize(BF::default_target->getSize().y * HUD_SCALE);
	info.setString("Initializing...");

	Esc_hint.setFont(font);
	Esc_hint.setCharacterSize(BF::default_target->getSize().y * HUD_SCALE);
	Esc_hint.setLineSpacing(1.15f);
	Esc_hint.move(0.f, (float)BF::default_target->getSize().y - (float)Esc_hint.getCharacterSize() * pow(Esc_hint.getLineSpacing(), 2.f) * 3.f);
	Esc_hint.setString("Move: W, A, S, D\nShoot: LMB\nPress Esc to exit\n\n\n\n\n\n\n\n15515");
	
	return true;
}

void BF::draw_debug_hud(sf::RenderTarget& target)
{
	static int frames = 0;
	static sf::Text info;
	static sf::Text Esc_hint;
	static sf::Time frame_end;
	static sf::Clock fps_clock;
	static bool init = init_debug_hud(info, Esc_hint);

	target.draw(info);
	target.draw(Esc_hint);

	frames++;
	if (frames == 10)
	{
		frame_end = fps_clock.restart();
		frames = 0;
		info.setString(std::to_string(10.f / frame_end.asSeconds()) + "\n" +
			"Frame time: " + std::to_string(frame_end.asMicroseconds() / 10) + "us" + "\n" +
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
