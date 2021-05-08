#include <SFML/Graphics.hpp>
#include "Tank.h"


sf::Vector2i Tanks::GetPosInWorld(const Tank& tank) {
	sf::Vector2i vec((tank.pos.x - 8) / 8, (tank.pos.y - 8) / 8);
	vec.x += (tank.rotation == 3) && tank.pos.x % 8;
	vec.y += (tank.rotation == 0) && tank.pos.y % 8;
	return vec;
}

void Tanks::Rotate(Tank& tank, int new_rotate) {
	auto xy = GetPosInWorld(tank);
	tank.pos.x = xy.x * 8 + 8;
	tank.pos.y = xy.y * 8 + 8;
	tank.rotation = new_rotate % 4;
}

void Tanks::changeDirection(Tank& tank) {
	//TODO
	Rotate(tank, tank.rotation + rand() % 4);
}

void Tanks::changeDirection2(Tank tank) {
	if (rand() % 2 == 0)
		changeDirection(tank);
	else if (rand() % 2 == 0)
		Rotate(tank, tank.rotation + 1);
	else
		Rotate(tank, tank.rotation + 4 - 1);
}

void Tanks::Destroy(Tank& tank) {
	auto xy = GetPosInWorld(tank);
	int x = xy.x, y = xy.y;
	m_level->aplly_map_to_map(m_level->colision_map, m_level->empty_colisoin, x, y, 1);
	tank.active = 0;
	m_explosion->Create(tank.pos,Explosion::Big,(tank.type_ind>=4?( tank.type_ind-3 )*100:0));
}

void Tanks::init(Level* level, sf::Texture* texture, Bullets* bullets, Explosion* explosion, Controls* controls) {
	m_level = level;
	m_bullets = bullets;
	m_explosion = explosion;
	m_controls = controls;
	tanks.resize(6);
	tankType.init(texture);
	tanks[0]={{8 * 8 + 8,26 * 8 + 8},0,0,0,1,1,-1 };
}


//void AddTank(entt::registry& registry, sf::Vector2i pos, int number = 0, int type_ind = 0, int rotate = 0);
//void UpdateKeyState(entt::registry& registry);
//void UpdateAiRotation(entt::registry& registry);


//void AddTank(entt::registry& registry, sf::Vector2i pos, int number = 0, int type_ind = 0, int rotate = 0);
//void UpdateKeyState(entt::registry& registry);

void Tanks::UpdateAi()
{
	for (int i = 0; i < tanks.size(); ++i)
		if (tanks[i].active && tanks[i].ai)
		{
			auto& tank = tanks[i];
			int tmp = tankType.tanks_types[tank.type_ind].speed;
			if (!(tmp == 4 || ((tank.time_to_move + 1) % tmp)))
				continue;
			if (tank.pos.x % 8 == 0 && tank.pos.y % 8 == 0 && rand() % 16 == 0)
				changeDirection(tank);///
			else if (!tank.can_move && rand() % 4 == 0)
			{
				if (tank.pos.x % 8 != 0 || tank.pos.y % 8 != 0)
					Rotate(tank, tank.rotation + 2);
				else
					changeDirection2(tank);
			}
			if (rand()%32==0) {
				sf::Vector2i moveVec((tank.rotation == 1) - (tank.rotation == 3), (tank.rotation == 2) - (tank.rotation == 0));
				moveVec.x *= 6;
				moveVec.y *= 6;
				m_bullets->AddBullet(tank.pos + moveVec, tank.rotation, tankType.tanks_types[tank.type_ind].bullet_speed, tankType.tanks_types[tank.type_ind].bullet_power, i);
			}
		}
}

void Tanks::UpdateP()
{
	for (int i = 0; i < 2; ++i)
		if (tanks[i].active && !tanks[i].ai)
		{
			auto& tank = tanks[i];
			auto& control = m_controls->control[i];
			if (tank.is_move = (control.up || control.down || control.left || control.right))
				if (!((control.up && tank.rotation == 0) || (control.right && tank.rotation == 1) ||
					(control.down && tank.rotation == 2) || (control.left && tank.rotation == 3)))
					Rotate(tank,(control.up ? 0 : (control.right ? 1 :
						(control.down ? 2 : (control.left ? 3 : 0)))));
			if (control.shot) {
				control.shot = 0;
				control.shot_ = 1;
				sf::Vector2i moveVec((tank.rotation == 1) - (tank.rotation == 3), (tank.rotation == 2) - (tank.rotation == 0));
				moveVec.x *= 6;
				moveVec.y *= 6;
				m_bullets->AddBullet(tank.pos+ moveVec,tank.rotation,tankType.tanks_types[tank.type_ind].bullet_speed, tankType.tanks_types[tank.type_ind].bullet_power,i);
			}

		}
}

