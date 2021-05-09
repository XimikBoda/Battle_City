#include "SpawnFire.h"
#include <SFML/Graphics/RenderTarget.hpp>

void SpawnFire::init(sf::Texture* texture, sf::RenderTarget* ren, long* count)
{
	this->texture = texture;
	this->ren = ren;
	this->m_count = count;
}

void SpawnFire::Create(sf::Vector2f pos, Type type)
{
	sf::Sprite sprite;
	sprite.setOrigin(8, 8);
	sprite.setTexture(*texture);
	sprite.setPosition(pos);
	//sprite.setScale(2,2);
	exps.push_back(Exp(*m_count, pos, type, sprite));
}
void SpawnFire::Create(sf::Vector2i pos, Type type)
{
	Create(sf::Vector2f(pos.x, pos.y), type);
}

void SpawnFire::Update(long count) {
	for (int i = 0; i < exps.size(); ++i)
	{	
		bool t = equence[exps[i].type].size() == exps[i].animation_i;
		if (!t&&exps[i].animation_time++ >= equence[exps[i].type][exps[i].animation_i]) {
				exps[i].animation_time = 0;
				++exps[i].animation_i;
			}
		if (t)
		{
			std::swap(exps[i], exps[exps.size() - 1]);
			exps.resize(exps.size() - 1);
			--i;
		}
		else
		{
			
			int c = exps[i].animation_i%4;
			if ((exps[i].animation_i / 4) % 2 == 1)
				c = 3 - c;

			exps[i].sprite.setTextureRect(sf::IntRect(cord_ex.x + 16 * c, cord_ex.y, 16, 16));
		}
	}
}

void SpawnFire::Draw()
{
	for (int i = 0; i < exps.size(); ++i) {
		ren->draw(exps[i].sprite);
	}
}
