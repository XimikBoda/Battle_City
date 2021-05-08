#pragma once
#include <vector>
#include <SFML/Graphics/Sprite.hpp>
#include "Score.h"
class Explosion 
{
public:
	enum Type 
	{
		Small = 0,
		Big = 1
	};
private:
	struct Exp 
	{
		long start_count = 0;
		sf::Sprite sprite;
		sf::Vector2f pos = { 0, 0 };
		Type type = Small;
		int score = 0;
		Exp() = default;
		Exp(long start_count, sf::Vector2f pos, Type type, int score,const sf::Sprite& sprite) 
		{
			this->start_count = start_count;
			this->pos = pos;
			this->type = type;
			this->score = score;
			this->sprite = sprite;
		}
	};
	sf::RenderTarget* ren = 0;
	std::vector<std::vector<int>> equence =
	{
		{3,0,1,2},
		{6,0,1,2,3,4,0}
	};
	sf::Texture* texture;
	long* m_count;
	Score* m_score;
	sf::Vector2i cord_ex = { 16 * 2 * 4, 0 };
	std::vector<Exp> exps;
public:
	void init(sf::Texture* texture, sf::RenderTarget* ren, long* count, Score* score);
	void Create(sf::Vector2f pos = { 0, 0 }, Type type = Small, int score = 0);
	void Create(sf::Vector2i pos = { 0, 0 }, Type type = Small, int score = 0);
	void Update(long count);
	void Draw();
};

