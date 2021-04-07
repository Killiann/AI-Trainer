#pragma once
#include <SFML/Graphics.hpp>

class Dropdown
{
	bool isOpen;
	int selectedIndex = -1;

	sf::RectangleShape background;
	std::string ddName;

	sf::Font* font = nullptr;
	sf::Text title;	

	std::vector<sf::RectangleShape> items;
	std::vector<sf::Text> itemNames;

	sf::Color bgColor = sf::Color(40, 40, 40);
	sf::Color hoverColor = sf::Color(80, 80, 80);
	sf::Color textColor = sf::Color::White;

public:
	Dropdown(sf::Vector2f ddPosition, std::string ddTitle, sf::Font* ddFont);
	
	void AddItem(std::string itemName);
	inline void Select(int itemID) { selectedIndex = itemID; }

	void Draw(sf::RenderTarget& window);
};

