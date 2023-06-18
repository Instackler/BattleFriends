#include <BF.h>


BF::Entity::Entity(std::string& filename)
{
	m_texture.loadFromFile(filename);
}

