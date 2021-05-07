#include "Level.h"
//#include "Tank.h"
#include <fstream>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

sf::Color HSVtoRGB(long long H, double S, double V) {
	H %= 360;
	double C = S * V;
	double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
	double m = V - C;
	double Rs, Gs, Bs;
	if (H >= 0 && H < 60) {
		Rs = C;
		Gs = X;
		Bs = 0;
	}
	else if (H >= 60 && H < 120) {
		Rs = X;
		Gs = C;
		Bs = 0;
	}
	else if (H >= 120 && H < 180) {
		Rs = 0;
		Gs = C;
		Bs = X;
	}
	else if (H >= 180 && H < 240) {
		Rs = 0;
		Gs = X;
		Bs = C;
	}
	else if (H >= 240 && H < 300) {
		Rs = X;
		Gs = 0;
		Bs = C;
	}
	else {
		Rs = C;
		Gs = 0;
		Bs = X;
	}
	return sf::Color((Rs + m) * 255, (Gs + m) * 255, (Bs + m) * 255);
}

void Level::Map::resize(int x, int y)
{
	map.clear();
	map.resize(y, std::vector<uint8_t>(x));
}

void Level::Map::resize(const sf::Vector2i& siz)
{
	resize(siz.x, siz.y);
}

sf::Vector2i Level::Map::size()
{
	return sf::Vector2i((map.size() ? map[0].size() : 0), map.size());
}


Level::Map Level::get_tank_colision(int tank_number) {
	Map tmp = empty_colisoin;
	tmp.map[0][0] = tmp.map[0][1] = tmp.map[1][0] = tmp.map[1][1] = tank_number;
	return tmp;
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
	tmp.resize(w * 2, h * 2);
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

bool Level::check_colision_on_curent_map(int x, int y) {
	return x >= 0 && y >= 0 && x < act_map.size().x&& x < act_map.size().y && !act_map.map[y][x] && !act_map.map[y][x + 1] && !act_map.map[y + 1][x] && !act_map.map[y + 1][x + 1];
}

void Level::set_map(int index)
{
	act_map = maps[index];
	colision_map.resize(act_map.size());
}

sf::Vector2i Level::get_size_curent_map() {
	return act_map.size();
}

void Level::DrawBack(sf::RenderTarget* ren, long count, sf::Vector2f pos)
{
	sf::RectangleShape rs;
	rs.setFillColor(sf::Color::Black);
	rs.setSize(sf::Vector2f(act_map.size().x * 8, act_map.size().y * 8));
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
				sprite.setPosition(i * 8, j * 8);
				sprite.move(pos);
				sprite.setTextureRect(sf::IntRect(cord_bl.x + 8 * (t % 15), cord_bl.y + 8 * (t / 15), 8, 8));
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

uint8_t Level::get_block(int x, int y) {
	if (x >= 0 && y >= 0 && x < act_map.size().x && y < act_map.size().y)
		return act_map.map[y][x];
	else
		return -1;
}
uint8_t Level::get_collision(int x, int y) {
	if (x >= 0 && y >= 0 && x < act_map.size().x && y < act_map.size().y)
		return colision_map.map[y][x];
	else
		return 0;
}

bool Level::is_air(uint8_t bl)
{
	return bl == 0 || bl == 18 || bl == 17;
}
bool Level::is_air(int x, int y, bool cheak_col)
{
	if (cheak_col && get_collision(x, y))
		return false;
	else
		return is_air(get_block(x, y));
}
