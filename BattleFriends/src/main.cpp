#include <pch.h>

#include <BF.h>

#define ENTITY_NUM 9



int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "BattleFriends", sf::Style::Close);
	window.setFramerateLimit(120);


	BF::init();
	srand(time(NULL));

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
	BF::entities[5].stationary = true;
	

	sf::Font font;
	font.loadFromFile("resources/fonts/raleway/Raleway-SemiBold.ttf");
	sf::Text fpsCounter;
	fpsCounter.setFont(font);
	fpsCounter.setString("Hello world");

	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		
		//update entities
		BF::updateEntities();

		//check collisions
		BF::checkCollisions();

		//draw entities
		BF::drawEntities(window);

		window.draw(fpsCounter);

		window.display();

		fpsCounter.setString(std::to_string(1.f / clock.restart().asSeconds()));
	}

	BF::entities.clear();

	return 0;
}