#include "Controls.h"

void Controls::Update() {
	for (int i = 0; i < 2; ++i) {
		control[i].up = sf::Keyboard::isKeyPressed(control[i].kup);
		control[i].down = sf::Keyboard::isKeyPressed(control[i].kdown);
		control[i].left = sf::Keyboard::isKeyPressed(control[i].kleft);
		control[i].right = sf::Keyboard::isKeyPressed(control[i].kright);
		if (control[i].shot_) {
			control[i].shot = 0;
			if (!sf::Keyboard::isKeyPressed(control[i].kshot)) {
				control[i].shot_ = 0;
			}
		}
		else
			control[i].shot = sf::Keyboard::isKeyPressed(control[i].kshot);
	}
}
