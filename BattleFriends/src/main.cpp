#include <pch.h>
#include <BF.h>

std::atomic_bool running;

void physics_loop()
{
	do
	{
		BF::update();
		//sf::sleep(sf::milliseconds(1));
	} while (running);
}

int main()
{
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "BattleFriends", sf::Style::Fullscreen);
	window.setFramerateLimit(240);

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
	sf::Text Entities_count;
	Entities_count.setFont(font);
	Entities_count.move(0.f, sf::VideoMode::getDesktopMode().height / 20.f * 2.f);
	sf::Text Projectiles_count;
	Projectiles_count.setFont(font);
	Projectiles_count.move(0.f, sf::VideoMode::getDesktopMode().height / 20.f * 3.f);
	sf::Text Esc_hint;
	Esc_hint.setFont(font);
	Esc_hint.setCharacterSize(40);
	Esc_hint.move(0.f, sf::VideoMode::getDesktopMode().height - 141.f);
	Esc_hint.setString("Move: W, A, S, D\nShoot: LMB\nPress Esc to exit");
	#endif // SHOW_FPS

	bool drawn_once = false;
	running.store(true);
	std::thread physics_thread(physics_loop);
	sf::Event event;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
			{
				running = false;
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
		window.draw(Entities_count);
		window.draw(Projectiles_count);
		window.draw(Esc_hint);

		frames++;
		if (frames == 10)
		{
			fpsCounter.setString(std::to_string(10.f / fps_clock.restart().asSeconds()));
			frames = 0;
			Entities_count.setString("Entities: " + std::to_string(BF::get_Entity_count()));
			Projectiles_count.setString("Projectiles: " + std::to_string(BF::get_Projectile_count()));
		}
		#endif // SHOW_FPS
		
		window.display();
	}

	running = false;
	physics_thread.join();
	BF::clear();
	return 0;
}