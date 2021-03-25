#pragma once
#include "Render.h"
#include <vector>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <entt\entity\registry.hpp>

namespace Type {
	class Explosion {};
};

class Explosion
{
public:
	enum Type
	{
		Small = 0,
		Big = 1
	};
private:
	struct Data
	{
		sf::Vector2f pos = { 0, 0 };
		Type type = Small;
		int score = 0;
		int animation_count = 0;
		int animation_state = 0;
	};
	std::vector<std::vector<int>> equence =
	{
		{3,0,1,2},
		{6,0,1,2,3,4,0}
	};
	sf::Texture* texture;
	sf::Vector2i cord_ex = { 16 * 2*4, 0 };
public:
	
	void init(sf::Texture* texture);
	void Create(entt::registry& registry, sf::Vector2f pos = { 0, 0 }, Type type = Small, int score = 0);
	void Update(entt::registry& registry);
};

