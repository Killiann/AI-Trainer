#pragma once
#include "UIElement.h"

class DropdownItem : public UIElement {
	friend class Dropdown;

	bool selected = false;

	/// <summary>
	/// Toggle selected
	/// </summary>
	/// <param name="sel">bool, new selected value</param>
	inline void Selected(bool sel) {
		selected = sel;
		if (selected) {
			background.setFillColor(hoverColor);
		}
		else {
			background.setFillColor(color);
		}
	}

	/// <summary>
	/// Toggle hovering
	/// </summary>
	/// <param name="hovering">bool, new hover value</param>
	inline void Hovering(bool hovering) {
		isHovering = hovering;
		hovering ? background.setOutlineColor(accentColor) : background.setOutlineColor(color);
	}

	void OnClick();
public:
	DropdownItem() {}
	DropdownItem(sf::Vector2f ddPosition, sf::Vector2f ddSize, ResourceManager* resource, std::string ddText);

	void operator=(DropdownItem i);
};


class Dropdown : public UIElement
{
	bool isOpen = false;
	int selectedIndex = 0;

	sf::Sprite arrow;
	std::vector<DropdownItem> items;

	/// <summary>
	/// Change selected item ID
	/// </summary>
	/// <param name="itemID">int, new selected item</param>
	inline void Select(int itemID) { selectedIndex = itemID; }

	void OnClick();

public:
	Dropdown(){}
	Dropdown(sf::Vector2f ddPosition, sf::Vector2f ddSize, ResourceManager* resource, std::string ddText);
	
	void AddItem(std::string itemName);
	void Update(sf::RenderWindow& window, sf::Event& event);
	void Draw(sf::RenderTarget& window);

	/// <summary>
	/// Get current selected index
	/// </summary>
	/// <returns>int, selected index</returns>
	inline int GetSelected() { return selectedIndex; }	

	void operator=(Dropdown dd);
};

