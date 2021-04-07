#include "Dropdown.h"

Dropdown::Dropdown(sf::Vector2f ddPosition, std::string ddText, sf::Font* ddFont, sf::Texture* ddArrow) : ddName(ddText), font(ddFont){
	
	background.setPosition(ddPosition);
	background.setSize(sf::Vector2f((float)width, (float)height));
	background.setFillColor(bgColor);
	background.setOutlineThickness(1.f);
	background.setOutlineColor(accent);

	arrow.setTexture(*ddArrow);
	arrow.setScale(0.3, 0.3);
	arrow.setOrigin(23, 12);
	arrow.setPosition(ddPosition.x + 120, ddPosition.y + 12);

	title.setFont(*font);
	title.setString(ddText);
	title.setScale(sf::Vector2f(0.5, 0.5));
	title.setFillColor(textColor);
	title.setOutlineThickness(0);
	title.setLetterSpacing(1.f);
	title.setOrigin(title.getLocalBounds().width / 2, title.getLocalBounds().height / 2);
	title.setPosition(ddPosition.x + (width / 2.f) - 10, ddPosition.y + (height / 2.f) - 4);

	if (!handCursor.loadFromSystem(sf::Cursor::Hand))
		std::cout << "Could not load hand cursor.";

	if (!arrowCursor.loadFromSystem(sf::Cursor::Arrow))
		std::cout << "Could not load arrow cursor.";

}

void Dropdown::AddItem(std::string itemName) {
	sf::Vector2f position(background.getPosition().x, (background.getPosition().y + ((items.size() + 1) * (height + 2))));
	ddItem newItem(itemName, position, font);
	items.push_back(newItem);
}

void Dropdown::Update(sf::RenderWindow& window, sf::Event& event) {
	sf::FloatRect bounds = background.getGlobalBounds();

	if (!isHovering) {
		if (bounds.contains((sf::Vector2f)sf::Mouse::getPosition(window))) {
			window.setMouseCursor(handCursor);
			Hovering(true);
		}
	}
	else {
		if (!isClicking && event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				Clicking(true);
			}
		}

		if (isClicking && event.type == sf::Event::MouseButtonReleased) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				Clicking(false);
				
				//toggle dropdown
				isOpen = isOpen ? false : true;
				arrow.rotate(180);
			}
		}

		if (!bounds.contains((sf::Vector2f)sf::Mouse::getPosition(window))) {
			window.setMouseCursor(arrowCursor);
			if (isClicking) Clicking(false);
			Hovering(false);
		}
	}

	//items
	if (isOpen) {
		for (unsigned int i = 0; i < items.size(); ++i) {
			sf::FloatRect itemBounds = items[i].background.getGlobalBounds();
			if (itemBounds.contains((sf::Vector2f)sf::Mouse::getPosition(window))) 
				window.setMouseCursor(handCursor);

			if (!items[i].isHovering) {
				if (itemBounds.contains((sf::Vector2f)sf::Mouse::getPosition(window))) {
					window.setMouseCursor(handCursor);
					items[i].Hovering(true);
				}
			}
			else {
				if (!items[i].isClicking && event.type == sf::Event::MouseButtonPressed) {
					if (event.mouseButton.button == sf::Mouse::Left) {
						items[i].Clicking(true);
					}
				}

				if (items[i].isClicking && event.type == sf::Event::MouseButtonReleased) {
					if (event.mouseButton.button == sf::Mouse::Left) {
						items[i].Clicking(false);

						//toggle dropdown
						isOpen = isOpen ? false : true;
						arrow.rotate(180);

						selectedIndex = i;
						for (auto& it : items)
							it.Selected(false);

						items[i].Selected(true);
						std::cout << "selected item: " << selectedIndex << std::endl;
					}
				}

				if (!itemBounds.contains((sf::Vector2f)sf::Mouse::getPosition(window))) {
					window.setMouseCursor(arrowCursor);
					if (items[i].isClicking) items[i].Clicking(false);
					items[i].Hovering(false);
				}
			}
		}
	}
}

void Dropdown::Draw(sf::RenderTarget& window) {
	window.draw(background);
	window.draw(title);
	window.draw(arrow);
	if (isOpen) {
		for (auto& i : items)
			i.Draw(window);
	}
}