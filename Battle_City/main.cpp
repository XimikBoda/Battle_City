#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>


int main() {
	sf::RenderWindow win(sf::VideoMode(640, 480), "ImGui + SFML = <3");
	win.setFramerateLimit(60);
	ImGui::SFML::Init(win);

	std::ifstream in("standart_levels.bin", std::ios_base::binary);
	in.seekg(0, std::ios::end);
	auto buf_s = in.tellg();
	in.seekg(0, std::ios::beg);
	unsigned char* buf = new unsigned char[buf_s];
	in.read((char*)buf, buf_s);
	in.close();

	sf::Texture tex;
	tex.loadFromFile("temp.png");
	sf::Sprite sp(tex);

	int n = 0;

	sf::Clock deltaClock;
	while (win.isOpen()) {
		sf::Event event;
		while (win.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);
			switch (event.type) {
			case sf::Event::Closed:
				return 0;
				break;
			case sf::Event::Resized:
				win.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
				break;
			case sf::Event::KeyPressed:
				switch (event.key.code) {
				}
			}
		}

		ImGui::SFML::Update(win, deltaClock.restart());

		ImGui::Begin("Test");
		ImGui::SliderInt("Level", &n, 0, buf_s / 91);
		ImGui::End();

		for (int i = 0; i < 91 * 2; ++i) {
			unsigned char c = 0x0f & (buf[n * 91 + i / 2] >> 4 * !(i % 2));
			sp.setTextureRect(sf::IntRect(64 * (c % 4), 64 * (c / 4), 64, 64));
			sp.setPosition(64 * (i % 14), 64 * (i / 14));
			win.draw(sp);
		}

		ImGui::SFML::Render(win);
		win.display();
		win.clear();
	}

	ImGui::SFML::Shutdown();
}