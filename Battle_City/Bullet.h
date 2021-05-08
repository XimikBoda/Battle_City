#pragma once
#include "Level.h"
#include "Explosion.h"
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
class Tanks;
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
	Tanks* m_tanks;
	long* m_count;
	sf::Texture* texture;
	int CheakColisionWithBullets(int j);
	bool CheakColisionWithTanks(Bullet& bullet);
	bool CheakColisionWithBlocks(Bullet& bullet);
	void DeleteInd(int i);
	void UpdatePos(Bullet& bullet);
public:
	void init(sf::Texture* texture, Level* level, Explosion* explosion, long* count, Tanks* tanks);
	void AddBullet(sf::Vector2i pos, int rotation, int speed, bool power, int tank);
	void Update();
	void Draw(sf::RenderTarget* ren);
};

