#pragma once

#include <iostream>
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
	sf::Color hoverColor = sf::Color(80, 80, 80);
	sf::Color clickColor = sf::Color(100, 100, 100);
	sf::Color accent = sf::Color(250, 95, 87);

	bool isHovering = false;
	bool isClicking = false;

	sf::Cursor handCursor;
	sf::Cursor arrowCursor;

	inline void Hovering(bool hovering) {
		isHovering = hovering;
		hovering ? background.setFillColor(hoverColor) : background.setFillColor(color);
	}

	inline void Clicking(bool clicking) {
		isClicking = clicking;
		clicking ? background.setFillColor(clickColor) : background.setFillColor(hoverColor);
	}

	void (*clickFunc)() = nullptr;

public:
	Button(){}
	Button(sf::Vector2f btnPosition, std::string btnText, sf::Font* btnFont, void (f)());

	void Update(sf::RenderWindow& window, sf::Event& event);
	
	void Draw(sf::RenderTarget& window);
};

