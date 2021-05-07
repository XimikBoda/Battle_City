#include "Game.h"

Game::Game(Window* window) 
{
	m_window = window;
	m_texure.loadFromFile("sprites.png");
	m_explosion.init(&m_texure);
	m_interface.init(&m_level);
	m_tanks.init(&m_level,&m_texure);
	m_level.init(&m_texure);
	m_level.load_from_original_binary("standart_levels.bin");
	m_level.set_map(0);
}
void Game::run() 
{
	while (m_run) 
	{
		event();
		postEvents();
		mainCycles();
		mainDraw();
		imguiDraw();
		render();
	}
}

void Game::event() 
{
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
		case sf::Event::MouseButtonPressed:
		{
			sf::Vector2f worldPos = m_window->m_window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
			switch (event.mouseButton.button)
			{
			case sf::Mouse::Button::Left:
				m_explosion.Create(m_count, &m_window->m_window, worldPos, Explosion::Big, 0);
				break;
			case sf::Mouse::Button::Right:
				m_explosion.Create(m_count, &m_window->m_window, worldPos, Explosion::Small, 0);
				break;
			default:
				break;
			}
			break;
		}
		default:
			break;
		}
	}
}

void Game::postEvents() 
{
	m_window->postEvents();
}

void Game::mainCycles() 
{
	m_interface.Update(&m_window->m_window);
	/*if (r_exp)
		m_explosion.Create(m_registry, m_window->m_window.mapPixelToCoords(sf::(rand() % m_window->m_window.getSize().x,
			rand() % m_window->m_window.getSize().y)), Explosion::Big, 0);*/
	m_tanks.UpdateAi();
	m_tanks.UpdatePos(m_level);
	m_explosion.Update(m_count);
}

void Game::mainDraw() 
{
	m_interface.Draw(&m_window->m_window);
	m_level.DrawBack(&m_window->m_window,m_count);
	m_tanks.Draw(&m_window->m_window);
	m_level.DrawFront(&m_window->m_window);
	m_explosion.Draw();
	m_tanks.DrawColosion(&m_window->m_window);
}

void Game::imguiDraw() 
{
	ImGui::ShowDemoWindow();
	m_window->imguiDraw();
	m_tanks.imguiDraw(m_level);
	ImGui::Begin("Debug");
	ImGui::Text("Frame count %d", m_count);
	ImGui::Text("Second count %d", m_second);
	ImGui::Text("Real second count %d", m_r_second);
	ImGui::Checkbox("Random explosion", &r_exp);
	if (ImGui::SliderInt("Level", &m_select_level, 0, m_level.get_levels_count() - 1))
		m_level.set_map(m_select_level);
	ImGui::End();
}

void Game::render() 
{
	ImGui::SFML::Render(m_window->m_window);
	m_window->m_window.display();
	m_count++;
	m_second += m_count % 64 == 0;
	m_r_second += m_count % 60 == 0;
}