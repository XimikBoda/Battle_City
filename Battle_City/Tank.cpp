#include "Tank.h"
#include <entt/entity/handle.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

bool draw_colision = false;

sf::Vector2i Tank::Helper::GetPosInWorld(const Tank::Component::MoveData& moveData) {
	sf::Vector2i vec((moveData.pos.x - 8) / 8, (moveData.pos.y - 8) / 8);
	vec.x += (moveData.rotation == 3) && moveData.pos.x % 8;
	vec.y += (moveData.rotation == 0) && moveData.pos.y % 8;
	return vec;
}

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
		auto xy = Tank::Helper::GetPosInWorld(moveState);
		moveState.pos.x = xy.x * 8 + 8;
		moveState.pos.y = xy.y * 8 + 8;
		moveState.rotation = needRorare.new_rotate;
		registry.remove<Tank::Event::NeedRotate>(entity);
	}
}
void Tank::System::UpdateAiRotation(entt::registry& registry,int resp_time=190) {
	resp_time /= 8;
	auto view = registry.view<Tank::Component::Data, Tank::Component::MoveData, Tank::Event::NeedAiRotate>();
	for (auto [entity, data, moveState, needRorare] : view.each())
	{
		switch (data.live_time / resp_time)
		{
		case 0:

		}
		registry.remove<Tank::Event::NeedAiRotate>(entity);
	}
}

void Tank::System::UpdatePos(entt::registry& registry, TankType& tankType, Level& level)
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

				auto xy = Tank::Helper::GetPosInWorld(moveState);
				int x = xy.x, y = xy.y;
				if (moveState.rotation == 0 && level.is_air(level.get_block(x, y - 1)) && level.is_air(level.get_block(x + 1, y - 1)) ||
					moveState.rotation == 1 && level.is_air(level.get_block(x + 2, y)) && level.is_air(level.get_block(x + 2, y + 1)) ||
					moveState.rotation == 2 && level.is_air(level.get_block(x, y + 2)) && level.is_air(level.get_block(x + 1, y + 2)) ||
					moveState.rotation == 3 && level.is_air(level.get_block(x - 1, y)) && level.is_air(level.get_block(x - 1, y + 1))
					)
					moveState.pos += moveVec;
				data.anim_c = !data.anim_c;
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
		draw_element.sprite.setOrigin(8, 8);
		draw_element.sprite.setTexture(*tankType.texture);
		draw_element.sprite.setTextureRect(sf::IntRect(tankType.tanks_types[data.type_ind].cord_sprite.x + 16 * data.anim_c + 32 * tmp, tankType.tanks_types[data.type_ind].cord_sprite.y, 16, 16));
	}
}
void Tank::System::DrawColosion(sf::RenderTarget* ren, entt::registry& registry, sf::Vector2f pos)
{
	if (!draw_colision)
		return;
	sf::RectangleShape rc;
	rc.setFillColor(sf::Color(255, 0, 0, 100));
	rc.setSize(sf::Vector2f(16, 16));
	auto view = registry.view<Tank::Component::Data, Tank::Component::MoveData>();
	for (auto [entity, data, moveState] : view.each())
	{
		auto xy = Tank::Helper::GetPosInWorld(moveState);
		rc.setPosition(xy.x * 8, xy.y * 8);
		rc.move(pos);
		ren->draw(rc);
	}
}

void Tank::System::imguiDraw(entt::registry& registry, Level& level)
{
	ImGui::Begin("Tanks");
	ImGui::Checkbox("Draw colision", &draw_colision);
	auto view = registry.view<Type::Tank>();
	for (auto entity : view)
		if (ImGui::CollapsingHeader(std::string("Tank " + std::to_string((int)entity)).c_str()))
		{
			{
				bool ai = registry.any<Mark::AI>(entity);
				if (ImGui::Checkbox("AI", &ai))
					if (ai)
						registry.emplace<Mark::AI>(entity);
					else
						registry.remove<Mark::AI>(entity);
			}
			if (registry.has<Tank::Component::Data>(entity) && ImGui::TreeNode("Data")) {
				auto& data = registry.get<Component::Data>(entity);
				ImGui::InputInt("Number", &data.number);
				ImGui::SliderInt("Type", &data.type_ind, 0, 7);
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
				auto xy = Tank::Helper::GetPosInWorld(moveData);
				int x = xy.x, y = xy.y;
				ImGui::Text("%d %d", x, y);
				ImGui::Text("%d %d", level.is_air(level.get_block(x, y)), level.is_air(level.get_block(x + 1, y)));
				ImGui::Text("%d %d", level.is_air(level.get_block(x, y + 1)), level.is_air(level.get_block(x + 1, y + 1)));
			}
		}

	ImGui::End();
}
