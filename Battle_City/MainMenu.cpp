#include "MainMenu.h"
#include "ScoreBoard.h"
#include "ResourceManager.h"

MainMenu::MainMenu(Window* window)
{
	m_window = window;
	/*if (!font.loadFromFile("barcade-brawl.ttf"))
		exit(1);
	if (!logo.loadFromFile("logo.png"))
		exit(1);*/
	LoadFontFromResource(L"barcadebrawlttf", font);
	LoadTextureFromResource(L"logopng", logo);
	slogo.setTexture(logo);
	slogo.setOrigin(logo.getSize().x / 2, logo.getSize().y / 2);
	slogo.scale(0.5, 0.5);
	slogo.setPosition(10 * 8, logo.getSize().y / 2 + 2);
}

void MainMenu::Run() {
	while (run) {
		Events();
		Resize();
		Draw();
	}
}

void MainMenu::Events() {
	sf::Event event{};
	while (m_window->m_window.pollEvent(event))
	{

		if (m_window->event(event)) //ImGui and windows event
			break;

		switch (event.type)
		{
		case sf::Event::Closed:
			exit(0);
			break;
		case sf::Event::KeyPressed:
			switch (event.key.code)
			{
			case sf::Keyboard::W:
			case sf::Keyboard::Up:
				chose = (chose + 3) % 4;
				break;
			case sf::Keyboard::S:
			case sf::Keyboard::Down:
				chose = (chose + 1) % 4;
				break;
			case sf::Keyboard::A:
			case sf::Keyboard::Left:
				level = (level + 34) % 35;
				break;
			case sf::Keyboard::D:
			case sf::Keyboard::Right:
				level = (level + 1) % 35;
				break;
			case sf::Keyboard::Space:
			case sf::Keyboard::Enter:
			case sf::Keyboard::RControl:
				switch (chose)
				{
				case 0:
				case 1:
				{
					Game game(m_window, chose + 1, level);
					game.run();
				}
				break;
				case 2:
				{
					ScoreBoard sb(m_window);
					sb.Run();
				}
					break;
				case 3:
					run = false;
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
}

void MainMenu::Resize() {
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

void MainMenu::Draw() {
	sf::RenderTarget* ren = &m_window->m_window;
	ren->clear();
	ren->draw(slogo);
	for (int i = 0; i < 4; ++i) {
		sf::Text txt(sf::String(menu_punct[i]), font, 20 / 5 * n);
		txt.scale(1.0 / float(n), 1.0 / float(n));
		txt.setPosition(65, 70 + i * 10);
		if (i == chose)
			txt.setFillColor(sf::Color::Red);
		ren->draw(txt);
	}
	{
		sf::Text txt(sf::String("Start level: " + std::to_string(level + 1)), font, 20/5*n);
		txt.scale(1.0 / float(n), 1.0 / float(n));
		txt.setPosition(10, 3);
		ren->draw(txt);
	}
	{
		sf::Text txt(sf::String("Use WASD or Arrows to navigate on menu \n\nand choose start level\n\nPress Enter or Space to go"), font, 15 / 5 * n);
		txt.scale(1.0 / float(n), 1.0 / float(n));
		txt.setPosition(10, 17 * 8 + 4);
		ren->draw(txt);
	}
	m_window->m_window.display();
}
