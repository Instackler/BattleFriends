#pragma once

namespace BF
{
	template<typename SFML_type>
	bool loadResource(const std::string& name, const char* type, SFML_type &obj)
	{
		HRSRC hResource = FindResourceA(NULL, (LPCSTR)name.c_str(), type);
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
}
