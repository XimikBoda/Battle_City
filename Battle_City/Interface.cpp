#include "Interface.h"

void Interface::init(Level* level) {
	this->level = level;
	rs.setFillColor(sf::Color(99, 99, 99));
	rs.setPosition(0, 0);
}

void Interface::Update(sf::RenderTarget* ren) {
	auto map_size = level->get_size_curent_map();
	sf::Vector2f size((map_size.x + 2 + 4) * 8, (map_size.y + 1 + 1) * 8), avr((size.x-16) / 2, (size.y-8) / 2);
	rs.setSize(size);

	//view=sf::View(avr/*,size*/);
	sf::View view(sf::FloatRect(0, 0, ren->getSize().x, ren->getSize().y));
	int n = std::min(ren->getSize().x / size.x, ren->getSize().y / size.y);
	view.setCenter(avr);
	view.zoom(1.0 / float(n));
	ren->setView(view);
}

void Interface::Draw(sf::RenderTarget* ren) {
	ren->draw(rs);
}
