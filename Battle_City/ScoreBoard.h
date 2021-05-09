#pragma once
#include "Window.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
class ScoreBoard
{
	Window* m_window = 0;
	bool run = 1;
	json table;
	int n = 0;
	sf::Font font;
	int new_score=-1;
	int ind = -1;
	sf::String new_name="Type your name";
public:
	ScoreBoard(Window* window);
	void Run();
	~ScoreBoard();
	void Add(int score);
private:
	void Events();
	void Resize();
	void Draw();
};

