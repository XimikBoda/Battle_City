#include "Game.h"
#include "ScoreBoard.h"
#include "ResourceManager.h"

Game::Game(Window* window, int players, int select_level)
{
	m_window = window;
	m_players = players;
	m_select_level = select_level;
	//m_texure.loadFromFile("sprites.png");
	LoadTextureFromResource(L"spritespng", m_texure);
	m_score.init(&m_texure, &m_window->m_window, &m_count);
	m_explosion.init(&m_texure, &m_window->m_window, &m_count, &m_score);
	m_spawnFire.init(&m_texure, &m_window->m_window, &m_count);
	m_interface.init(&m_level,&m_texure);
	m_tanks.init(&m_level, &m_texure,&m_bullets,&m_explosion,&m_controls,&m_spawnFire,&destroed);
	m_bullets.init(&m_texure,&m_level,&m_explosion,&m_count,&m_tanks);
	m_level.init(&m_texure);
	m_level.load_from_original_binary();
	m_level.set_map(m_select_level%35);
	m_level.spawn_staff(0);
}
void Game::run()
{
	while (m_run)
	{
		event();
		postEvents();
		mainCycles();
		mainDraw();
#ifdef _DEBUG
		imguiDraw();
#endif // _DEBUG
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
				//m_explosion.Create(m_count, &m_window->m_window, worldPos, Explosion::Big, 0);
				break;
			case sf::Mouse::Button::Right:
				//m_explosion.Create(m_count, &m_window->m_window, worldPos, Explosion::Small, 0);
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
	if (gameOver > -1)
		gameOver--;
	if (gameOver == 0) {
		for (int i = 0; i < 2; ++i)
			for (int j = 0; j < 4; ++j)
				score += destroed[i][j]*(j+1)*100;
		ScoreBoard sb(m_window);
		sb.Add(score);
		sb.Run();
		m_run = false;
	}
	m_tanks.logic(m_players,190-m_select_level*4- (m_players - 1) * 20,atanks,lives);
	if ((!m_tanks.Cheak_lives(m_players, lives) || !m_level.cheak_staff())&&gameOver==-1)
		gameOver = 64;
	if (!m_tanks.Cheak_enemy(atanks)) {
		m_select_level += 1;
		m_level.set_map(m_select_level % 35);
		m_level.spawn_staff(0);
		m_tanks.Desctoy_all();
		m_count = 0, m_second = 0, m_r_second = 0;
		lives[0]++;
		lives[1]++;
		atanks = 20;
		gameOver = -1;
		for (int i = 0; i < 2; ++i)
			for (int j = 0; j < 4; ++j)
				score += destroed[i][j] * (j + 1) * 100;
		destroed = { {0,0,0,0}, {0,0,0,0} };
	}

	m_controls.Update();
	m_interface.Update(&m_window->m_window);
	/*if (r_exp)
		m_explosion.Create(m_registry, m_window->m_window.mapPixelToCoords(sf::(rand() % m_window->m_window.getSize().x,
			rand() % m_window->m_window.getSize().y)), Explosion::Big, 0);*/
	m_bullets.Update();
	m_tanks.UpdateSpawning();
	m_tanks.UpdateAi(m_count);
	m_tanks.UpdateP();
	m_tanks.UpdatePos();
	m_spawnFire.Update(m_count);
	m_explosion.Update(m_count);
	m_score.Update(m_count);
}

void Game::mainDraw()
{
	m_interface.Draw(&m_window->m_window,atanks,m_players,lives[0],lives[1], m_select_level+1);
	m_level.DrawBack(&m_window->m_window, m_count);
	m_tanks.Draw(&m_window->m_window);
	m_bullets.Draw(&m_window->m_window);
	m_level.DrawFront(&m_window->m_window);
	m_spawnFire.Draw();
	m_explosion.Draw();
	m_score.Draw();
	m_tanks.DrawColosion(&m_window->m_window);
	if (gameOver > -1)
		m_interface.ShowGameOver(&m_window->m_window);

}

void Game::imguiDraw()
{
	ImGui::ShowDemoWindow();
	m_window->imguiDraw();
	m_tanks.imguiDraw();
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
#ifdef _DEBUG
	ImGui::SFML::Render(m_window->m_window);
#endif // _DEBUG
	m_window->m_window.display();
	m_count++;
	m_second += m_count % 64 == 0;
	m_r_second += m_count % 60 == 0;
}