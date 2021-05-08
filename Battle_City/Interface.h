#pragma once
#include "Level.h"
#include <SFML/Graphics/RectangleShape.hpp>
class Interface
{
	Level* level;
	sf::RectangleShape rs;
	sf::Texture* m_texture;
public:
	void init(Level* level, sf::Texture* texture);
	void Update(sf::RenderTarget* ren);
	void Draw(sf::RenderTarget* ren, int atank = 0, int payers=0, int p1 = 0, int p2 = 0, int level_ = 0);
};