#pragma once
#include <SFML/Graphics.hpp>

#include "ResourceManager.h"
#include "Button.h"
#include "Textbox.h"
#include "Dropdown.h"
#include "Label.h"
#include "UIElement.h"

class MainMenu
{
	sf::Vector2f size = sf::Vector2f(900.f, 500.f);
	sf::Vector2f position = sf::Vector2f(300.f, 200.f);
	float padding = 40.f;
	sf::RectangleShape background;
	ResourceManager* resourceManager;

	sf::Text title;
	sf::Text prompt;

	std::vector<Button> navigation;
	Dropdown dd;

public:
	MainMenu(ResourceManager* resource);
	void Update(sf::RenderWindow& window, sf::Event& event);
	void Draw(sf::RenderTarget& window);
};

