#include "Tank.h"
#include <entt/entity/handle.hpp>

void Tank::System::AddTank(entt::registry& registry, sf::Vector2i pos, int number, int type_ind, int rotate)
{
	auto entity = registry.create();
	sf::Sprite sprite;
	registry.emplace<Render::Component::DrawElement>(entity, sprite);
	registry.emplace<Tank::Component::Data>(entity, number, type_ind);
	registry.emplace<Tank::Component::MoveData>(entity, pos, rotate);
	registry.emplace<Type::Tank>(entity);
	registry.emplace<Control::Mark::ControlledExistence1>(entity);
	registry.emplace<Control::Component::KeyState>(entity);
	registry.emplace<Tank::Component::KeyState>(entity);
}

void Tank::System::UpdateKeyState(entt::registry& registry)
{
	auto view = registry.view<Tank::Component::KeyState, Control::Component::KeyState, Tank::Component::MoveData>();
	for (auto [entity, locKeyState, keyState, moveState] : view.each())
	{
		locKeyState.up = keyState.up > 0.5;
		locKeyState.down = keyState.down > 0.5;
		locKeyState.right = keyState.right > 0.5;
		locKeyState.left = keyState.left > 0.5;
		locKeyState.shot = keyState.shot > 0.5;
		if (moveState.is_move = (locKeyState.up || locKeyState.down || locKeyState.left || locKeyState.right))
			if (!((locKeyState.up && moveState.rotation == 0) || (locKeyState.right && moveState.rotation == 1) ||
				(locKeyState.down && moveState.rotation == 2) || (locKeyState.left && moveState.rotation == 3)))
				registry.emplace<Tank::Event::NeedRotate>(entity, (locKeyState.up ? 0 : (locKeyState.right ? 1 :
					(locKeyState.down ? 2 : (locKeyState.left ? 3 : 0)))));
	}
}

void Tank::System::UpdateRotation(entt::registry& registry) {
	auto view = registry.view<Tank::Component::MoveData, Tank::Event::NeedRotate>();
	for (auto [entity, moveState, needRorare] : view.each())
	{
		//todo
		moveState.rotation = needRorare.new_rotate;
		registry.remove<Tank::Event::NeedRotate>(entity);
	}
}

void Tank::System::UpdatePos(entt::registry& registry, TankType& tankType)
{
	auto view = registry.view<Tank::Component::Data, Tank::Component::MoveData>();
	for (auto [entity, data, moveState] : view.each())
	{
		if (moveState.is_move)
		{
			sf::Vector2i moveVec((moveState.rotation == 1) - (moveState.rotation == 3), (moveState.rotation == 2) - (moveState.rotation == 0));
			moveState.time_to_move = (moveState.time_to_move + 1) % 4;
			int tmp = tankType.tanks_types[data.type_ind].speed;
			if (tmp == 4 || !(moveState.time_to_move % (4 - tmp)))
			{
				moveState.pos += moveVec;
				data.anim_c != data.anim_c;
			}
		}
		else
			moveState.time_to_move = 0;
	}
}

void Tank::System::UpdateSprites(entt::registry& registry, TankType& tankType)
{
	auto view = registry.view<Tank::Component::Data, Render::Component::DrawElement, Tank::Component::MoveData>();
	for (auto [entity, data, draw_element, moveState] : view.each())
	{
		int tmp = moveState.rotation % 4;
		draw_element.sprite.setColor(sf::Color(250, 230, 158));
		draw_element.sprite.setPosition(moveState.pos.x, moveState.pos.y);
		draw_element.sprite.setOrigin(8,8);
		draw_element.sprite.setTexture(*tankType.texture);
		draw_element.sprite.setTextureRect(sf::IntRect(tankType.tanks_types[data.type_ind].cord_sprite.x+16 * data.anim_c+32*tmp, tankType.tanks_types[data.type_ind].cord_sprite.y , 16, 16));
	}
}

void Tank::System::imguiDraw(entt::registry& registry)
{
	ImGui::Begin("Tanks");
	auto view = registry.view<Type::Tank>();
	for (auto entity : view)
		if (ImGui::CollapsingHeader(std::string("Tank " + std::to_string((int)entity)).c_str()))
		{
			if (registry.has<Tank::Component::Data>(entity) && ImGui::TreeNode("Data")) {
				auto &data = registry.get<Component::Data>(entity);
				ImGui::InputInt("Number", &data.number);
				ImGui::SliderInt("Type", &data.type_ind,0,8);
				ImGui::InputInt("Anim_c", &data.anim_c);
				ImGui::TreePop();
				ImGui::Separator();
			}
			if (registry.has<Tank::Component::MoveData>(entity) && ImGui::TreeNode("MoveData")) {
				auto& moveData = registry.get<Component::MoveData>(entity);
				ImGui::InputInt2("Pos", &moveData.pos.x);
				ImGui::InputInt("Time to move", &moveData.time_to_move);
				ImGui::InputInt("Rotate", (int*)&moveData.rotation);
				ImGui::Checkbox("Is move", &moveData.is_move);
				ImGui::TreePop();
				ImGui::Separator();
			}
		}

	ImGui::End();
}
