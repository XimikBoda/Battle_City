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

void Tanks::init(Level* level, sf::Texture* texture, Bullets* bullets, Explosion* explosion, Controls* controls, SpawnFire* spawnFire) {
	m_level = level;
	m_bullets = bullets;
	m_explosion = explosion;
	m_controls = controls;
	m_spawnFire = spawnFire;
	tanks.resize(6);
	tankType.init(texture);
	//tanks[0]={{8 * 8 + 8,26 * 8 + 8},0,0,0,1,1,-1 };
}


//void AddTank(entt::registry& registry, sf::Vector2i pos, int number = 0, int type_ind = 0, int rotate = 0);
//void UpdateKeyState(entt::registry& registry);
//void UpdateAiRotation(entt::registry& registry);


//void AddTank(entt::registry& registry, sf::Vector2i pos, int number = 0, int type_ind = 0, int rotate = 0);
//void UpdateKeyState(entt::registry& registry);


//void AddTank(entt::registry& registry, sf::Vector2i pos, int number = 0, int type_ind = 0, int rotate = 0);
//void UpdateKeyState(entt::registry& registry);

void Tanks::HitBy(int tank_to, int tank_by) {
	//std::cout << "Hit to " << tank_to << " by " << tank_by << '\n';
	if (tank_to < 0 && tank_to >= tanks.size() && tank_by < 0 && tank_by >= tanks.size())
		return;
	if (tank_to < 2 && tank_by>1)
		Destroy(tanks[tank_to]);
	else if (tank_by < 2 && tank_to>1)
		Destroy(tanks[tank_to]);
}

void Tanks::UpdateAi(long count)
{
	for (int i = 0; i < tanks.size(); ++i)
		if (tanks[i].active && tanks[i].ai)
		{
			auto& tank = tanks[i];
			tank.live_time+= (count%64)==0;
			tank.is_move = 1;
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
		if (tanks[i].active && (tanks[i].is_move|| onIce(tanks[i])&& tanks[i].time_in_ice<28))
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
				if (tanks[i].is_move) {
					tank.anim_c = !tank.anim_c;
					tanks[i].time_in_ice = 0;
				}
				else
					++tanks[i].time_in_ice;
			}
		}
		else {
			tanks[i].time_to_move = 0;

		}
}

void Tanks::UpdateSpawning() {
	for (int i = 0; i < tanks.size(); ++i)
		if (!tanks[i].active && tanks[i].time_tospawn > -1) {
			if (tanks[i].time_tospawn == 0) {
				tanks[i].active = 1;
				auto xy = GetPosInWorld(tanks[i]);
				m_level->aplly_map_to_map(m_level->colision_map, m_level->tmp_map = m_level->get_tank_colision(i + 1), xy.x, xy.y, 1);
			}
			--tanks[i].time_tospawn;
		}
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
			if (i ==0)
				sprite.setColor(sf::Color(252, 228, 160));
			else if (i == 1)
				sprite.setColor(sf::Color(176, 252, 204));
			else
				sprite.setColor(sf::Color(252, 252, 252));
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
			ImGui::InputInt("Time to spawn", &tank.time_tospawn);
			ImGui::InputInt("Time in ice", &tank.time_in_ice);
			ImGui::Text("Live time %d", tank.live_time);
			auto xy = GetPosInWorld(tank);
			int x = xy.x, y = xy.y;
			ImGui::Text("%d %d", x, y);
			ImGui::TreePop();
			ImGui::Separator();
		}

	ImGui::End();
}

void Tanks::spawn_tank(sf::Vector2i pos, int rotation, bool ai, int type_ind, int number, int ind) {
	tanks[ind] = { pos ,rotation,0,0,0,0,number,type_ind,0,0,ai,0,ind > 1 ? 60 : 44 };
	//auto xy = GetPosInWorld(tanks[ind]);
	//m_level->aplly_map_to_map(m_level->colision_map, m_level->tmp_map = m_level->get_tank_colision(ind + 1), xy.x, xy.y, 1);
	m_spawnFire->Create(pos, (SpawnFire::Type)(ind > 1));

}

void Tanks::logic(int m_players, int respawn_time, int& atanks, int lives[2]) {
	if (time_to_spawn > -1)
		--time_to_spawn;
	for (int i = 0; i < m_players; ++i)
		if (!tanks[i].active && tanks[i].time_tospawn == -1 && lives[i]) {
			spawn_tank(sf::Vector2i((m_level->act_map.size().x / 2 + (i ? 3 : -5)) * 8 + 8, (m_level->act_map.size().y - 2) * 8 + 8), 0, 0, 0, 0, i);
			--lives[i];
		}
	int ai = -1;
	for (int i = 2; i < 6; ++i)
		if (!tanks[i].active && tanks[i].time_tospawn == -1) {
			ai = i;
			break;
		}
	if (ai != -1 && atanks) {
		if (time_to_spawn == 0)
			spawn_tank(sf::Vector2i((m_level->act_map.size().x - 2) / 2 * 8 * ((spawn_point++) % 3) + 8, 8), 2, 1, rand() % 4 + 4, 21 - atanks--, ai);
		else if (time_to_spawn == -1)
			time_to_spawn = respawn_time;
	}
}
