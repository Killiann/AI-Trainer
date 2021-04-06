#include "Button.h"

Button::Button(sf::Vector2f btnPosition, std::string btnText, sf::Font* btnFont) : font(btnFont) {
		
	background.setPosition(btnPosition);
	background.setSize(sf::Vector2f((float)width, (float)height));
	background.setFillColor(color);
	background.setOutlineThickness(1.5f);
	background.setOutlineColor(sf::Color(250,95,87));

	text.setFont(*font);
	text.setString(btnText);
	text.setScale(sf::Vector2f(0.5, 0.5));
	text.setFillColor(textColor);
	text.setOutlineThickness(0);
	text.setLetterSpacing(1.f);
	text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
	text.setPosition(btnPosition.x + (width / 2.f), btnPosition.y + (height / 2.f) - 4);

}

void Button::Draw(sf::RenderTarget& window) {
	window.draw(background);
	window.draw(text);
}