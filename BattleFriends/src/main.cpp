#include <pch.h>
#include <BF.h>


int main()
{
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "BattleFriends", sf::Style::Fullscreen);
	window.setFramerateLimit(170);

	BF::init(&window);
	BF::spawn_random_ent();

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