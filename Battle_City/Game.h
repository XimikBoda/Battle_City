#pragma once
#include "Window.h"
#include "Explosion.h"
class Game
{
private:
	Window* m_window = 0;
	Explosion m_explosion;
	bool m_run = true;
	sf::Texture m_texure;
	long m_count = 0, m_second = 0, m_r_second = 0;

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

