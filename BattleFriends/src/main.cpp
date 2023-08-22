#include <pch.h>
#include <BF.h>



int main()
{
	//sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "BattleFriends", sf::Style::Fullscreen);
	sf::RenderWindow window(sf::VideoMode(800, 450), "BattleFriends", sf::Style::Close);
	SetForegroundWindow(window.getSystemHandle());
	window.setVerticalSyncEnabled(true);
	BF::init(&window);

	
	tinytmx::Map map;
	BF::loadMap("map", "tmx", map);
	struct texparams
	{
		std::string name = "default";
		unsigned int width, height = 0;
	};
	std::map<int, texparams> texparams_by_gid;
	for (auto tileset : map.GetTilesets())
	{
		int first_gid = tileset->GetFirstGid();
		for (auto tile : tileset->GetTiles())
		{
			std::string file_name = tile->GetImage()->GetSource();
			texparams texpar{ file_name.substr(0, file_name.find(".")), // Discard file extension
							tile->GetImage()->GetWidth(),
							tile->GetImage()->GetHeight() };
			texparams_by_gid.emplace(first_gid + tile->GetId(), texpar);
		}
	}

	for (auto obj_group : map.GetObjectGroups())
	{
		for (auto object : obj_group->GetObjects())
		{
			texparams texpar = texparams_by_gid[object->GetTile()->gid];
			BF::Entity entity{ texpar.name };
			entity.setPosition(object->GetX(), object->GetY());
			entity.setScale(object->GetWidth() / texpar.width, object->GetHeight() / texpar.height);
			BF::entities.push_back(entity);
		}
	}

	//BF::Entity entity{"logo3"};
	//entity.setPosition(MAP_WIDTH / 2.f, MAP_HEIGHT / 2.f);
	//entity.setScale();


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