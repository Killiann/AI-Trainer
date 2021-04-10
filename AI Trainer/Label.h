#pragma once
//#include <SFML/Graphics.hpp>
#include "UIElement.h"

class Label : public UIElement
{	

public:
	Label(){}
	Label(sf::Vector2f lblPosition, sf::Vector2f size, ResourceManager* resource, std::string message, float fontScale);
	
	inline void UpdateText(std::string newText) { text.setString(newText); }

	void Draw(sf::RenderTarget& window);
};

