#pragma once
#include "Controls.h"
#include "Level.h"
#include "Bullet.h"
#include "SpawnFire.h"
#include <vector>
#include <string>
#include <imgui.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>
class TankType
{
public:
	struct TankType_d
	{
		int speed; // px(/frame)
		int max_bullet_count;
		int bullet_speed;// px/frame
		bool bullet_power;
		sf::Vector2i cord_sprite;
	};
	std::vector<TankType_d> tanks_types =
	{
		{3, 1, 2, 0, {0, 0 * 16}},
		{3, 1, 4, 0, {0, 1 * 16}},
		{3, 2, 2, 0, {0, 2 * 16}},
		{3, 2, 2, 1, {0, 3 * 16}},
		{2, 1, 2, 0, {0, 4 * 16}},
		{4, 1, 4, 0, {0, 5 * 16}},
		{2, 1, 2, 0, {0, 6 * 16}},
		{2, 1, 2, 0, {0, 7 * 16}}
	};
	sf::Texture* texture;
	void init(sf::Texture* texture)
	{
		this->texture = texture;
	}
};
class Tanks
{
	struct Tank {
		sf::Vector2i pos = { 8,8 };
		int rotation = 0;//0 - up, 1 - right, 2 - down, 3 - left
		int time_to_move = 0;
		bool is_move = 0;
		bool can_move = true;
		bool active = false;
		int number = 0;
		int type_ind = 0;
		int anim_c = 0;
		unsigned char live_time = 0;
		bool ai = 0;
		int state = 0;
		int time_tospawn = -1;
		int time_in_ice = 0;
	};
	bool draw_colision = false;
	TankType tankType;
	std::vector<Tank> tanks;
	Level* m_level;
	Bullets* m_bullets;
	Explosion* m_explosion;
	Controls* m_controls;
	SpawnFire* m_spawnFire;
	int time_to_spawn = 1;
	int spawn_point = 0;
	std::vector<std::vector<int>> *destroed;

	sf::Vector2i GetPosInWorld(const Tank& tank);
	void Rotate(Tank& tank, int new_rotate = 0);
	void changeDirection(Tank& tank);
	void changeDirection2(Tank tank);
	void Destroy(Tank& tank);
	bool onIce(Tank& tank);
public:
	void init(Level* level, sf::Texture* texture, Bullets* bullets, Explosion* explosion,
		Controls* controls, SpawnFire* spawnFire, std::vector<std::vector<int>>* destroed);
	//void AddTank(entt::registry& registry, sf::Vector2i pos, int number = 0, int type_ind = 0, int rotate = 0);
	//void UpdateKeyState(entt::registry& registry);
	void HitBy(int tank_to, int tank_by);
	void UpdateAi(long m_count);
	void UpdateP();
	void UpdatePos();
	void UpdateSpawning();

	void Draw(sf::RenderTarget* ren);
	void DrawColosion(sf::RenderTarget* ren, sf::Vector2f pos = { 0,0 });
	void imguiDraw();
	void spawn_tank(sf::Vector2i pos, int rotation, bool ai, int type_ind, int number, int ind);
	void logic(int m_players, int respawn_time, int& atanks, int lives[2]);
	bool Cheak_lives(int m_players, int lives[2]);
	bool Cheak_enemy(int atanks);
	void Desctoy_all() {
		for (int i = 0; i < 6; ++i)
			Destroy(tanks[i]);
	}

};
