#include "Explosion.h"
#include <SFML/Graphics/RenderTarget.hpp>

void Explosion::init(sf::Texture* texture) 
{
	this->texture = texture;
}

void Explosion::Create(long start_count, sf::RenderTarget* ren, sf::Vector2f pos, Type type, int score) 
{
	sf::Sprite sprite;
	sprite.setOrigin(16, 16);
	sprite.setTexture(*texture);
	sprite.setPosition(pos);
	//sprite.setScale(2,2);
	exps.push_back(Exp(start_count, ren, pos, type, score, sprite));
}

void Explosion::Update(long count) {
	for (int i = 0; i < exps.size(); ++i) 
	{
		int n = (count - exps[i].start_count) / equence[exps[i].type][0] + 1;
		if (n >= equence[exps[i].type].size()) 
		{
			std::swap(exps[i], exps[exps.size() - 1]);
			exps.resize(exps.size() - 1);
			--i;
		}
		else 
		{
			int c = equence[exps[i].type][n];
			exps[i].sprite.setTextureRect(sf::IntRect(cord_ex.x + 32 * c, cord_ex.y, 32, 32));
		}
	}
}

void Explosion::Draw() 
{
	for (int i = 0; i < exps.size(); ++i) {
		exps[i].ren->draw(exps[i].sprite);
	}
}
