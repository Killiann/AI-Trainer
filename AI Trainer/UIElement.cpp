#include "UIElement.h"

UIElement::UIElement(sf::Vector2f pos, sf::Vector2f s, ResourceManager* resource) : resourceManager(resource), position(pos), size(s){
	font = resourceManager->GetRobotoRegular();

	background.setPosition(position);
	background.setSize(sf::Vector2f(size.x, size.y));
	background.setFillColor(color);
	background.setOutlineThickness(outlineThickness);
	background.setOutlineColor(accentColor);

	text.setFont(*font);	
	text.setScale(sf::Vector2f(0.5, 0.5));
	text.setFillColor(textColor);
	text.setOutlineThickness(0);
	text.setLetterSpacing(1.f);
	text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
	text.setPosition(position.x + (size.x / 2.f), position.y + (size.y / 2.f) - 4);

	handCursor = resourceManager->GetHandCursor();
	textCursor = resourceManager->GetTextCursor();
	arrowCursor = resourceManager->GetArrowCursor();
}

void UIElement::Update(sf::RenderWindow& window, sf::Event& event) {
	sf::FloatRect bounds = background.getGlobalBounds();
	//on mouse enter bounds
	if (!isHovering) {
		if (bounds.contains((sf::Vector2f)sf::Mouse::getPosition(window))) {
			window.setMouseCursor(*handCursor);
			Hovering(true);
		}
	}
	else {
		//on mouse down
		if (!isClicking && event.type == sf::Event::MouseButtonPressed) 
			if (event.mouseButton.button == sf::Mouse::Left) 
				Clicking(true);					

		//on mouse up
		if (isClicking && event.type == sf::Event::MouseButtonReleased) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				Clicking(false);
				OnClick();
			}
		}

		//on mouse exit bounds
		if (!bounds.contains((sf::Vector2f)sf::Mouse::getPosition(window))) {
			window.setMouseCursor(*arrowCursor);
			if (isClicking) Clicking(false);
			Hovering(false);
		}
	}
}

void UIElement::Draw(sf::RenderTarget& window) {
	window.draw(background);
 	window.draw(text);
}

void UIElement::operator=(UIElement b) {
	this->resourceManager = b.resourceManager;
	this->position = b.position;
	this->size = b.size;
	this->font = b.font;
	this->background = b.background;
	this->text = b.text;
	this->outlineThickness = b.outlineThickness;
	this->color = b.color;
	this->textColor = b.textColor;
	this->hoverColor = b.hoverColor;
	this->clickColor = b.clickColor;
	this->accentColor = b.accentColor;
	this->isHovering = b.isHovering;
	this->isClicking = b.isClicking;			

	handCursor = b.resourceManager->GetHandCursor();
	textCursor = b.resourceManager->GetTextCursor();
	arrowCursor = b.resourceManager->GetArrowCursor();
}