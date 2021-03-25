#include "Game.h"

Game::Game(Window* window) 
{
	m_window = window;
	m_texure.loadFromFile("sprites.png");
	m_explosion.init(&m_texure);
	m_interface.init(&m_level);
	m_tankType.init(&m_texure);
	m_level.init(&m_texure);
	m_level.load_from_original_binary("standart_levels.bin");
	m_level.set_map(0);
	Tank::System::AddTank(m_registry, { 8,8 });
	Control::System::Init(m_registry);
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
				m_explosion.Create(m_registry, worldPos, Explosion::Big, 0);
				break;
			case sf::Mouse::Button::Right:
				m_explosion.Create(m_registry, worldPos, Explosion::Small, 0);
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
	Control::System::ResertStates(m_registry);
	m_window->postEvents();
}

void Game::mainCycles() 
{
	Control::System::KeyboardMovement(m_registry);
	Control::System::ApplyMovement(m_registry);

	Tank::System::UpdateKeyState(m_registry);
	Tank::System::UpdateRotation(m_registry);
	Tank::System::UpdatePos(m_registry,m_tankType);
	Tank::System::UpdateSprites(m_registry, m_tankType);

	m_interface.Update(&m_window->m_window);
	if(r_exp)
		m_explosion.Create(m_registry,m_window->m_window.mapPixelToCoords(sf::Vector2i(rand()%m_window->m_window.getSize().x, 
				rand() % m_window->m_window.getSize().y)), Explosion::Big, 0);

	m_explosion.Update(m_registry);
}

void Game::mainDraw() 
{
	m_interface.Draw(&m_window->m_window);
	m_level.DrawBack(&m_window->m_window, m_count);
	Render::System::Draw<Type::Tank>(m_registry, &m_window->m_window);//m_tank.Draw(&m_window->m_window);

	m_level.DrawFront(&m_window->m_window);
	Render::System::Draw<Type::Explosion>(m_registry, &m_window->m_window);//m_explosion.Draw();
}

void Game::imguiDraw() 
{
	ImGui::ShowDemoWindow();
	m_window->imguiDraw();
	Tank::System::imguiDraw(m_registry);
	ImGui::Begin("Debug");
	ImGui::Text("Frame count %d", m_count);
	ImGui::Text("Second count %d", m_second);
	ImGui::Text("Real second count %d", m_r_second);
	ImGui::Checkbox("Random explosion", &r_exp);
	if(ImGui::SliderInt("Level", &m_select_level, 0, m_level.get_levels_count()-1))
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