#include <pch.h>
#include <loadTexture.h>

sf::Texture loadTexture(int name)
{
	HRSRC hResource = FindResourceA(NULL, MAKEINTRESOURCE(name), TEXT("PNG"));
	if (hResource != NULL) {
		DWORD resourceSize = SizeofResource(NULL, hResource);
		HGLOBAL hResourceData = LoadResource(NULL, hResource);
		if (hResourceData != NULL) {

			sf::Texture texture;
			LPVOID pData = LockResource(hResourceData);

			sf::Image image;
			image.loadFromMemory(pData, resourceSize);

			texture.create(image.getSize().x, image.getSize().y);
			texture.update(image);

			FreeResource(hResourceData);

			return texture;
		}
		// return errTexture;
	}
	// return noTexture;
}