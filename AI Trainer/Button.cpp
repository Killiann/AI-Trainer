#include "Button.h"

Button::Button(sf::Vector2f btnPosition, std::string btnText, sf::Font* btnFont, void (f)()) : font(btnFont) {
		
	background.setPosition(btnPosition);
	background.setSize(sf::Vector2f((float)width, (float)height));
	background.setFillColor(color);
	background.setOutlineThickness(1.5f);
	background.setOutlineColor(accent);

	text.setFont(*font);
	text.setString(btnText);
	text.setScale(sf::Vector2f(0.5, 0.5));
	text.setFillColor(textColor);
	text.setOutlineThickness(0);
	text.setLetterSpacing(1.f);
	text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
	text.setPosition(btnPosition.x + (width / 2.f), btnPosition.y + (height / 2.f) - 4);
	
	if (!handCursor.loadFromSystem(sf::Cursor::Hand))
		std::cout << "Could not load hand cursor.";
	
	if (!arrowCursor.loadFromSystem(sf::Cursor::Arrow))
		std::cout << "Could not load arrow cursor.";

	clickFunc = f;
}

void Button::Update(sf::RenderWindow& window, sf::Event& event) {
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
				//run clicked function
				clickFunc();
			}
		}

		if (!bounds.contains((sf::Vector2f)sf::Mouse::getPosition(window))) {
			window.setMouseCursor(arrowCursor);
			if (isClicking) Clicking(false);
			Hovering(false);
		}
	}
}

void Button::Draw(sf::RenderTarget& window) {
	window.draw(background);
	window.draw(text);
}