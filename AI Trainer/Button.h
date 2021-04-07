#pragma once

#include <SFML/Graphics.hpp>
#include "UIElement.h"

class Button : public UIElement
{
	void (*clickFunc)() = nullptr;

	void OnClick();
public:
	Button(); //default
	Button(sf::Vector2f btnPosition, sf::Vector2f btnSize,  ResourceManager* resource, std::string btnText, void (f)());

	void operator=(Button b);
};

