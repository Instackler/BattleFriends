#include <pch.h>
#include <Map.h>
#include <BF.h>

void BF::parseMap(const std::string& name, const char* type, tinytmx::Map& map)
{
	HRSRC hResource = FindResourceA(NULL, (LPCSTR)name.c_str(), type);
	if (hResource != NULL)
	{
		DWORD resourceSize = SizeofResource(NULL, hResource);
		HGLOBAL hResourceData = LoadResource(NULL, hResource);
		if (hResourceData != NULL)
		{
			LPVOID pData = LockResource(hResourceData);
			map.ParseText((char*)pData);
			FreeResource(hResourceData);
		}
	}
}

void BF::loadMap()
{
	tinytmx::Map map;
	BF::parseMap("map", "tmx", map);
	struct texparams
	{
		std::string name = "default";
		unsigned int width = 0, height = 0;
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
			entity.setOrigin(texpar.width / 2.f, texpar.height / 2.f);
			entity.setScale(object->GetWidth() / texpar.width, object->GetHeight() / texpar.height);
			entity.setPosition(object->GetX() + object->GetWidth() * 0.5f, object->GetY() - object->GetHeight() * 0.5f);
			BF::map_objects.push_back(entity);
		}
	}
}
