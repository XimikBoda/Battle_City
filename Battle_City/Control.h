#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <entt\entity\registry.hpp>
#include <SFML/System/Vector2.hpp>
namespace Control {
	namespace Element {
		/*struct Keyboard {
			sf::Keyboard::Key code;
			SERIALIZE(Keyboard, code)
		};*/
	}
	namespace Mark {
		struct ControlledExistence1 {};
		struct ControlledExistence2 {};
		struct ControlledExistence3 {};
		struct ControlledExistence4 {};

		struct SourceOfControl1 {};
		struct SourceOfControl2 {};
		struct SourceOfControl3 {};
		struct SourceOfControl4 {};
	}
	namespace Component {
		struct KeyState {
			float up, down, left, right, shot;
		};
		struct KeyboardMovement {
			sf::Keyboard::Key up = sf::Keyboard::W;
			sf::Keyboard::Key down = sf::Keyboard::S;
			sf::Keyboard::Key left = sf::Keyboard::A;
			sf::Keyboard::Key right = sf::Keyboard::D;
			sf::Keyboard::Key shot = sf::Keyboard::Space;
		};
	}
	namespace System {
		void Init(entt::registry& registry);
		void ResertStates(entt::registry& registry);
		void KeyboardMovement(entt::registry& registry);
		template<typename T1, typename T2>
		void ApplyMovementForCurrent(entt::registry& registry) {
			auto viewControl = registry.view<Component::KeyState, T1>();
			for (auto entityControl : viewControl) {
				auto& locKeyState = registry.get<Component::KeyState>(entityControl);
				auto viewExistence = registry.view<Component::KeyState, T2>();
				for (auto entityExistence : viewExistence) {
					auto& keyState = registry.get<Component::KeyState>(entityExistence);
					keyState= locKeyState;
				}
			}
		}
		void ApplyMovement(entt::registry& registry);
	}
}