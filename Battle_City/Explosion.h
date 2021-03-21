#pragma once
#include <vector>
#include <SFML/Graphics/Sprite.hpp>
class Explosion {
public:
	enum Type {
		Small = 0,
		Big = 1
	};
private:
	struct Exp {
		long start_count = 0;
		sf::Sprite sprite;
		sf::RenderTarget* ren = 0;
		sf::Vector2f pos = { 0, 0 };
		Type type = Small;
		int score = 0;
		Exp() = default;
		Exp(long start_count, sf::RenderTarget* ren, sf::Vector2f pos, Type type, int score) {
			this->start_count = start_count;
			this->ren = ren;
			this->pos = pos;
			this->type = type;
			this->score = score;
		}
	};
	std::vector<std::vector<int>> equence =
	{
		{3,0,1,2},
		{6,0,1,2,3,4,0}
	};
	sf::Texture* texture;
	sf::Vector2i cord_ex = { 16*2, 0 };
	std::vector<Exp> exps;
public:
	void init(sf::Texture* texture);
	void Create(long start_count = 0, sf::RenderTarget* ren = 0, sf::Vector2f pos = { 0, 0 }, Type type = Small, int score = 0);
	void Update(long count);
	void Draw();
};

