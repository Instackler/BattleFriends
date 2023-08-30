#include <pch.h>
#include <BF.h>
#include <minimap.h>

// Game state and inputs
std::vector<BF::Entity> BF::entities;
std::vector<BF::Player> BF::players;
std::vector<BF::Projectile> BF::projectiles;
BF::player_inputs BF::game_inputs[PLAYER_COUNT] = {};
BF::player_inputs local_inputs = {};

// Non-game state
std::vector<BF::Entity> BF::map_objects;
int BF::MAP_WIDTH = 0;
int BF::MAP_HEIGHT = 0;

// ggpo variables
GGPOSession* BF::session;
GGPOPlayer ggpo_players[PLAYER_COUNT] = {};
GGPOPlayerHandle ggpo_player_handles[PLAYER_COUNT];
GGPOPlayerHandle local_ggpo_player;
int local_player_index = 0;

// renderer variables
sf::RenderTarget* BF::default_target = nullptr;
sf::Sprite background;
std::unordered_map<std::string, sf::Texture> BF::textures;
std::atomic_flag has_focus;

// Physics thread vars
std::atomic_flag running;
std::atomic<sf::Time> physics_time;
std::thread* physics_thread_ptr = nullptr;
std::mutex update_mutex;

/*
void BF::physics_loop()
{
	static sf::Clock physics_clock;
	static sf::Clock frame_clock;
	do
	{
		physics_clock.restart();
		frame_clock.restart();
		ggpo_idle(session, tick_time);
		{
			const std::lock_guard<std::mutex> physics_lock(update_mutex);
			run();
		}
		Sleep(tick_time - frame_clock.restart().asMilliseconds());
		physics_time.store(physics_clock.restart());
	} while (running.test());
}
*/

void BF::physics_loop()
{
	static sf::Clock physics_clock;
	auto time_between_frames = std::chrono::microseconds(std::chrono::seconds(1)) / tick_rate;
	auto target_tp = std::chrono::steady_clock::now();
	std::chrono::microseconds threshold(50);
	do
	{
		physics_clock.restart();
		target_tp += time_between_frames;  // calculate target point in time
		// do stuff
		ggpo_idle(session, tick_time - 1);
		{
			const std::lock_guard<std::mutex> physics_lock(update_mutex);
			run();
		}
		std::this_thread::sleep_until(target_tp - threshold);  // sleep short of that time point
		while (std::chrono::steady_clock::now() < target_tp) {} // busy wait
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
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E && has_focus.test())
	{
		spawn_random_ent();
	}
	for (int i = 0; i < PLAYER_COUNT; i++)
	{
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num1 + i && has_focus.test())
		{
			#ifndef DISABLE_GGPO
			start_ggpo(i + 1);
			#endif // !DISABLE_GGPO
		}
	}
}

void BF::updateInputs()
{
	if (has_focus.test())
	{
		local_inputs.up = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
		local_inputs.left = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
		local_inputs.down = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
		local_inputs.right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
		local_inputs.shoot = sf::Mouse::isButtonPressed(sf::Mouse::Left);
		local_inputs.mouse_pos = sf::Mouse::getPosition(*(sf::RenderWindow*)BF::default_target); //TODO: change BF::init() argument to sf::RenderWindow*
	}
	else
	{
		local_inputs.up = false;
		local_inputs.left = false;
		local_inputs.down = false;
		local_inputs.right = false;
		local_inputs.shoot = false;
	}
}

void BF::updateEntities()
{
	for (auto&& entity : entities)
	{
		entity.update();
	}
}

void BF::updatePlayers()
{
	int i = 0;
	for (auto&& player : players)
	{
		#ifndef DISABLE_GGPO
		player.update(game_inputs[i++]);
		#else
		player.update(local_inputs);
		#endif // !DISABLE_GGPO
	}
}

