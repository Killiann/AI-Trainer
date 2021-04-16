#include "Textbox.h"

/// <summary>
/// Initialise Textbox
/// </summary>
/// <param name="position">sf::Vector2f, textbox position</param>
/// <param name="size">sf::Vector2f, textbox size</param>
/// <param name="resource">ResourceManager* resource</param>
/// <param name="isNumeric">Bool, Is textbox input limited to numeric values</param>
/// <param name="maxVal">Int, Limit on numeric input</param>
Textbox::Textbox(sf::Vector2f position, sf::Vector2f size, ResourceManager* resource, bool isNumeric = false, int maxVal = 0) : numeric(isNumeric) , UIElement(position, size, resource){	
	max = maxVal;

	text.setOrigin(0, text.getLocalBounds().height / 2);
	text.setPosition(position.x + 5, position.y + (size.y / 2.f) - 9);

	SetColor(sf::Color::White);
	SetHoverColor(sf::Color::White);
	SetClickColor(sf::Color::White);
	SetAccentColor(sf::Color(60, 60, 60));
	SetTextColor(sf::Color(60, 60, 60));
}

/// <summary>
/// Initialise Textbox - toggle focus
/// </summary>
void Textbox::OnClick() {
	UIElement::OnClick();
	Focused(isFocused ? false : true);
}

/// <summary>
/// Update Textbox - handle user text input
/// </summary>
/// <param name="window">sf::Renderwindow reference</param>
/// <param name="event">sf::Event reference</param>
void Textbox::Update(sf::RenderWindow& window, sf::Event& event) {
	UIElement::Update(window, event);
	if (!hidden) {
		if (isFocused) {

			//unfocus if clicked outside of element
			if (event.type == sf::Event::MouseButtonPressed) {
				if (!background.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(window))) {
					if (event.mouseButton.button == sf::Mouse::Left) {
						Focused(false);
					}
				}
			}

			if (event.type == sf::Event::TextEntered) {
				if (event.text.unicode == 0x08) {
					if (value.size() > 0) {
						value = value.substr(0, value.size() - 1);//backspace
						text.setString(value);
					}
				}
				else if (text.getGlobalBounds().width < size.x - 15) {
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
}

/// <summary>
/// Draw Textbox
/// </summary>
/// <param name="window">sf::RenderTarget</param>
void Textbox::Draw(sf::RenderTarget& window) {
	if (!hidden) {
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
}

void Textbox::operator=(Textbox t) {
	UIElement::operator=(t);

	numeric = t.numeric;
	isFocused = t.isFocused;
	value = t.value;
	showCursor = t.showCursor;
}