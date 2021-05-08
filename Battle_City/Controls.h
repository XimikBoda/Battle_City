#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
class Control
{
public:
	sf::Keyboard::Key kup = sf::Keyboard::W;
	sf::Keyboard::Key kdown = sf::Keyboard::S;
	sf::Keyboard::Key kleft = sf::Keyboard::A;
	sf::Keyboard::Key kright = sf::Keyboard::D;
	sf::Keyboard::Key kshot = sf::Keyboard::Space;
	bool up=0, down=0, left=0, right=0, shot=0;
};
class Controls
{
public:
	Control control[2];
	void Update() {
		for (int i = 0; i < 2; ++i) {
			control[i].up = sf::Keyboard::isKeyPressed(control[i].kup);
			control[i].down = sf::Keyboard::isKeyPressed(control[i].kdown);
			control[i].left = sf::Keyboard::isKeyPressed(control[i].kleft);
			control[i].right = sf::Keyboard::isKeyPressed(control[i].kright);
			control[i].shot = sf::Keyboard::isKeyPressed(control[i].kshot);
		}
	}
};

