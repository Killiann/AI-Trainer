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
	bool hidden = false;

	bool hoverable = true;
	bool clickable = true;
	bool onScreen = false;

	//cursors
	sf::Cursor* handCursor = nullptr;
	sf::Cursor* textCursor = nullptr;
	sf::Cursor* arrowCursor = nullptr;
	
	unsigned int cursorId = 0;

	//helpers
	
	/// <summary>
	/// Toggle hover
	/// </summary>
	/// <param name="hovering">bool, new hover value</param>
	inline virtual void Hovering(bool hovering) {
		isHovering = hovering;
		hovering ? background.setFillColor(hoverColor) : background.setFillColor(color);
	}
	/// <summary>
	/// Toggle click
	/// </summary>
	/// <param name="clicking">bool, new click value</param>
	inline virtual void Clicking(bool clicking) {
		isClicking = clicking;
		clicking ? background.setFillColor(clickColor) : background.setFillColor(hoverColor);
	}
	virtual void OnClick() {};

public:
	UIElement() {}; //default
	UIElement(sf::Vector2f pos, sf::Vector2f size, ResourceManager* resource);
	virtual void Update(sf::RenderWindow& window, sf::Event& event);
	virtual void Draw(sf::RenderTarget& window);

	//set styling
	
	/// <summary>
	/// Set position
	/// </summary>
	/// <param name="p">sf::Vector2f, new position</param>
	inline void SetPosition(sf::Vector2f p) { position = p; }
	/// <summary>
	/// Set size
	/// </summary>
	/// <param name="s">sf::Vector2f, new size</param>
	inline void SetSize(sf::Vector2f s) { size = s; }
	/// <summary>
	/// Set hover color
	/// </summary>
	/// <param name="c">sf::Color new color</param>
	inline void SetHoverColor(sf::Color c) { hoverColor = c; }
	/// <summary>
	/// Set click color
	/// </summary>
	/// <param name="c">sf::Color new color</param>
	inline void SetClickColor(sf::Color c) { clickColor = c; }
	/// <summary>
	/// Set font scale - default 0.5
	/// </summary>
	/// <param name="scale">float, new scale</param>
	inline void SetFontScale(float scale) { text.setScale(scale, scale); }
	/// <summary>
	/// Set text of element
	/// </summary>
	/// <param name="newString">std::string new text value</param>
	inline void SetText(std::string newString) { text.setString(newString); }
	/// <summary>
	/// Set border thickness
	/// </summary>
	/// <param name="t">float, t new thickness</param>
	inline void SetOutlineThickness(float t) { 
		outlineThickness = t; 
		background.setOutlineThickness(t);
	}
	/// <summary>
	/// Set background color
	/// </summary>
	/// <param name="c">sf::Color, new color</param>
	inline void SetColor(sf::Color c) {
		color = c;
		background.setFillColor(color);
	}
	/// <summary>
	/// Set accent color
	/// </summary>
	/// <param name="c">sf::Color, new accent color</param>
	inline void SetAccentColor(sf::Color c) { 
		accentColor = c;
		background.setOutlineColor(accentColor);
	}
	/// <summary>
	/// Set text color
	/// </summary>
	/// <param name="c">sf::Color new color</param>
	inline void SetTextColor(sf::Color c) {
		textColor = c;
		text.setFillColor(textColor);
	}
	/// <summary>
	/// Set text string and center
	/// </summary>
	/// <param name="s">std::string s</param>
	inline void SetTextString(std::string s) {
		text.setString(s);
		text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
	}	
	/// <summary>
	/// Set font of element
	/// </summary>
	/// <param name="newFont">sf::Font*, new font</param>
	inline void SetFont(sf::Font* newFont) {
		if (newFont != nullptr) {
			font = newFont;
			text.setFont(*font);
		}
	}
	/// <summary>
	/// Center text
	/// </summary>
	inline void CenterText() {
		text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
		text.setPosition(sf::Vector2f(position.x + (size.x / 2), position.y + (size.y / 2.f) - 4));
	}
	/// <summary>
	/// Offset text within element by pixels
	/// </summary>
	/// <param name="x">float, x offset</param>
	/// <param name="y">float ,y offset</param>
	virtual void OffsetText(float x, float y) { text.setPosition(text.getPosition().x + x, text.getPosition().y + y); }

	//get 
	
	/// <summary>
	/// Get text value
	/// </summary>
	/// <returns>std::string, text value</returns>
	inline std::string GetText() { return text.getString(); }
	/// <summary>
	/// Get size of element
	/// </summary>
	/// <returns>sf::Vector2f size</returns>
	virtual sf::Vector2f GetSize() { return size; }
	/// <summary>
	/// Get position of element
	/// </summary>
	/// <returns>sf::Vector2f position of element</returns>
	virtual sf::Vector2f GetPosition() { return position; }

	//set functionality
	
	/// <summary>
	/// Check if currently hovering
	/// </summary>
	/// <returns>bool, is hovering</returns>
	inline virtual bool IsHovering() { return isHovering; }
	/// <summary>
	/// Enable click functionality
	/// </summary>
	inline void EnableClick() { clickable = true; }
	/// <summary>
	/// Disable click functionality
	/// </summary>
	inline void DisableClick() { clickable = false; }
	/// <summary>
	/// Enable hover functionality
	/// </summary>
	inline void EnableHover() { hoverable = true; }
	/// <summary>
	/// Disable hover functionality
	/// </summary>
	inline void DisableHover() { hoverable = false; }
	
	/// <summary>
	/// Show Element if hidden
	/// </summary>
	inline void Show() { hidden = false; }
	/// <summary>
	/// Hide Element if shown
	/// </summary>
	inline void Hide() { hidden = true; }	
			
	virtual void operator=(UIElement b);
};

