#include <pch.h>
#include <BF.h>
#include <vector>


int main(int argc, char** argv)
{
	//sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "BattleFriends", sf::Style::Fullscreen);
	sf::RenderWindow window(sf::VideoMode(800, 450), "BattleFriends", sf::Style::Close);
	SetForegroundWindow(window.getSystemHandle());
	//window.setFramerateLimit(5);
	window.setVerticalSyncEnabled(true);

	BF::init(&window);

	sf::Event event;
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			BF::process_event(event, window);
		}

		BF::physics_loop();

		window.clear(sf::Color(20, 21, 26, 100));
		BF::draw();
		window.display();
	}

	BF::clear();
	return 0;
}