#include "Textbox.h"

Textbox::Textbox(sf::Vector2f position, sf::Font* tbFont, bool isNumeric) : font(tbFont), numeric(isNumeric){

	background.setSize(sf::Vector2f((float)width, (float)height));
	background.setPosition(position);
	background.setFillColor(color);
	background.setOutlineThickness(1.f);
	background.setOutlineColor(unfocusedAccent);

	text.setFont(*font);
	text.setString("test");
	text.setScale(sf::Vector2f(0.5, 0.5));
	text.setFillColor(textColor);
	text.setOutlineThickness(0);
	text.setLetterSpacing(1.f);
	text.setOrigin(0, text.getLocalBounds().height / 2);
	text.setPosition(position.x + 5, position.y + (height / 2.f) - 4);

	if (!textCursor.loadFromSystem(sf::Cursor::Text))
		std::cout << "Could not load hand cursor.";

	if (!arrowCursor.loadFromSystem(sf::Cursor::Arrow))
		std::cout << "Could not load arrow cursor.";

}

void Textbox::Update(sf::RenderWindow& window, sf::Event& event) {
	sf::FloatRect bounds = background.getGlobalBounds();
	
	if (!isHovering) {
		if (bounds.contains((sf::Vector2f)sf::Mouse::getPosition(window))) {
			window.setMouseCursor(textCursor);
			Hovering(true);
		}
		else if (event.type == sf::Event::MouseButtonPressed) 
			if(event.mouseButton.button == sf::Mouse::Left)
				Focused(false);
		
	}
	else {
		if (!isFocused && event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				Focused(true);
			}
		}

		if (!bounds.contains((sf::Vector2f)sf::Mouse::getPosition(window))) {
			window.setMouseCursor(arrowCursor);
			Hovering(false);
		}
	}

	if (isFocused) {
		if (event.type == sf::Event::TextEntered) {
			if (event.text.unicode == 0x08) {
				if (value.size() > 0) {
					value = value.substr(0, value.size() - 1);//backspace
					text.setString(value);
				}
			}
			else if(text.getGlobalBounds().width < width - 15){
				if (!numeric) {
					if (event.text.unicode < 0x80) {
						value += (char)event.text.unicode;
						text.setString(value);
					}
				}
				else {
					if (event.text.unicode >= 0x30 && event.text.unicode <= 0x39) {
						value += (char)event.text.unicode;
						text.setString(value);
					}
				}
			}
		}
	}
}

void Textbox::Draw(sf::RenderTarget& window) {
	window.draw(background);

	//blinking cursor
	if (isFocused) {
		if (cursorTimer.getElapsedTime().asMilliseconds() >= cTime) {
			cursorTimer.restart();
			showCursor = !showCursor;
		}
		if (showCursor)text.setString(text.getString() + "|");
	}
	window.draw(text);
	if (isFocused && showCursor)text.setString(text.getString().substring(0, text.getString().getSize() - 1));
}