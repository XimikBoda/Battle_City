#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <string>
#include <SFML/Graphics/RenderTarget.hpp>
#include <entt/entity/fwd.hpp>
class Level
{
	struct Map
	{
		std::vector<std::vector<uint8_t>> map;
		void resize(int x, int y);
		sf::Vector2i size();
		Map() = default;
	};
	struct Col_bl 
	{
		entt::entity en=(entt::entity)-1;
		uint16_t v=0;
	};
	sf::Texture* texture;
	sf::Vector2i cord_bl = { 16 * 2 * 4, 16 * 4 };
	std::vector<Map> maps;
	std::vector<Level::Map> bin_decode =
	{
		{{//0
			{0x00,0x0F},
			{0x00,0x0F}
		}},
		{{//1
			{0x00,0x00},
			{0x0F,0x0F}
		}},
		{{//2
			{0x0F,0x00},
			{0x0F,0x00}
		}},
		{{//3
			{0x0F,0x0F},
			{0x00,0x00}
		}},
		{{//4
			{0x0F,0x0F},
			{0x0F,0x0F}
		}},
		{{//5
			{0x00,0x10},
			{0x00,0x10}
		}},
		{{//6
			{0x00,0x00},
			{0x10,0x10}
		}},
		{{//7
			{0x10,0x00},
			{0x10,0x00}
		}},
		{{//8
			{0x10,0x10},
			{0x00,0x00}
		}},
		{{//9
			{0x10,0x10},
			{0x10,0x10}
		}},
		{{//A
			{0x13,0x13},
			{0x13,0x13}
		}},
		{{//B
			{0x11,0x11},
			{0x11,0x11}
		}},
		{{//C
			{0x12,0x12},
			{0x12,0x12}
		}},
		{{//D
			{0x00,0x00},
			{0x00,0x00}
		}},
		{{//E
			{0x00,0x00},
			{0x00,0x00}
		}},
		{{//F
			{0x00,0x00},
			{0x00,0x00}
		}}
	};
	Map act_map, tank_colision = { { {0xff,0xff},{0xff,0xff}  } };
	std::vector<std::vector<Col_bl>> col_map;
public:
	void init(sf::Texture* texture);
	void load_from_original_binary(const std::string& str, int w = 13, int h = 14);
	void aplly_map_to_map(Map& a, Map& b, int x, int y, bool t = 1);
	bool check_colision_on_curent_map(int x, int y) {
		return x >= 0 && y >= 0 && x < act_map.size().x&& x < act_map.size().y && !act_map.map[y][x] && !act_map.map[y][x + 1] && !act_map.map[y + 1][x] && !act_map.map[y + 1][x + 1];
	}
	void set_map(int index);
	sf::Vector2i get_size_curent_map();
	void DrawBack(sf::RenderTarget* ren, long count, sf::Vector2f pos = { 0,0 });
	void DrawFront(sf::RenderTarget* ren, sf::Vector2f pos = { 0,0 });
	int get_levels_count();
	uint8_t get_block(int x, int y);
	bool is_air(uint8_t bl);
};

