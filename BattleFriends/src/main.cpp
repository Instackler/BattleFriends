#include <pch.h>
#include <BF.h>



int main()
{
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "BattleFriends", sf::Style::Fullscreen);
	window.setFramerateLimit(120);

	BF::init();
	BF::spawn_random_ent();

	#ifdef SHOW_FPS
	sf::Font font;
	font.loadFromFile("resources/fonts/raleway/Raleway-SemiBold.ttf");
	sf::Text fpsCounter;
	fpsCounter.setFont(font);
	fpsCounter.setString("Hello world");
	sf::Clock clock;
	int frames = 0;
	sf::Text Entities_count;
	Entities_count.setFont(font);
	Entities_count.move(0.f, 100.f);
	sf::Text Projectiles_count;
	Projectiles_count.setFont(font);
	Projectiles_count.move(0.f, 200.f);	
	#endif // SHOW_FPS


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				window.close();
		}

		window.clear(sf::Color(20, 21, 26, 100));
		
		BF::update();
		BF::draw(window);

		#ifdef SHOW_FPS
		window.draw(fpsCounter);
		window.draw(Entities_count);
		window.draw(Projectiles_count);
		frames++;
		if (frames == 100)
		{
			fpsCounter.setString(std::to_string(100.f / clock.restart().asSeconds()));
			frames = 0;
			Entities_count.setString("Entities: " + std::to_string(BF::get_Entity_count()));
			Projectiles_count.setString("Projectiles: " + std::to_string(BF::get_Projectile_count()));
		}
		#endif // SHOW_FPS
		
		window.display();
	}

	BF::clear();
	return 0;
}