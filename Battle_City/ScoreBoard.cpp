#include "ScoreBoard.h"
#include "ResourceManager.h"
#include <fstream>


ScoreBoard::ScoreBoard(Window* window)
{
	m_window = window;
	//if (!font.loadFromFile("barcade-brawl.ttf"))
	//	exit(1);
	LoadFontFromResource (L"barcadebrawlttf",font);
	std::ifstream in("records.json");
	if (in.is_open()) {
		in >> table;
		in.close();
	}
	if (!table.is_array())
		table = {};
	for (int i = 0; i < 10; ++i) {
		if (i >= table.size())
			table.push_back({});
		if (table[i]["name"].is_null())
			table[i]["name"] = "Test " + std::to_string(i + 1);
		if (table[i]["score"].is_null())
			table[i]["score"] = (10-i)*100;
	}
	
}

ScoreBoard::~ScoreBoard()
{
	std::ofstream out("records.json");
	out << table << std::endl;
	out.close();
}


void ScoreBoard::Run() {
	while (run) {
		Events();
		Resize();
		Draw();
	}
}

void ScoreBoard::Add(int score) {
	new_score = score;
	json tmp;
	tmp["name"] = new_name;
	tmp["score"] = score;
	for (int i = 0; i < 10; ++i)
		if (score > table[i]["score"]) {
			table.insert(table.begin() + i, tmp);
			ind = i;
			table.erase(table.begin() + 10);
			break;
		}
}

void ScoreBoard::Events() {
	sf::Event event{};
	while (m_window->m_window.pollEvent(event))
	{

		if (m_window->event(event)) //ImGui and windows event
			break;

		switch (event.type)
		{
		case sf::Event::TextEntered:
			if (event.text.unicode == 8) {
				if (new_name.getSize())
					new_name.erase(new_name.getSize() - 1, 1);
			}
			else
				if(event.text.unicode>=33&& event.text.unicode <= 126)
					new_name += event.text.unicode;
			if(ind!=-1)
				table[ind]["name"] = new_name.toAnsiString();
			break;
		case sf::Event::Closed:
			exit(0);
			break;
		case sf::Event::KeyPressed:
			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
				run = 0;
				break;
			default:
				break;
			}
			break;
		
		}
	}
}

void ScoreBoard::Resize() {
	auto map_size = sf::Vector2i(20, 20);
	sf::Vector2f size((map_size.x) * 8, (map_size.y) * 8), avr((size.x) / 2, (size.y) / 2);
	sf::RectangleShape rs;
	rs.setSize(size);
	rs.setPosition(0, 0);
	sf::View view(sf::FloatRect(0, 0, m_window->m_window.getSize().x, m_window->m_window.getSize().y));
	n = std::min(m_window->m_window.getSize().x / size.x, m_window->m_window.getSize().y / size.y);
	view.setCenter(avr);
	view.zoom(1.0 / float(n));
	m_window->m_window.setView(view);
}

void ScoreBoard::Draw() {
	sf::RenderTarget* ren = &m_window->m_window;
	ren->clear();
	{
		sf::Text txt("Scoreboard", font, 20 / 5 * n);
		txt.scale(1.0 / float(n), 1.0 / float(n));
		txt.setPosition(60, 20);
		ren->draw(txt);
	}
	if(new_score>=0){
		sf::Text txt("You score: "+std::to_string(new_score), font, 20 / 5 * n);
		txt.scale(1.0 / float(n), 1.0 / float(n));
		txt.setPosition(20, 10);
		ren->draw(txt);
	}
	{
		sf::Text txt("Press esc to exit", font, 20 / 5 * n);
		txt.scale(1.0 / float(n), 1.0 / float(n));
		txt.setPosition(10, 19*8);
		ren->draw(txt);
	}
	for (int i = 0; i < 10; ++i) {
		std::string s = table[i]["name"];
		sf::Text txt(s, font, 20 / 5 * n);
		if (i == ind)
			txt.setFillColor(sf::Color::Red);
		txt.scale(1.0 / float(n), 1.0 / float(n));
		txt.setPosition(20, 40 + i * 10);
		ren->draw(txt);
		int ss = table[i]["score"];
		txt.setString(std::to_string(ss));
		txt.setPosition(120, 40 + i * 10);
		ren->draw(txt);
	}
	m_window->m_window.display();
}
