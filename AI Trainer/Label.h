#pragma once
#include "UIElement.h"

class Label : public UIElement {	

public:
	Label(){}
	Label(sf::Vector2f lblPosition, sf::Vector2f size, ResourceManager* resource, std::string message, float fontScale);
	
	/// <summary>
	/// Update label text value
	/// </summary>
	/// <param name="newText">std::string, new text value</param>
	inline void UpdateText(std::string newText) { text.setString(newText); }		
};

