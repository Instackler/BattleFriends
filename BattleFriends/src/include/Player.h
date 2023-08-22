#pragma once
#include <Entity.h>

namespace BF 
{
	struct player_inputs
	{
		bool up, left, down, right, shoot;
		sf::Vector2i mouse_pos;
	};

	class Player : public Entity
	{
	public:
		Player(const std::string& textureID);
		Player(const Player&);
		Player() = delete;
		Player(Player&& other) noexcept;
		Player& operator= (const Player&) = default;

		~Player();

		void update(const player_inputs& inputs);

	private:
		void applyInputs(const player_inputs& inputs);
		void shoot(const sf::Vector2i mouse_pos);

	private:
		unsigned int cooldown = 20;
		unsigned int counter = 20;
	};
}