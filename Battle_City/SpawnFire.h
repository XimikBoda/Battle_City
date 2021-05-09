#pragma once
#include <vector>
#include <SFML/Graphics/Sprite.hpp>
class SpawnFire
{
public:
	enum Type
	{
		Player = 0,
		Enemy = 1
	};
private:
	struct Exp
	{
		long start_count = 0;
		sf::Sprite sprite;
		sf::Vector2f pos = { 0, 0 };
		Type type = Player;
		int animation_i = 0;
		int animation_time = 0;
		Exp() = default;
		Exp(long start_count, sf::Vector2f pos, Type type, const sf::Sprite& sprite)
		{
			this->start_count = start_count;
			this->pos = pos;
			this->type = type;
			this->sprite = sprite;
		}
	};
	sf::RenderTarget* ren = 0;
	std::vector<std::vector<int>> equence =
	{
		{2,3,3,4,2,3,4,3,2,4,3,3,1},
		{4,3,3,6,4,4,6,4,4,6,4,4,2}
	};
	sf::Texture* texture;
	long* m_count;
	sf::Vector2i cord_ex = { 128, 48 };
	std::vector<Exp> exps;
public:
	void init(sf::Texture* texture, sf::RenderTarget* ren, long* count);
	void Create(sf::Vector2f pos = { 0, 0 }, Type type = Player);
	void Create(sf::Vector2i pos = { 0, 0 }, Type type = Player);
	void Update(long count);
	void Draw();
};