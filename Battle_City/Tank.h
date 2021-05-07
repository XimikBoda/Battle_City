#pragma once
#include "Control.h"
#include "Level.h"
#include <vector>
#include <string>
#include <imgui.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
class TankType
{
public:
	struct TankType_d
	{
		int speed; // px(/frame)
		int max_bullet_count;
		int bullet_speed;// px/frame
		sf::Vector2i cord_sprite;
	};
	std::vector<TankType_d> tanks_types =
	{
		{3, 1, 2, {0, 0 * 16}},
		{3, 1, 4, {0, 1 * 16}},
		{3, 2, 2, {0, 2 * 16}},
		{3, 2, 2, {0, 3 * 16}},
		{2, 1, 2, {0, 4 * 16}},
		{4, 1, 4, {0, 5 * 16}},
		{2, 1, 2, {0, 6 * 16}},
		{2, 1, 2, {0, 7 * 16}}
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
	};
	bool draw_colision = false;
	Level* m_level = 0;
	std::vector<Tank> tanks;
	TankType tankType;

	sf::Vector2i GetPosInWorld(const Tank& tank);
	void Rotate(Tank& tank, int new_rotate = 0);
	void changeDirection(Tank& tank);
	void changeDirection2(Tank tank);
public:
	void init(Level* level, sf::Texture* texture);
	//void AddTank(entt::registry& registry, sf::Vector2i pos, int number = 0, int type_ind = 0, int rotate = 0);
	//void UpdateKeyState(entt::registry& registry);
	void UpdateAi();
	void UpdatePos(Level& level);

	void Draw(sf::RenderTarget* ren);
	void DrawColosion(sf::RenderTarget* ren, sf::Vector2f pos = { 0,0 });
	void imguiDraw(Level& m_level);

};
