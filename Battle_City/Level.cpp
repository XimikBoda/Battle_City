#include "Level.h"
#include <fstream>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

void Level::Map::resize(int x, int y)
{
	map.clear();
	map.resize(y, std::vector<uint8_t>(x));
}

sf::Vector2i Level::Map::size()
{
	return sf::Vector2i((map.size() ? map[0].size() : 0), map.size());
}


void Level::init(sf::Texture* texture)
{
	this->texture = texture;
}

void Level::load_from_original_binary(const std::string& str, int w, int h)
{
	std::ifstream in(str, std::ios::binary);
	in.seekg(0, std::ios::end);
	auto buf_s = in.tellg();
	in.seekg(0, std::ios::beg);
	unsigned char* buf = new unsigned char[buf_s];
	in.read((char*)buf, buf_s);
	in.close();
	Map tmp;
	tmp.resize(w*2, h*2);
	for (int i = 0; i < buf_s / (w * h / 2); ++i)
	{
		for (int j = 0; j < w * h; ++j)
		{
			uint8_t c = 0xF & (buf[i * w * h / 2 + j / 2] >> 4 * !(j % 2));
			aplly_map_to_map(tmp, bin_decode[c], 2 * (j % h), 2 * (j / h));
		}
		maps.push_back(tmp);
	}
	delete[] buf;
}

void Level::aplly_map_to_map(Map& a, Map& b, int x, int y, bool t)
{
	auto size_a = a.size(), size_b = b.size();
	if (x < 0 || x + size_b.x > size_a.x || y < 0 || y + size_b.y > size_a.y)
		return;
	for (int i = 0; i < size_b.x; ++i)
		for (int j = 0; j < size_b.y; ++j)
			if (t || b.map[j][i])
				a.map[j + y][i + x] = b.map[j][i];
}

void Level::set_map(int index)
{
	act_map = maps[index];
}

sf::Vector2i Level::get_size_curent_map() {
	return act_map.size();
}

void Level::DrawBack(sf::RenderTarget* ren, long count, sf::Vector2f pos)
{
	sf::RectangleShape rs;
	rs.setFillColor(sf::Color::Black);
	rs.setSize(sf::Vector2f(act_map.size().x*8, act_map.size().y*8));
	ren->draw(rs);
	sf::Sprite sprite(*texture);
	for (int i = 0; i < act_map.size().x; ++i)
		for (int j = 0; j < act_map.size().y; ++j)
		{
			uint8_t t = act_map.map[j][i];
			if (t == 0x13)
				t += (count / 32) % 2;
			if (t < 30 && t != 0x11) 
			{
				sprite.setPosition(i*8,j*8);
				sprite.move(pos);
				sprite.setTextureRect(sf::IntRect(cord_bl.x + 8*(t % 15), cord_bl.y + 8*(t / 15), 8, 8));
				ren->draw(sprite);
			}
		}
}

void Level::DrawFront(sf::RenderTarget* ren, sf::Vector2f pos)
{
	sf::Sprite sprite(*texture);
	for (int i = 0; i < act_map.size().x; ++i)
		for (int j = 0; j < act_map.size().y; ++j)
		{
			uint8_t t = act_map.map[j][i];
			if (t == 0x11) {
				sprite.setPosition(i * 8, j * 8);
				sprite.move(pos);
				sprite.setTextureRect(sf::IntRect(cord_bl.x + 8 * (t % 15), cord_bl.y + 8 * (t / 15), 8, 8));
				ren->draw(sprite);
			}
		}
}

int Level::get_levels_count()
{
	return maps.size();
}
