#include <pch.h>
#include <BF.h>

#define SHOW_FPS


int main()
{
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "BattleFriends", sf::Style::Fullscreen);
	window.setFramerateLimit(120);

	BF::init();
	srand(time(NULL));

	BF::players.emplace_back("resources/logo.png");
	for (int i = 0; i < ENTITY_NUM; i++)
	{
		BF::entities.emplace_back("resources/logo.png");
		if (i < 4)
		{
			BF::entities[i].move((i + 2) * 220.f, 100.f);
			BF::entities[i].setSpeed(((rand() % 11) - 5) / 20.0f, (rand() % 11) / 10.0f);
		}
		else
		{
			BF::entities[i].move((i - 3) * 210.f, 600.f);
			BF::entities[i].setSpeed(((rand() % 11) - 5) / 20.0f, ((rand() % 11) - 10) / 10.0f);
		}
	}
	BF::entities[1].stationary = true;
	BF::entities[4].stationary = true;


	#ifdef SHOW_FPS
	sf::Font font;
	font.loadFromFile("resources/fonts/raleway/Raleway-SemiBold.ttf");
	sf::Text fpsCounter;
	fpsCounter.setFont(font);
	fpsCounter.setString("Hello world");
	sf::Clock clock;
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
		
		BF::update();  //update entities
		BF::checkCollisions();

		BF::draw_world(window);
		BF::draw_minimap(window);

		#ifdef SHOW_FPS
		window.draw(fpsCounter);
		fpsCounter.setString(std::to_string(1.f / clock.restart().asSeconds()));
		#endif // SHOW_FPS
		
		window.display();
	}


	BF::clear();
	return 0;
}