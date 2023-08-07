#include <pch.h>
#include <BF.h>
#include <vector>


int main()
{
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "BattleFriends", sf::Style::Fullscreen);
	SetForegroundWindow(window.getSystemHandle());
	//window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);


	BF::init(&window);
	BF::spawn_random_ent();


	//GGPOSession* ggpo = NULL;
	//GGPOErrorCode result;
	//GGPOSessionCallbacks cb;

	///* fill in all callback functions */
	//cb.begin_game = [](const char*){return true; };
	//cb.advance_frame = vw_advance_frame_callback;
	//cb.load_game_state = vw_load_game_state_callback;
	//cb.save_game_state = vw_save_game_state_callback;
	//cb.free_buffer = vw_free_buffer;
	//cb.on_event = vw_on_event_callback;

	///* Start a new session */
	//result = ggpo_start_session(&ggpo,         // the new session object
	//	&cb,           // our callbacks
	//	"test_app",    // application name
	//	2,             // 2 players
	//	sizeof(int),   // size of an input packet
	//	8001);
	
	unsigned char* buffer;
	int len;

	sf::Event event;
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			BF::process_event(event, window);
		}

		window.clear(sf::Color(20, 21, 26, 100));

		BF::save_game_state(&buffer, &len, 0, 0);
		while (true)
		{
			if (BF::update_mutex.try_lock())
			{
				BF::players.clear();
				BF::entities.clear();
				BF::projectiles.clear();
				BF::update_mutex.unlock();
				break;
			}
		}
		BF::load_game_state(buffer, len);
		BF::free_buffer(buffer);

		BF::draw();
		window.display();
	}

	BF::clear();
	return 0;
}