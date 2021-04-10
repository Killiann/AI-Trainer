#pragma once
#include "UIElement.h"

class Label : public UIElement {	

public:
	Label(){}
	Label(sf::Vector2f lblPosition, sf::Vector2f size, ResourceManager* resource, std::string message, float fontScale);
	
	inline void UpdateText(std::string newText) { text.setString(newText); }	
	inline void CenterText() { 
		text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2); 
		text.setPosition(sf::Vector2f(position.x + (size.x / 2), position.y + (size.y / 2.f) - 4));
	}
};

