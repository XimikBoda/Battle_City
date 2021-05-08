#include "Explosion.h"
#include <SFML/Graphics/RenderTarget.hpp>

void Explosion::init(sf::Texture* texture, sf::RenderTarget* ren, long* count, Score* score)
{
	this->texture = texture;
	this->ren = ren;
	this->m_count = count;
	this->m_score = score;
}

void Explosion::Create(sf::Vector2f pos, Type type, int score) 
{
	sf::Sprite sprite;
	sprite.setOrigin(16, 16);
	sprite.setTexture(*texture);
	sprite.setPosition(pos);
	//sprite.setScale(2,2);
	exps.push_back(Exp(*m_count, pos, type, score, sprite));
}
void Explosion::Create( sf::Vector2i pos, Type type, int score)
{
	sf::Sprite sprite;
	sprite.setOrigin(16, 16);
	sprite.setTexture(*texture);
	sprite.setPosition(sf::Vector2f(pos.x, pos.y));
	//sprite.setScale(2,2);
	exps.push_back(Exp(*m_count, sf::Vector2f(pos.x,pos.y), type, score, sprite));
}

void Explosion::Update(long count) {
	for (int i = 0; i < exps.size(); ++i) 
	{
		int n = (count - exps[i].start_count) / equence[exps[i].type][0] + 1;
		if (n >= equence[exps[i].type].size()) 
		{
			if (exps[i].score != 0)
				m_score->Create(exps[i].pos,12,exps[i].score);
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
		ren->draw(exps[i].sprite);
	}
}
