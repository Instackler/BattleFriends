#include <pch.h>
#include <loadTexture.h>

/*
bool BF::loadResource(int name, const char* type, bool (*SFML_load_from_memory)(const void*, size_t))
{
	HRSRC hResource = FindResourceA(NULL, MAKEINTRESOURCE(name), TEXT(type));
	if (hResource != NULL)
	{
		DWORD resourceSize = SizeofResource(NULL, hResource);
		HGLOBAL hResourceData = LoadResource(NULL, hResource);
		if (hResourceData != NULL)
		{
			LPVOID pData = LockResource(hResourceData);
			SFML_load_from_memory(pData, resourceSize);
			FreeResource(hResourceData);
			return true;
		}
		// TODO: add logging
		return false;
	}
	// TODO: add logging
	return false;
}
*/

template<typename SFML_type>
bool BF::loadResource(int name, const char* type, SFML_type obj)
{
	HRSRC hResource = FindResourceA(NULL, MAKEINTRESOURCE(name), TEXT(type));
	if (hResource != NULL)
	{
		DWORD resourceSize = SizeofResource(NULL, hResource);
		HGLOBAL hResourceData = LoadResource(NULL, hResource);
		if (hResourceData != NULL)
		{
			LPVOID pData = LockResource(hResourceData);
			obj.loadFromMemory(pData, resourceSize);
			FreeResource(hResourceData);
			return true;
		}
		// TODO: add logging
		return false;
	}
	// TODO: add logging
	return false;
}