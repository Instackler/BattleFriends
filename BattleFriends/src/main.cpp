#include <pch.h>
#include <BF.h>

bool vw_advance_frame_callback(int flags)
{
	return true;
}

bool vw_load_game_state_callback(unsigned char* buffer, int len)
{
	return true;
}

bool vw_save_game_state_callback(unsigned char** buffer, int* len, int* checksum, int frame)
{
	return true;
}

void vw_free_buffer(void* buffer) {}

bool vw_on_event_callback(GGPOEvent* info)
{
	return true;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "BattleFriends", sf::Style::Fullscreen);
	//window.setFramerateLimit(170);

	BF::init(&window);
	BF::spawn_random_ent();

	GGPOSession* ggpo = NULL;
	GGPOErrorCode result;
	GGPOSessionCallbacks cb;

	/* fill in all callback functions */
	cb.begin_game = [](const char*){return true; };
	cb.advance_frame = vw_advance_frame_callback;
	cb.load_game_state = vw_load_game_state_callback;
	cb.save_game_state = vw_save_game_state_callback;
	cb.free_buffer = vw_free_buffer;
	cb.on_event = vw_on_event_callback;

	/* Start a new session */
	result = ggpo_start_session(&ggpo,         // the new session object
		&cb,           // our callbacks
		"test_app",    // application name
		2,             // 2 players
		sizeof(int),   // size of an input packet
		8001);


	sf::Event event;
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
			{
				window.close();
				BF::clear();
				return 0;
			}
		}

		window.clear(sf::Color(20, 21, 26, 100));
		BF::draw();
		BF::draw_debug_hud();
		window.display();
	}

	BF::clear();
	return 0;
}