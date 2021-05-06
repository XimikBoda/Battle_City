#include "Control.h"
void Control::System::Init(entt::registry& registry) {
	auto entity = registry.create();
	registry.emplace<Component::KeyState>(entity);
	registry.emplace<Component::KeyboardMovement>(entity);
	registry.emplace<Mark::SourceOfControl1>(entity);
}
void Control::System::ResertStates(entt::registry& registry) {
	auto view = registry.view<Component::KeyState>();
	for (auto entity : view) {
		auto& state = registry.get<Component::KeyState>(entity);
		state.up = state.down = state.left = state.right = 0;
	}
}

void Control::System::KeyboardMovement(entt::registry& registry) {
	auto view = registry.view<Component::KeyState, Component::KeyboardMovement>();
	for (auto entity : view) {
		auto& movementState = registry.get<Component::KeyState>(entity);
		auto& keyboardMovement = registry.get<Component::KeyboardMovement>(entity);
		movementState.up = std::max<float>(movementState.up, (float)sf::Keyboard::isKeyPressed(keyboardMovement.up));
		movementState.down = std::max<float>(movementState.down, (float)sf::Keyboard::isKeyPressed(keyboardMovement.down));
		movementState.left = std::max<float>(movementState.left, (float)sf::Keyboard::isKeyPressed(keyboardMovement.left));
		movementState.right = std::max<float>(movementState.right, (float)sf::Keyboard::isKeyPressed(keyboardMovement.right));
		movementState.shot = std::max<float>(movementState.right, (float)sf::Keyboard::isKeyPressed(keyboardMovement.shot));
	}
}

void Control::System::ApplyMovement(entt::registry& registry) {
	ApplyMovementForCurrent<Mark::SourceOfControl1, Mark::ControlledExistence1>(registry);
	ApplyMovementForCurrent<Mark::SourceOfControl2, Mark::ControlledExistence2>(registry);
	ApplyMovementForCurrent<Mark::SourceOfControl3, Mark::ControlledExistence3>(registry);
	ApplyMovementForCurrent<Mark::SourceOfControl4, Mark::ControlledExistence4>(registry);
}