#pragma once
#include <vector>
#include <SFML/Graphics/Sprite.hpp>
class Score
{
private:
	struct Score_el
	{
		long start_count = 0;
		sf::Sprite sprite;
		sf::Vector2f pos = { 0, 0 };
		long time_show = 0;
		int score = 0;
		Score_el() = default;
		Score_el(long start_count, sf::Vector2f pos, long time_show, int score, const sf::Sprite& sprite)
		{
			this->start_count = start_count;
			this->pos = pos;
			this->time_show = time_show;
			this->score = score;
			this->sprite = sprite;
		}
	};
	sf::RenderTarget* ren = 0;
	sf::Texture* texture;
	long* m_count;
	sf::Vector2i cord_ex = { 16 * 2 * 4, 0 };
	std::vector<Score_el> scores;
public:
	void init(sf::Texture* texture, sf::RenderTarget* ren, long* count);
	void Create(sf::Vector2i pos = { 0, 0 }, long time_show = 0, int score = 0);
	void Create(sf::Vector2f pos = { 0, 0 }, long time_show = 0, int score = 0);
	void Update(long count);
	void Draw();
};

