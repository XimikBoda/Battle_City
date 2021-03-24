#pragma once
#include <vector>
#include <string>
#include <imgui.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
class Tank
{
	struct TankType
	{
		int speed; // px(/frame)
		int max_bullet_count;
		int bullet_speed;// px/frame
		sf::Vector2i cord_sprite;
	};
	struct Active_Tank
	{
		int number = 0;
		int type_ind = 0;
		bool active = 0;
		sf::Vector2i pos = { 0,0 };
		int time_to_move = 0;
		int rotate = 0;//0 - up, 1 - right, 2 - down, 3 - left
		sf::Sprite sprite;
	};
	std::vector<TankType> tanks_types =
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
	std::vector<Active_Tank> tanks;
	sf::Texture* texture;
public:
	void init(sf::Texture* texture)
	{
		this->texture = texture;
		tanks.push_back({ 0,0,0,{0,0},0,0 });
	}
	void Update() {
		for (int i = 0; i < tanks.size(); ++i) {
			int tmp = tanks[i].rotate % 4;
			tanks[i].sprite.setColor(sf::Color(250, 230, 158));
			tanks[i].sprite.setPosition(tanks[i].pos.x+ 16*(tmp==1||tmp==2), tanks[i].pos.y + 16*(tmp==2||tmp==3));
			tanks[i].sprite.setRotation(tanks[i].rotate *90);
			tanks[i].sprite.setTexture(*texture);
			tanks[i].sprite.setTextureRect(sf::IntRect(tanks_types[tanks[i].type_ind].cord_sprite.x, tanks_types[tanks[i].type_ind].cord_sprite.y, 16, 16));
		}
	}
	void Draw(sf::RenderTarget* ren) {
		for (int i = 0; i < tanks.size(); ++i) {
			ren->draw(tanks[i].sprite);
		}
	}
	void imguiDraw(){
		ImGui::Begin("Tanks");
		for (int i = 0; i < tanks.size(); ++i) 
			if (ImGui::CollapsingHeader(std::string("Tank " + std::to_string(i + 1)).c_str())) {
				ImGui::InputInt("Number", &tanks[i].number);
				ImGui::InputInt("Type", &tanks[i].type_ind);
				ImGui::Checkbox("Active", &tanks[i].active);
				ImGui::InputInt2("Pos", &tanks[i].pos.x);
				ImGui::InputInt("Time to move", &tanks[i].time_to_move);
				//ImGui::SliderInt("Rotate", &tanks[i].rotate, 0, 360);
				ImGui::InputInt("Rotate", &tanks[i].rotate);
			}
				
		ImGui::End();
	}
};

