#include "Explosion.h"
#include <SFML/Graphics/RenderTarget.hpp>

void Explosion::init(sf::Texture* texture) {
	this->texture = texture;
}

void Explosion::Create(long start_count, sf::RenderTarget* ren, sf::Vector2f pos, Type type, int score) {
	exps.push_back(Exp(start_count, ren, pos, type, score));
	exps[exps.size() - 1].sprite.setOrigin(16, 16);
	exps[exps.size() - 1].sprite.setTexture(*texture);
	exps[exps.size() - 1].sprite.setPosition(pos);
	exps[exps.size() - 1].sprite.setScale(2,2);
}

void Explosion::Update(long count) {
	for (int i = 0; i < exps.size(); ++i) {
		int n = (count - exps[i].start_count) / equence[exps[i].type][0] + 1;
		if (n >= equence[exps[i].type].size()) {
			std::swap(exps[i], exps[exps.size() - 1]);
			exps.resize(exps.size() - 1);
			--i;
		}
		else {
			int c = equence[exps[i].type][n];
			exps[i].sprite.setTextureRect(sf::IntRect(cord_ex.x + 32 * c, cord_ex.y, 32, 32));
		}
	}
}

void Explosion::Draw() {
	for (int i = 0; i < exps.size(); ++i) {
		exps[i].ren->draw(exps[i].sprite);
	}
}
