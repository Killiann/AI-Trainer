#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "ResourceManager.h"

class UIElement {

protected:
	ResourceManager* resourceManager = nullptr; 

	//data
	sf::Vector2f position;
	sf::Vector2f size;
	sf::Font* font = nullptr;
	sf::RectangleShape background;
	sf::Text text;

	float outlineThickness = 1.f;

	//colors
	sf::Color color;
	sf::Color textColor;
	sf::Color hoverColor;
	sf::Color clickColor;
	sf::Color accentColor;

	bool isHovering = false;
	bool isClicking = false;
	bool hoverable = true;

	//cursors
	sf::Cursor* handCursor = nullptr;
	sf::Cursor* textCursor = nullptr;
	sf::Cursor* arrowCursor = nullptr;
	
	//helpers
	inline virtual void Hovering(bool hovering) {
		isHovering = hovering;
		hovering ? background.setFillColor(hoverColor) : background.setFillColor(color);
	}
	inline virtual void Clicking(bool clicking) {
		isClicking = clicking;
		clicking ? background.setFillColor(clickColor) : background.setFillColor(hoverColor);
	}
	virtual void OnClick() {};

public:
	UIElement() {}; //default
	UIElement(sf::Vector2f pos, sf::Vector2f size, ResourceManager* resource);

	inline void SetPosition(sf::Vector2f p) { position = p; }
	inline void SetSize(sf::Vector2f s) { size = s; }
	inline void SetHoverColor(sf::Color c) { hoverColor = c; }
	inline void SetClickColor(sf::Color c) { clickColor = c; }
	inline void SetOutlineThickness(float t) { 
		outlineThickness = t; 
		background.setOutlineThickness(t);
	}
	inline void SetColor(sf::Color c) {
		color = c;
		background.setFillColor(color);
	}
	inline void SetAccentColor(sf::Color c) { 
		accentColor = c;
		background.setOutlineColor(accentColor);
	}
	inline void SetTextColor(sf::Color c) {
		textColor = c;
		text.setFillColor(textColor);
	}
	inline void SetTextString(std::string s) {
		text.setString(s);
		text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
	}	

	inline bool IsHovering() { return isHovering; }

	virtual sf::Vector2f GetSize() { return size; }
	virtual sf::Vector2f GetPosition() { return position; }

	virtual void Update(sf::RenderWindow& window, sf::Event& event);
	virtual void Draw(sf::RenderTarget& window);

	virtual void operator=(UIElement b);
};

