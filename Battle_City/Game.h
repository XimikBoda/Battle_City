#pragma once
#include "Window.h"
#include "Explosion.h"
#include "Level.h"
#include "Interface.h"
#include "Tank.h"
#include "Controls.h"
#include "Bullet.h"
#include "Score.h"
#include "SpawnFire.h"
class Game
{
private:
	Window* m_window = 0;
	Explosion m_explosion;
	Level m_level;
	Interface m_interface;
	Tanks m_tanks;
	Controls m_controls;
	Bullets m_bullets;
	Score m_score;
	SpawnFire m_spawnFire;
	bool m_run = true;
	sf::Texture m_texure;
	long m_count = 0, m_second = 0, m_r_second = 0;
	int m_select_level = 0;
	int m_players = 0;
	int anim_t = 20;
	
	
	int atanks = 20;
	int lives[2] = { 3,3 };
	std::vector<std::vector<int>> destroed = { {0,0,0,0}, {0,0,0,0} };
	int gameOver = -1;
	int score = 0;

	//TEST
	bool r_exp = 0;

public:
	Game(Window* window, int players, int select_level);
	void run();
private:
	void event();

	void postEvents();

	void mainCycles();

	void mainDraw();

	void imguiDraw();

	void render();


};

