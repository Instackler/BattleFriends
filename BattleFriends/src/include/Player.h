#pragma once
#include <Entity.h>

namespace BF 
{
	class Player : public Entity
	{
	public:
		//Player(const char* filename);
		Player(int textureID);
		Player() = delete;
		Player(Player&& other) noexcept;
		Player& operator= (const Player&) = default;
		~Player();

		void update() override;

	private:
		void checkInput();
		void shoot();

	private:
		unsigned int cooldown = 20;
		unsigned int counter = 20;
	};
}