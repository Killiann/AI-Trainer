#include "Label.h"

Label::Label(sf::Vector2f lblPosition, std::string lblText, sf::Font* lblFont): font(lblFont){

	text.setFont(*font);
	text.setString(lblText);
	text.setScale(sf::Vector2f(0.5, 0.5));
	text.setFillColor(color);
	text.setOutlineThickness(0);
	text.setLetterSpacing(1.f);	
	text.setPosition(lblPosition);

}

void Label::Draw(sf::RenderTarget& window) {
	window.draw(text);
}