#include "Score.h"
#include <SFML/Graphics/RenderTarget.hpp>

void Score::init(sf::Texture* texture, sf::RenderTarget* ren, long* count)
{
	this->texture = texture;
	this->ren = ren;
	this->m_count = count;
}

void Score::Create(sf::Vector2i pos, long time_show, int score)
{
	Create(sf::Vector2f(pos.x, pos.y), time_show, score);
}
void Score::Create(sf::Vector2f pos, long time_show, int score)
{
	sf::Sprite sprite;
	sprite.setOrigin(8, 8);
	sprite.setTexture(*texture);
	sprite.setPosition(pos);
	sprite.setTextureRect(sf::IntRect(160 + 16 * (score / 100 - 1), 80, 16, 16));
	scores.push_back(Score_el(*m_count, pos, time_show, score, sprite));
}

void Score::Update(long count)
{
	for (int i = 0; i < scores.size(); ++i)
	{
		if (scores[i].start_count+ scores[i].time_show<=*m_count)
		{
			std::swap(scores[i], scores[scores.size() - 1]);
			scores.resize(scores.size() - 1);
			--i;
		}
	}
}

void Score::Draw()
{
	for (int i = 0; i < scores.size(); ++i) {
		ren->draw(scores[i].sprite);
	}
}
