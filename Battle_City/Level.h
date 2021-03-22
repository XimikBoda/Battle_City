#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <string>
#include <SFML/Graphics/RenderTarget.hpp>
class Level
{
	struct map
	{
		std::vector<std::vector<uint8_t>> map;
		void resize(int x, int y);
		sf::Vector2i size();
	};
	sf::Texture* texture;
	sf::Vector2i cord_bl = { 16 * 2, 16 * 4 };
	std::vector<map> maps;
	std::vector<Level::map> bin_decode =
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
	map act_map;
public:
	void init(sf::Texture* texture);
	void load_from_original_binary(const std::string& str, int w=13, int h=14);
	void aplly_map_to_map(map& a, map& b, int x, int y, bool t = 1);
	void set_map(int index);
	void DrawBack(sf::RenderTarget* ren, long count);
	void DrawFront(sf::RenderTarget* ren);
};

