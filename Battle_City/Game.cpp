#include "Game.h"

Game::Game(Window* window) {
	m_window = window;
	m_texure.loadFromFile("sprites.png");
	m_explosion.init(&m_texure);
}
void Game::run() {
	while (m_run) {
		event();
		postEvents();
		mainCycles();
		mainDraw();
		imguiDraw();
		render();
	}
}

void Game::event() {
	sf::Event event{};
	while (m_window->m_window.pollEvent(event)) {

		if (m_window->event(event)) //ImGui and windows event
			break;

		switch (event.type) {
		case sf::Event::Closed:
			exit(0);
			break;
		case sf::Event::MouseButtonPressed:
			switch (event.mouseButton.button){
			case sf::Mouse::Button::Left:
				m_explosion.Create(m_count, &m_window->m_window, sf::Vector2f(event.mouseButton.x, event.mouseButton.y), Explosion::Big, 0);
				break;
			case sf::Mouse::Button::Right:
				m_explosion.Create(m_count, &m_window->m_window, sf::Vector2f(event.mouseButton.x, event.mouseButton.y), Explosion::Small, 0);
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

void Game::postEvents() {
	m_window->postEvents();
}

void Game::mainCycles() {
	m_explosion.Update(m_count);
}

void Game::mainDraw() {
	m_explosion.Draw();
}

void Game::imguiDraw() {
	ImGui::ShowDemoWindow();
	m_window->imguiDraw();
	ImGui::Begin("Debug");
	ImGui::Text("Frame count %d", m_count);
	ImGui::Text("Second count %d", m_second);
	ImGui::Text("Real second count %d", m_r_second);
	ImGui::End();
}

void Game::render() {
	ImGui::SFML::Render(m_window->m_window);
	m_window->m_window.display();
	m_count++;
	m_second += m_count % 64 == 0;
	m_r_second += m_count % 60 == 0;
}