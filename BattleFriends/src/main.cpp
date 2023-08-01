#include <pch.h>
#include <BF.h>

std::atomic_flag running;
std::atomic<sf::Time> physics_time;

void physics_loop()
{
	sf::Clock physics_clock;
	do
	{
		BF::update();
		sf::sleep(sf::milliseconds(1));
		physics_time.store(physics_clock.restart());
	} while (running.test());
}

int main()
{
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "BattleFriends", sf::Style::Fullscreen);
	window.setFramerateLimit(200);

	BF::init(&window);
	BF::spawn_random_ent();

	#ifdef SHOW_FPS
	sf::Font font;
	BF::loadResource(Raleway_Semibold, "TTF", font);

	sf::Text fpsCounter;
	fpsCounter.setFont(font);
	fpsCounter.setString("Hello world");
	sf::Clock fps_clock;
	int frames = 0;

	sf::Text info;
	info.setFont(font);
	info.move(0.f, sf::VideoMode::getDesktopMode().height / 20.f * 0.7f);

	sf::Text Esc_hint;
	Esc_hint.setFont(font);
	Esc_hint.setCharacterSize(40);
	Esc_hint.move(0.f, sf::VideoMode::getDesktopMode().height - 141.f);
	Esc_hint.setString("Move: W, A, S, D\nShoot: LMB\nPress Esc to exit");
	#endif // SHOW_FPS

	bool drawn_once = false;
	std::thread physics_thread(physics_loop);
	sf::Event event;
	running.test_and_set();

	sf::Clock frame_time;
	sf::Time frame_end;
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
			{
				running.clear();
				window.close();
				physics_thread.join();
				BF::clear();
				return 0;
			}
		}

		window.clear(sf::Color(20, 21, 26, 100));
		
		//BF::update();
		BF::draw();

		#ifdef SHOW_FPS
		window.draw(fpsCounter);
		window.draw(info);
		window.draw(Esc_hint);

		frames++;
		if (frames == 10)
		{
			fpsCounter.setString(std::to_string(10.f / fps_clock.restart().asSeconds()));
			frames = 0;
			info.setString("Frame time: " + std::to_string(frame_end.asMicroseconds()) + "us" + "\n" +
						   "Physics time: " + std::to_string(physics_time.load().asMicroseconds()) + "us" + "\n" +
						   "Entities: " + std::to_string(BF::get_Entity_count()) + "\n" +
						   "Projectiles: " + std::to_string(BF::get_Projectile_count()));
		}
		#endif // SHOW_FPS
		
		window.display();
		frame_end = frame_time.restart();
	}

	running.clear();
	physics_thread.join();
	BF::clear();
	return 0;
}