#pragma once
#include "UIElement.h"

class Textbox : public UIElement 
{
	bool numeric = false;
	bool isFocused = false;
		
	std::string value;

	//blinking cursor
	float cTime = 500.f; //milliseconds
	bool showCursor = false;
	sf::Clock cursorTimer;	

	void OnClick();

	inline void Focused(bool focused) {
		isFocused = focused;
		if (isFocused)cursorTimer.restart();
		isFocused ? SetOutlineThickness(2.f) : SetOutlineThickness(1.f);
	}

public:
	Textbox(){}
	Textbox(sf::Vector2f position, sf::Vector2f size, ResourceManager* resource, bool numeric);

	void Update(sf::RenderWindow& window, sf::Event& event);	
	void Draw(sf::RenderTarget& window);

	void operator=(Textbox t);
};

