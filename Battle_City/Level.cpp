#include "Level.h"
#include <fstream>
#include <SFML/Graphics/Sprite.hpp>

void Level::map::resize(int x, int y)
{
	map.clear();
	map.resize(y, std::vector<uint8_t>(x));
}

sf::Vector2i Level::map::size()
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
	map tmp;
	tmp.resize(w, h);
	for (int i = 0; i < buf_s / (w * h); ++i)
	{
		for (int j = 0; j < w * h * 2; ++j)
		{
			uint8_t c = 0xF & (buf[i * w * h + j / 2] >> 4 * !(j % 2));
			aplly_map_to_map(tmp, bin_decode[c], 2 * (j % h), 2 * (j / h));
		}
		maps.push_back(tmp);
	}
	delete[] buf;
}

void Level::aplly_map_to_map(map& a, map& b, int x, int y, bool t)
{
	auto size_a = a.size(), size_b = b.size();
	if (x < 0 || x + size_b.x > size_a.x || y < 0 || y + size_b.y > size_a.y)
		return;
	for (int i = 0; i < size_b.x; ++i)
		for (int j = 0; j < size_b.y; ++j)
			if (t || b.map[j][i])
				a.map[j + y][i + x] = b.map[i][j];
}

void Level::set_map(int index)
{
	act_map = maps[index];
}

void Level::DrawBack(sf::RenderTarget* ren, long count)
{
	sf::Sprite sprite(*texture);
	for (int i = 0; i < act_map.size().x; ++i)
		for (int j = 0; j < act_map.size().y; ++j)
		{
			uint8_t t = act_map.map[j][i];
			if (t == 13)
				t += (count / 32) % 3;
			if (t < 30 && t != 11) 
			{
				sprite.setPosition(i*8,j*8);
				sprite.setTextureRect(sf::IntRect(cord_bl.x + t % 15, cord_bl.y + t / 15, 8, 8));
				ren->draw(sprite);
			}
		}
}

void Level::DrawFront(sf::RenderTarget* ren)
{
	sf::Sprite sprite(*texture);
	for (int i = 0; i < act_map.size().x; ++i)
		for (int j = 0; j < act_map.size().y; ++j)
		{
			uint8_t t = act_map.map[j][i];
			if (t == 11) {
				sprite.setPosition(i * 8, j * 8);
				sprite.setTextureRect(sf::IntRect(cord_bl.x + t % 15, cord_bl.y + t / 15, 8, 8));
				ren->draw(sprite);
			}
		}
}