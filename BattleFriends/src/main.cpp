#include <pch.h>
#include <BF.h>

bool loadResourceByName(const char* name, const char* type, sf::Texture &obj)
{
	HRSRC hResource = FindResourceA(NULL, name, type);
	if (hResource != NULL)
	{
		DWORD resourceSize = SizeofResource(NULL, hResource);
		HGLOBAL hResourceData = LoadResource(NULL, hResource);
		if (hResourceData != NULL)
		{
			LPVOID pData = LockResource(hResourceData);
			obj.loadFromMemory(pData, resourceSize);
			FreeResource(hResourceData);
			// TODO: add logging
			return true;
		}
		// TODO: add logging
		return false;
	}
	// TODO: add logging
	return false;
}

int main()
{
	//sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "BattleFriends", sf::Style::Fullscreen);
	sf::RenderWindow window(sf::VideoMode(800, 450), "BattleFriends", sf::Style::Close);
	SetForegroundWindow(window.getSystemHandle());
	window.setVerticalSyncEnabled(true);

	tinytmx::Map* map = new tinytmx::Map();
	std::string fileName = "resources/map.tmx";
	map->ParseFile(fileName);
	delete map;

	BF::init(&window);

	sf::Event event;
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			BF::process_event(event, window);
		}

		window.clear(sf::Color(20, 21, 26, 100));
		BF::draw();
		window.display();
	}

	BF::clear();
	return 0;
}