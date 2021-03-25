#pragma once
#include "Render.h"
#include "Control.h"
#include <vector>
#include <string>
#include <imgui.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <entt\entity\registry.hpp>
#include <entt/entity/handle.hpp>
namespace Type {
	class Tank {};
};
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

namespace Tank
{
	namespace Event
	{
		struct NeedRotate
		{
			int new_rotate;
		};
	}
	namespace Component
	{
		struct KeyState
		{
			bool up = 0, down = 0, right = 0, left = 0, shot = 0;
		};
		struct MoveData
		{
			sf::Vector2i pos = { 0,0 };
			int rotation = 0;//0 - up, 1 - right, 2 - down, 3 - left
			int time_to_move = 0;
			bool is_move = 0;
		};
		struct Data
		{
			int number = 0;
			int type_ind = 0;
			int anim_c=0;
		};
		struct MapPos
		{
			sf::Vector2i pos = { 0,0 };
		};
	}
	namespace System {
		void AddTank(entt::registry& registry, sf::Vector2i pos, int number = 0, int type_ind = 0, int rotate = 0);
		void UpdateKeyState(entt::registry& registry);
		void UpdateRotation(entt::registry& registry);
		void UpdatePos(entt::registry& registry, TankType& tankType);
		void UpdateSprites(entt::registry& registry, TankType& tankType);
		void imguiDraw(entt::registry& registry);
	}
};

