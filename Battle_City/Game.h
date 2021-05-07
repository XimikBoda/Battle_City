#pragma once
#include "Window.h"
#include "Explosion.h"
#include "Level.h"
#include "Interface.h"
#include "Tank.h"
class Game
{
private:
	Window* m_window = 0;
	Explosion m_explosion;
	Level m_level;
	Interface m_interface;
	Tanks m_tanks;
	bool m_run = true;
	sf::Texture m_texure;
	long m_count = 0, m_second = 0, m_r_second = 0;
	int m_select_level = 0;

	//TEST
	bool r_exp = 0;

public:
	Game(Window* window);
	void run();
private:
	void event();

	void postEvents();

	void mainCycles();

	void mainDraw();

	void imguiDraw();

	void render();
};

