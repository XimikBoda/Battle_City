#pragma once
#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

class Window {
public:
    explicit Window(const std::string &title = "Battle City", uint32_t width = 800, uint32_t height = 600);

    void setToCenter();

    void imguiDraw();

    bool event(sf::Event& event);

    void postEvents();

    void setFullScreen(bool fullScreen);

    void setFPSLimit(uint32_t fps);


    sf::RenderWindow m_window;
    bool m_fullScreen = false;
    sf::Clock m_deltaClock;
    sf::Vector2u m_windowSize;
    sf::Vector2i m_windowPosition;
    std::string m_title;
    uint32_t m_fps=60;

};

