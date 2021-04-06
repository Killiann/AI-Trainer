#pragma once
#include <SFML/Graphics.hpp>

class Label
{
	sf::Color color = sf::Color(30, 30, 30);
	sf::Font* font = nullptr;
	sf::Text text;

public:
	Label(){}
	Label(sf::Vector2f lblPosition, std::string lblText, sf::Font* lblFont);

	inline std::string GetText() { return text.getString(); }
	inline void UpdateText(std::string newText) { text.setString(newText); }

	void Draw(sf::RenderTarget& window);
};

