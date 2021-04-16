#include "Button.h"

Button::Button() : UIElement(){}
/// <summary>
/// Initialise Button
/// </summary>
/// <param name="btnPosition">sf::Vector2f, Button Position</param>
/// <param name="btnSize">sf::Vector2f, Button Size</param>
/// <param name="resource">ReourceManager* resource</param>
/// <param name="btnText">std::string, Button Text</param>
/// <param name="f">On Click Function (void (f)(void*))</param>
/// <param name="ctx">Context passed to Click Function (pointer)</param>
Button::Button(sf::Vector2f btnPosition, sf::Vector2f btnSize, ResourceManager* resource, std::string btnText, void (f)(void*) ,void* ctx)
	: UIElement(btnPosition, btnSize, resource), context(ctx), func(f){	

	//defaults
	SetColor(sf::Color(40, 40, 40)); 
	SetOutlineThickness(0.f);
	SetTextColor(sf::Color::White);
	SetHoverColor(sf::Color(10,10,10));
	SetClickColor(sf::Color(100, 100, 100));

	SetTextString(btnText);
}

/// <summary>
/// Call predefined On Click functiono
/// </summary>
void Button::OnClick() {
	UIElement::OnClick();
	func(context);
}

void Button::operator=(Button b) {
	UIElement::operator=(b);

	this->func = b.func;
	this->context = b.context;
}