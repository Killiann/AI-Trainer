#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

struct ddItem {

	ddItem(std::string itemName, sf::Vector2f position, sf::Font* font) {
		background.setPosition(position);
		background.setSize(sf::Vector2f((float)width, (float)height));
		background.setFillColor(itemBgColor);
		background.setOutlineThickness(1);
		background.setOutlineColor(itemBgColor);

		name.setFont(*font);
		name.setString(itemName);
		name.setScale(sf::Vector2f(0.5, 0.5));
		name.setFillColor(textColor);
		name.setOutlineThickness(0);
		name.setLetterSpacing(1.f);
		name.setOrigin(0, name.getLocalBounds().height / 2);
		name.setPosition(background.getPosition().x + 6, position.y + (height / 2.f) - 4);
	}

	const unsigned int width = 138;
	const unsigned int height = 25;

	bool isHovering = false;
	bool isClicking = false;
	bool selected = false;

	sf::Font* font = nullptr;

	sf::RectangleShape background;
	sf::Text name;
	bool itemHovering;
	bool itemClicking;


	sf::Color itemBgColor = sf::Color(60, 60, 60);
	sf::Color hoverColor = sf::Color(80, 80, 80);
	sf::Color clickColor = sf::Color(100, 100, 100);
	sf::Color textColor = sf::Color::White;
	sf::Color accent = sf::Color(250, 95, 87);

	inline void Hovering(bool hovering) {
		isHovering = hovering;		
		hovering ? background.setOutlineColor(accent): background.setOutlineColor(itemBgColor);
	}

	inline void Clicking(bool clicking) {
		isClicking = clicking;
		clicking ? background.setFillColor(clickColor) : background.setFillColor(hoverColor);
	}

	inline void Draw(sf::RenderTarget& window) {
		window.draw(background);
		window.draw(name);
	}

	inline void Selected(bool sel) {
		selected = sel;
		selected ? background.setFillColor(hoverColor) : background.setFillColor(itemBgColor);
	}
};

class Dropdown
{
	const unsigned int width = 138;
	const unsigned int height = 25;

	bool isHovering = false;
	bool isClicking = false;
	bool isOpen = false;
	int selectedIndex = -1;

	sf::RectangleShape background;
	std::string ddName;

	sf::Sprite arrow;
	sf::Font* font = nullptr;
	sf::Text title;	

	std::vector<ddItem> items;

	sf::Color bgColor = sf::Color(40, 40, 40);

	sf::Color hoverColor = sf::Color(80, 80, 80);
	sf::Color clickColor = sf::Color(100, 100, 100);
	sf::Color accent = sf::Color(250, 95, 87);
	sf::Color textColor = sf::Color::White;

	sf::Cursor handCursor;
	sf::Cursor arrowCursor;

	inline void Select(int itemID) { selectedIndex = itemID; }

	inline void Hovering(bool hovering) {
		isHovering = hovering;
		hovering ? background.setFillColor(hoverColor) : background.setFillColor(bgColor);
	}

	inline void Clicking(bool clicking) {
		isClicking = clicking;
		clicking ? background.setFillColor(clickColor) : background.setFillColor(hoverColor);
	}

public:
	Dropdown(){}
	Dropdown(sf::Vector2f ddPosition, std::string ddTitle, sf::Font* ddFont, sf::Texture* ddArrow);
	
	void AddItem(std::string itemName);
	void Update(sf::RenderWindow& window, sf::Event& event);

	inline int GetSelected() { return selectedIndex; }

	void Draw(sf::RenderTarget& window);
};

