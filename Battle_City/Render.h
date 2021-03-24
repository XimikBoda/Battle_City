#pragma once
#include <entt/entity/registry.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace Render {
	namespace Component {
		struct DrawElement {
			sf::Sprite sprite;
		};
	}
	namespace System {
		template <typename T>
		void Draw(entt::registry& registry, sf::RenderTarget* ren) {
			auto view = registry.view<Component::DrawElement,T>();
			for (auto entity : view) {
				auto drawElement = registry.get<Component::DrawElement>(entity);
				ren->draw(drawElement.sprite);
			}
		}
	}
};
