#pragma once

namespace BF
{
	//bool loadResource(int name, const char* type, bool (*SFML_load_from_memory)(const void*, size_t));

	template<typename SFML_type>
	bool loadResource(int name, const char* type, SFML_type obj);
}