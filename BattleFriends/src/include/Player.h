#pragma once
#include <Entity.h>


namespace BF 
{
	class Player : public Entity
	{
	public:
		Player(const char* filename);
		Player() = delete;
		Player(Player&& other) noexcept;
		Player& operator= (const Player&) = default;
		~Player();

		void update() override;

	private:
		void checkInput();
		void shoot();

	private:
		unsigned int cooldown = 10;
		unsigned int counter = 10;
	};
}