void Tanks::UpdatePos()
{
	for (int i = 0; i < tanks.size(); ++i)
		if (tanks[i].active && tanks[i].is_move)
		{
			auto& tank = tanks[i];
			sf::Vector2i moveVec((tank.rotation == 1) - (tank.rotation == 3), (tank.rotation == 2) - (tank.rotation == 0));
			tank.time_to_move = (tank.time_to_move + 1) % 4;
			int tmp = tankType.tanks_types[tank.type_ind].speed;
			if (tmp == 4 || ((tank.time_to_move+1) % tmp))
			{

				auto xy = GetPosInWorld(tank);
				int x = xy.x, y = xy.y;
				if (tank.rotation == 0 && m_level->is_air(x, y - 1, true) && m_level->is_air(x + 1, y - 1, true) ||
					tank.rotation == 1 && m_level->is_air(x + 2, y, true) && m_level->is_air(x + 2, y + 1, true) ||
					tank.rotation == 2 && m_level->is_air(x, y + 2, true) && m_level->is_air(x + 1, y + 2, true) ||
					tank.rotation == 3 && m_level->is_air(x - 1, y, true) && m_level->is_air(x - 1, y + 1, true))
				{
					tank.pos += moveVec;
					tank.can_move = 1;
					if (xy != GetPosInWorld(tank)) {
						m_level->aplly_map_to_map(m_level->colision_map, m_level->empty_colisoin, x, y, 1);
						xy = GetPosInWorld(tank);
						m_level->aplly_map_to_map(m_level->colision_map, m_level->tmp_map = m_level->get_tank_colision(i + 1), xy.x, xy.y, 1);
					}
				}
				else
					tank.can_move = 0;
				tank.anim_c = !tank.anim_c;
			}
		}
		else
			tanks[i].time_to_move = 0;
}

void Tanks::Draw(sf::RenderTarget* ren)
{
	sf::Sprite sprite;
	sprite.setOrigin(8, 8);
	sprite.setTexture(*tankType.texture);
	for (int i = 0; i < tanks.size(); ++i)
		if (tanks[i].active) {
			auto& tank = tanks[i];
			int tmp = tank.rotation % 4;
			sprite.setColor(sf::Color(250, 230, 158));
			sprite.setPosition(tank.pos.x, tank.pos.y);
			sprite.setTextureRect(sf::IntRect(tankType.tanks_types[tank.type_ind].cord_sprite.x + 16 * tank.anim_c + 32 * tmp, tankType.tanks_types[tank.type_ind].cord_sprite.y, 16, 16));
			ren->draw(sprite);
		}

}

void Tanks::DrawColosion(sf::RenderTarget* ren, sf::Vector2f pos)
{
	if (!draw_colision)
		return;
	sf::RectangleShape rc;
	rc.setFillColor(sf::Color(255, 0, 0, 100));
	rc.setSize(sf::Vector2f(16, 16));
	for (int i = 0; i < tanks.size(); ++i)
		if (tanks[i].active) {
			auto xy = GetPosInWorld(tanks[i]);
			rc.setPosition(xy.x * 8, xy.y * 8);
			rc.move(pos);
			ren->draw(rc);
		}
}

void Tanks::imguiDraw() {
	ImGui::Begin("Tanks");
	ImGui::Checkbox("Draw colision", &draw_colision);
	for (int i = 0; i < tanks.size(); ++i)
		if (ImGui::TreeNode(std::string("Tank " + std::to_string((int)i)).c_str()))
		{
			auto& tank = tanks[i];
			ImGui::Checkbox("AI", &tank.ai);
			ImGui::Checkbox("Active", &tank.active);
			ImGui::InputInt("Number", &tank.number);
			ImGui::SliderInt("Type", &tank.type_ind, 0, 7);
			ImGui::InputInt("Anim_c", &tank.anim_c);
			ImGui::InputInt2("Pos", &tank.pos.x);
			ImGui::InputInt("Time to move", &tank.time_to_move);
			ImGui::InputInt("Rotate", (int*)&tank.rotation);
			ImGui::InputInt("State", &tank.state);
			ImGui::Checkbox("Is move", &tank.is_move);
			ImGui::Checkbox("Can move", &tank.can_move);
			auto xy = GetPosInWorld(tank);
			int x = xy.x, y = xy.y;
			ImGui::Text("%d %d", x, y);
			ImGui::TreePop();
			ImGui::Separator();
		}

	ImGui::End();
}
