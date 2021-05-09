#include "Window.h"

Window::Window(const std::string& title, uint32_t width, uint32_t height)
{
    m_window.create(sf::VideoMode(width, height), title);
    m_window.setFramerateLimit(m_fps);
#ifdef _DEBUG
    ImGui::SFML::Init(m_window);
    ImGui::SFML::UpdateFontTexture();
#endif // _DEBUG
    // for(auto const &i:sf::VideoMode::getFullscreenModes()){
     //    std::cout<<i.width<<'x'<<i.height<<' '<<i.bitsPerPixel<<"bit\n";
     //}
    m_title = title;
}

void Window::setToCenter()
{
    sf::VideoMode screenMode = sf::VideoMode::getDesktopMode();
    sf::Vector2u windowSize = m_window.getSize();
    m_window.setPosition(sf::Vector2i(((int)screenMode.width - (int)windowSize.x) / 2,
        ((int)screenMode.height - (int)windowSize.y) / 2));
}

void Window::imguiDraw()
{
    sf::Vector2u windowSize = m_window.getSize();
    sf::Vector2i windowPosition = m_window.getPosition();
    bool fullScreen = m_fullScreen;
    uint32_t fps = m_fps;

    ImGui::Begin("Window");
    if (ImGui::Button("Window to center screen"))
        setToCenter();

    if (ImGui::Checkbox("FullScreen", &fullScreen))
        setFullScreen(fullScreen);

    if (ImGui::InputInt("FPS limit", reinterpret_cast<int*>(&fps), 1, 1))
        setFPSLimit(fps);

    if (ImGui::InputInt2("Window Size", reinterpret_cast<int*>(&windowSize.x)))
        m_window.setSize(windowSize);

    if (ImGui::InputInt2("Window Position", &windowPosition.x))
    {
        m_window.setPosition(windowPosition);
    }

    ImGui::End();
}

bool Window::event(sf::Event& event) {
#ifdef _DEBUG
    ImGui::SFML::ProcessEvent(event);
    ImGuiIO const& imGuiIo = ImGui::GetIO();
    if (imGuiIo.WantCaptureKeyboard || imGuiIo.WantCaptureMouse)
        return true;
#endif // _DEBUG
   

    switch (event.type) {
    case sf::Event::Resized:
        m_window.setView(sf::View(sf::FloatRect(0.f, 0.f, (float)event.size.width, (float)event.size.height)));
        break;
    case sf::Event::KeyPressed:
        switch (event.key.code)
        {
        case sf::Keyboard::F11:
            setFullScreen(!m_fullScreen);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return false;
}

void Window::postEvents()
{
    m_window.clear(sf::Color(20, 20, 20));
#ifdef _DEBUG
    ImGui::SFML::Update(m_window, m_deltaClock.restart());
#endif // _DEBUG
}

void Window::setFullScreen(bool fullScreen)
{
    if (fullScreen && !m_fullScreen)
    {
        m_windowSize = m_window.getSize();
        m_windowPosition = m_window.getPosition();
        m_window.create(sf::VideoMode::getDesktopMode(), m_title, sf::Style::Fullscreen);
    }
    else if (!fullScreen && m_fullScreen)
    {
        m_window.create(sf::VideoMode(m_windowSize.x, m_windowSize.y), m_title);
        m_window.setPosition(m_windowPosition);
    }
    m_window.setFramerateLimit(m_fps);
    m_fullScreen = fullScreen;
}

void Window::setFPSLimit(uint32_t fps)
{
    m_window.setFramerateLimit(fps);
    m_fps = fps;
}

sf::View Window::getStandartView() {
    return sf::View(sf::FloatRect(0.f, 0.f, (float)m_window.getSize().x, (float)m_window.getSize().y));
}
