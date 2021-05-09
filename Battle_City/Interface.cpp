#include "Interface.h"
#include <SFML/Graphics/Sprite.hpp>

void Interface::init(Level* level, sf::Texture* texture) {
	this->level = level;
	m_texture = texture;
	rs.setFillColor(sf::Color(99, 99, 99));
	rs.setPosition(0, 0);
}

void Interface::Update(sf::RenderTarget* ren) {
	auto map_size = level->get_size_curent_map();
	sf::Vector2f size((map_size.x + 2 + 4) * 8, (map_size.y + 1 + 1) * 8), avr((size.x - 16) / 2, (size.y - 8) / 2);
	rs.setSize(size);
	rs.setPosition(-16, -8);

	//view=sf::View(avr/*,size*/);
	sf::View view(sf::FloatRect(0, 0, ren->getSize().x, ren->getSize().y));
	int n = std::min(ren->getSize().x / size.x, ren->getSize().y / size.y);
	view.setCenter(avr);
	view.zoom(1.0 / float(n));
	ren->setView(view);
}
sf::IntRect getNumber(int n) {
	return sf::IntRect(168 + (n % 5) * 8, 104 + (n / 5) * 8, 8, 8);
}
void Interface::Draw(sf::RenderTarget* ren, int atank, int payers, int p1, int p2, int level_) {
	ren->draw(rs);
	int l = (level->get_size_curent_map().x + 4) * 8 - 8 * 3;
	sf::Sprite sp(*m_texture);
	sp.setTextureRect(sf::IntRect(160, 112, 8, 8));
	for (int i = 0; i < 10; ++i)
		for (int j = 0; j < 2; ++j)
			if (i * 2 + j < atank) {
				sp.setPosition(l + j * 8, 3 * 2 + i * 8);
				ren->draw(sp);
			}
	for (int i = 0; i < payers; ++i) {
		sp.setTextureRect(sf::IntRect(264, 40 + 3 * i * 8, 16, 16));
		sp.setPosition(l, 136 + 8 * 3 * i);
		ren->draw(sp);
		sp.setTextureRect(getNumber(i ? p2 : p1));
		sp.setPosition(l + 8, 136 + 8 + 8 * 3 * i);
		ren->draw(sp);
	}
	sp.setTextureRect(sf::IntRect(264, 88, 16, 16));
	sp.setPosition(l, 184);
	ren->draw(sp);
	if (level_ >= 10) {
		sp.setTextureRect(getNumber(level_ / 10));
		sp.setPosition(l, 184 + 16);
		ren->draw(sp);
	}
	sp.setTextureRect(getNumber(level_ % 10));
	sp.setPosition(l + 8, 184 + 16);
	ren->draw(sp);
}

void Interface::ShowGameOver(sf::RenderTarget* ren)
{
	sf::Sprite sp(*m_texture);
	sp.setTextureRect(sf::IntRect(128, 104, 32, 16));
	sp.setOrigin(16,8);
	sp.setPosition((level->act_map.size().x + 4) * 8/2, (level->act_map.size().x + 2) * 8/2);
	ren->draw(sp);
}
