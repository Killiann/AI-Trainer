#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Textbox
{
	bool numeric;
	bool isFocused = false;
	bool isHovering = false;
	bool isClicking = false;

	int width = 138, height = 25;

	sf::RectangleShape background;
	sf::Text text;
	sf::Font* font = nullptr;

	sf::Color textColor = sf::Color(40, 40, 40);
	sf::Color hoverColor = sf::Color(230, 230, 230);
	sf::Color color = sf::Color::White;

	sf::Color unfocusedAccent = sf::Color(250, 95, 87);
	sf::Color focusedAccent = sf::Color::Blue;

	std::string value;

	sf::Cursor arrowCursor;
	sf::Cursor textCursor;

	float cTime = 500.f; //milliseconds
	bool showCursor = false;
	sf::Clock cursorTimer;

	inline void Hovering(bool hovering) {
		isHovering = hovering;
		//hovering ? background.se(hoverColor) : background.setFillColor(color);
	}

	inline void Focused(bool focused) {
		isFocused = focused;
		if (isFocused)cursorTimer.restart();
		isFocused ? background.setOutlineColor(focusedAccent) : background.setOutlineColor(unfocusedAccent);
	}

public:
	Textbox(){}
	Textbox(sf::Vector2f position, sf::Font* font, bool numeric = false);

	void Update(sf::RenderWindow& window, sf::Event& event);
	
	void Draw(sf::RenderTarget& window);
};

