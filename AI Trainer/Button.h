#pragma once
#include <SFML/Graphics.hpp>

class Button
{
	const unsigned int width = 100;
	const unsigned int height = 25;

	sf::RectangleShape background;
	
	sf::Font* font = nullptr;
	sf::Text text;

	sf::Color color = sf::Color(40,40,40); 
	sf::Color textColor = sf::Color::White;
	sf::Color hoverColor = sf::Color(240, 240, 240);

	bool isHovering = false;

public:
	Button(){}
	Button(sf::Vector2f btnPosition, std::string btnText, sf::Font* btnFont);

	inline void Hovering(bool hovering) { 
		isHovering = hovering; 
		hovering ? background.setFillColor(hoverColor) : background.setFillColor(color);
	}

	void Draw(sf::RenderTarget& window);
};

