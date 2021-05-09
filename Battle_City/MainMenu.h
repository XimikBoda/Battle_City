#pragma once
#include "Window.h"
#include "Game.h"
class MainMenu
{
	Window* m_window = 0;
	int chose = 0;
	int level = 0;
	int n=1;
	bool run = 1;
	sf::Font font;
	sf::Texture logo;
	sf::Sprite slogo;
	std::string menu_punct[4] = { "1 Player" ,"2 Players" ,"Records","Exit" };
public:
	MainMenu(Window* window);
	void Run();
private:
	void Events();
	void Resize();
	void Draw();
};

