#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <string>
#include <SFML/Graphics/RenderTarget.hpp>
	
class Level
{
public:
	struct Map
	{
		std::vector<std::vector<uint8_t>> map;
		void resize(int x, int y);
		void resize(const sf::Vector2i& siz);
		sf::Vector2i size();
		Map() = default;
	};
private:
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
public:
	Map act_map, colision_map,  empty_colisoin = { { {0,0},{0,0}  } },tmp_map;
	Map get_tank_colision(int tank_number);
	void init(sf::Texture* texture);
	void load_from_original_binary(const std::string& str, int w = 13, int h = 14);
	void aplly_map_to_map(Map& a, Map& b, int x, int y, bool t = 1);
	bool check_colision_on_curent_map(int x, int y);
	void set_map(int index);
	sf::Vector2i get_size_curent_map();
	void DrawBack(sf::RenderTarget* ren, long count, sf::Vector2f pos = { 0,0 });
	void DrawFront(sf::RenderTarget* ren, sf::Vector2f pos = { 0,0 });
	int get_levels_count();
	uint8_t get_block(int x, int y);
	void set_block(int x, int y, uint8_t bl);
	uint8_t get_collision(int x, int y);
	bool is_air(int x, int y, bool cheak_col=0);
	bool is_air(uint8_t bl);
};