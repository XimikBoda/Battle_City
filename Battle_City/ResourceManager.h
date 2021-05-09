#pragma once
#include <string>
#include <SFML/Graphics/Font.hpp>

void LoadFontFromResource(const std::wstring& name, sf::Font& font);
void LoadBinFromResource(const std::wstring& name, unsigned char*& buf, size_t& size);
void LoadTextureFromResource(const std::wstring& name, sf::Texture& texture);
