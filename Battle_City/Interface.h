#pragma once
#include "Level.h"
#include <SFML/Graphics/RectangleShape.hpp>
class Interface
{
	Level* level;
	sf::RectangleShape rs;
public:
	void init(Level* level);
	void Update(sf::RenderTarget* ren);
	void Draw(sf::RenderTarget* ren);
};