void BF::loadTextures()
{
	sf::Texture tex;
	EnumResourceNamesA(NULL, "PNG",
		[](HMODULE hModule, LPCTSTR lpType, LPTSTR lpName, LONG_PTR lParam) -> BOOL
		{
			BF::loadResource(lpName, lpType, *((sf::Texture*)lParam));
			std::string name{ lpName };
			std::transform(name.begin(), name.end(), name.begin(), ::tolower);
			textures.emplace(name, *((sf::Texture*)lParam));
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
	TIMECAPS tc;
	timeGetDevCaps(&tc, sizeof(tc));
	timeBeginPeriod(tc.wPeriodMin);
	entities.reserve(100);
	players.reserve(32);
	projectiles.reserve(1000);

	BF::loadTextures();
	BF::loadMap();

	///////////////////////////////////// load the map
	float background_scale = 10.f;
	background.setTextureRect(sf::IntRect{ 0, 0, MAP_WIDTH / (int)background_scale, MAP_HEIGHT / (int)background_scale });
	textures["bedrock"].setRepeated(true);
	background.setTexture(textures["bedrock"]);
	background.setScale(background_scale, background_scale);
	background.setColor(sf::Color(80, 80, 80, 255));
	/////////////////////////////////////////////////////

	default_target = target;
	minimap::init();

	//BF::spawn_random_ent();
	for (int i = 0; i < PLAYER_COUNT; i++)
	{
		players.emplace_back("logo");
		players[i].move(MAP_WIDTH / 2.f + i * 160, MAP_HEIGHT / 2.f);
		players[i].setColor(sf::Color{ (sf::Uint8)(255 - i * 20), (sf::Uint8)(127 + i * 20), (sf::Uint8)(255 - i * 50) });
	}

	running.test_and_set();
	static std::thread physics_thread(BF::physics_loop);
	physics_thread_ptr = &physics_thread;
	has_focus.test_and_set();
}

void BF::clear()
{
	running.clear();
	physics_thread_ptr->join();
	if (session != nullptr)
	{
		ggpo_close_session(session);
		ggpo_deinitialize_winsock();
	}
	TIMECAPS tc;
	timeGetDevCaps(&tc, sizeof(tc));
	timeEndPeriod(tc.wPeriodMin);

	entities.clear();
	players.clear();
	projectiles.clear();
	textures.clear();
}

void BF::advance()
{
	updatePlayers();
	updateEntities();
	updateProjectiles();
	checkCollisions();
	checkHits();
	ggpo_advance_frame(session);
}

void BF::run()
{
	int flags = 0;
	updateInputs();
	#ifndef DISABLE_GGPO
	if (!ggpo_add_local_input(session, local_ggpo_player, &local_inputs, sizeof(player_inputs)))
	#endif // !DISABLE_GGPO
	{
		#ifndef DISABLE_GGPO
		if (!ggpo_synchronize_input(session, game_inputs, PLAYER_COUNT * sizeof(player_inputs), &flags))
		#endif // !DISABLE_GGPO
		{
			advance();
		}
	}
}

void BF::draw(sf::RenderTarget& target)
{
	static sf::View player_view({ 0.f, 0.f , 3200, 1800 });

	while (true)
	{
		if (update_mutex.try_lock())
		{
			player_view.setCenter(players.size() > 0 ? players[local_player_index].getPosition() : player_view.getCenter()); //change
			target.setView(player_view);
			target.draw(background);
			drawMap(target);
			drawEntities(target);
			drawPlayers(target);
			drawProjectiles(target);
			target.setView(target.getDefaultView());
			minimap::draw(target);
			draw_debug_hud(target);

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

void BF::drawMap(sf::RenderTarget& target)
{
	for (auto&& map_object : map_objects)
	{
		target.draw(map_object);
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
	for (auto&& player : players) //check player - entity collisions
	{
		for (auto&& entity : entities)
		{
			if (player.intersects(entity))
			{
				player.collide(entity);
			}
		}
	}

	for (int i = 0; i < (int)players.size() - 1; i++) //check player - player collisions
	{
		for (int j = i + 1; j < players.size(); j++)   //for each unique pair
		{
			if (players[i].intersects(players[j]))
			{
				players[i].collide(players[j]);
			}
		}
	}

	for (auto&& projectile : projectiles) //check projectile - map collisions
	{
		for (auto&& map_object : map_objects)
		{
			if (projectile.intersects_map(map_object))
			{
				projectile.collide_with_map();
			}
		}
	}

	for (auto&& player : players) //check player - map collisions
	{
		for (auto&& map_object : map_objects)
		{
			player.collide_with_map(map_object);
		}
	}

	for (auto&& player : players) //check player - projectile collisions
	{
		for (auto&& projectile : projectiles)
		{
			if (player.intersects(projectile))
			{
				projectile.collide(player);
			}
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

	std::erase_if(projectiles, [](BF::Projectile& projectile) { return projectile.out_of_bounds() || projectile.is_dead(); });
	std::erase_if(players, [](BF::Player& player) { return player.is_dead(); });
	std::erase_if(entities, [](BF::Entity& entity) { return entity.is_dead(); });
}

void BF::start_ggpo(int player_num)
{
	GGPOSessionCallbacks cb{};
	cb.begin_game = BF::begin_game;
	cb.advance_frame = BF::advance_frame_callback;
	cb.load_game_state = BF::load_game_state;
	cb.save_game_state = BF::save_game_state;
	cb.free_buffer = BF::free_buffer;
	cb.on_event = BF::on_event;

	ggpo_initialize_winsock();
	const int base_port = 6165;
	int local_port = base_port + player_num;

	const std::lock_guard<std::mutex> start_ggpo_lock(update_mutex);
	ggpo_start_session(&session, &cb, "BattleFriends", PLAYER_COUNT, sizeof(player_inputs), local_port);

	ggpo_set_disconnect_timeout(session, 0);

	for (int i = 0; i < PLAYER_COUNT; i++)
	{
		ggpo_players[i].player_num = i + 1;
		ggpo_players[i].size = sizeof(GGPOPlayer);
		if (player_num == i + 1)
		{
			ggpo_players[i].type = GGPO_PLAYERTYPE_LOCAL;
			ggpo_add_player(session, &ggpo_players[i], &ggpo_player_handles[i]);
			ggpo_set_frame_delay(session, ggpo_player_handles[i], 2);
			local_ggpo_player = ggpo_player_handles[i];
			local_player_index = i;
		}
		else
		{
			ggpo_players[i].type = GGPO_PLAYERTYPE_REMOTE;
			strcpy_s(ggpo_players[i].u.remote.ip_address, sizeof("127.0.0.1"), "127.0.0.1");
			ggpo_players[i].u.remote.port = base_port + (i + 1);
			ggpo_add_player(session, &ggpo_players[i], &ggpo_player_handles[i]);
		}
	}
}

void BF::spawn_random_ent()
{
	//srand(time(0));
	for (int i = 0; i < ENTITY_NUM; i++)
	{
		entities.emplace_back("logo");
		entities[i].setColor(sf::Color{ 255, 100, 100 });
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
	BF::loadResource("Raleway_Semibold", "TTF", font);

	info.setFont(font);
	info.setCharacterSize(BF::default_target->getSize().y * HUD_SCALE);
	info.setString("Initializing...");

	Esc_hint.setFont(font);
	Esc_hint.setCharacterSize(BF::default_target->getSize().y * HUD_SCALE);
	Esc_hint.setLineSpacing(1.15f);
	Esc_hint.move(0.f, (float)BF::default_target->getSize().y - (float)Esc_hint.getCharacterSize() * pow(Esc_hint.getLineSpacing(), 2.f) * 3.f);
	Esc_hint.setString("Move: W, A, S, D\nShoot: LMB\nPress Esc to exit");

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

sf::Vector2f normalize(const sf::Vector2f& source)
{
	float length = std::hypotf(source.x, source.y);
	if (length != 0)
	{
		return sf::Vector2f{ source.x / length, source.y / length };
	}
	else
	{
		return source;
	}
}
