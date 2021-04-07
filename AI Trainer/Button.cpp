#include "Button.h"

Button::Button():UIElement(){}

Button::Button(sf::Vector2f btnPosition, sf::Vector2f btnSize, ResourceManager* resource, std::string btnText, void (f)()) :
	UIElement(btnPosition, btnSize, resource) {
	clickFunc = f;

	//defaults
	SetColor(sf::Color(40, 40, 40)); 
	SetOutlineThickness(0.f);
	SetTextColor(sf::Color::White);
	SetHoverColor(sf::Color(10,10,10));
	SetClickColor(sf::Color(100, 100, 100));

	SetTextString(btnText);
}

void Button::OnClick() {
	clickFunc();
}

void Button::operator=(Button b) {
	UIElement::operator=(b);

	this->clickFunc = b.clickFunc;
}