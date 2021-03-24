#include "Explosion.h"

void Explosion::init(sf::Texture* texture)
{
	this->texture = texture;
}

void Explosion::Create(entt::registry& registry, sf::Vector2f pos, Type type, int score)
{
	auto entity = registry.create();
	sf::Sprite sprite;
	sprite.setOrigin(16, 16);
	sprite.setTexture(*texture);
	sprite.setPosition(pos);
	sprite.setTextureRect(sf::IntRect(cord_ex.x + 32 * 1, cord_ex.y, 32, 32));
	registry.emplace<Render::Component::DrawElement>(entity, sprite);
	registry.emplace<Explosion::Data>(entity, pos, type);
	registry.emplace<::Type::Explosion>(entity);
}

void Explosion::Update(entt::registry& registry)
{
	auto view = registry.view<Explosion::Data, Render::Component::DrawElement>();
	for (auto [entity, data, draw_element] : view.each())
		if (++data.animation_count >= equence[data.type][0])
			if (++data.animation_state >= equence[data.type].size())
				registry.destroy(entity);
			else
			{
				draw_element.sprite.setTextureRect(sf::IntRect(cord_ex.x + 32 * equence[data.type][data.animation_state],
					cord_ex.y, 32, 32));
				data.animation_count = 0;
			}
}
