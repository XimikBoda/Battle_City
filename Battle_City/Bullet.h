#pragma once
#include "Level.h"
#include "Explosion.h"
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
class Bullets
{
	struct Bullet
	{
		sf::Vector2i pos = { 8,8 };
		int rotation = 0;//0 - up, 1 - right, 2 - down, 3 - left
		int speed = 2;
		bool power = 0;
		int tank = -1;
	};
	std::vector<Bullet> bullets;
	Level* m_level;
	Explosion* m_explosion; 
	long* m_count;
	sf::Texture* texture;
public:
	void init(sf::Texture* texture, Level* level, Explosion* explosion, long* count) {
		this->texture = texture;
		m_level = level;
		m_explosion = explosion;
		m_count = count;
	}
	void AddBullet(sf::Vector2i pos, int rotation, int speed, bool power, int tank) {
		for (int i = 0; i < bullets.size(); ++i)
			if (bullets[i].tank == tank)
				return;
		bullets.push_back({ pos,rotation,speed,power,tank });
	}
	int CheakColisionWithBullets(int j)
	{
		for (int i = j + 1; i < bullets.size(); ++i)
			if (abs(bullets[i].pos.x - bullets[j].pos.x) <= 2 && abs(bullets[i].pos.y - bullets[j].pos.y) <= 2)
				return i;
		return -1;
	}
	bool CheakColisionWithBlocks(Bullet& bullet)
	{
		const sf::Vector2i ps[4][2] = { {{-1,-2},{1,-2}},{{2,-1},{2,1}},{{-1,2},{1,2}},{{-2,-1},{-2,1}} };
		const uint8_t tst[4] = { 0b0011, 0b1010, 0b1100, 0b0101 };
		uint8_t bl[2];
		sf::Vector2i xy[2];
		if (bullet.pos.x <0 || bullet.pos.y <0)
			return 1;
		bool ch = 0;
		for (int i = 0; i < 2; ++i) {
			xy[i].x = (bullet.pos.x + ps[bullet.rotation][i].x) / 8, xy[i].y = (bullet.pos.y + ps[bullet.rotation][i].y) / 8;
			bl[i] = m_level->get_block(xy[i].x, xy[i].y);
			
		}
		if (bl[0]+ bl[1]!=0&&bl[0] >= 0 && bl[1] >= 0 && bl[0] != 16 && bl[1] != 16 && !(bl[0] & ~tst[bullet.rotation]) && !(bl[1] & ~tst[bullet.rotation]))
		{
			m_level->set_block(xy[0].x, xy[0].y, 0);
			m_level->set_block(xy[1].x, xy[1].y, 0);
			ch = 1;
		}
		else
			for (int i = 0; i < 2; ++i) {
				if (bl[i] > 0 && bl[i] <= 15)
				{
					m_level->set_block(xy[i].x, xy[i].y, bl[i] & tst[bullet.rotation]);
					ch = 1;
				}else if (bl[i] == 0xff || (!bullet.power && bl[i] == 16))
					ch = 1;
				else if (bullet.power && bl[i] > 0 && bl[i] <= 16) {
					m_level->set_block(xy[i].x, xy[i].y, 0);
					ch = 1;
				}
			}
		return ch;
	}
	void DeleteInd(int i) {
		m_explosion->Create(*m_count, bullets[i].pos);
		std::swap(bullets[i], bullets[bullets.size() - 1]);
		bullets.resize(bullets.size() - 1);
	}

	void UpdatePos(Bullet& bullet) {
		sf::Vector2i moveVec((bullet.rotation == 1) - (bullet.rotation == 3), (bullet.rotation == 2) - (bullet.rotation == 0));
		moveVec.x *= bullet.speed;
		moveVec.y *= bullet.speed;
		bullet.pos += moveVec;
	}
	void Update() {
		for (int i = 0; i < bullets.size(); ++i) {
			UpdatePos(bullets[i]);
			if (CheakColisionWithBlocks(bullets[i])) {
				DeleteInd(i--);
				continue;
			}
			int j = CheakColisionWithBullets(i);
			if (j != -1) {
				DeleteInd(j);
				DeleteInd(i);
				i -= 1;
				continue;
			}
		}
	}
	void Draw(sf::RenderTarget* ren) {
		sf::Sprite sprite;
		sprite.setOrigin(8, 8);
		sprite.setTexture(*texture);
		sprite.setOrigin(4, 5);
		for (int i = 0; i < bullets.size(); ++i)
		{
			sprite.setTextureRect(sf::IntRect(192 + 8 * (bullets[i].rotation % 4), 48, 8, 8));
			sprite.setPosition(bullets[i].pos.x, bullets[i].pos.y);
			ren->draw(sprite);
		}
	}
};

