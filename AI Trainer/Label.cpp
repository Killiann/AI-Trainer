#include "Label.h"

/// <summary>
/// Initialise Label
/// </summary>
/// <param name="lblPosition">sf::Vector2f, label position</param>
/// <param name="size">sf::Vector2f, label size</param>
/// <param name="resource">ResourceManager* resource</param>
/// <param name="message">std::String, label text</param>
/// <param name="fontScale">Float, font scale (default 0.5) </param>
Label::Label(sf::Vector2f lblPosition, sf::Vector2f size, ResourceManager* resource, std::string message, float fontScale = 0.5) : UIElement(lblPosition, size, resource){

	DisableClick();
	DisableHover();
	SetColor(sf::Color::White);
	SetTextColor(sf::Color(40, 40, 40));
	SetHoverColor(sf::Color::White);
	SetAccentColor(sf::Color::White);
	text.setScale(fontScale, fontScale);
	text.setString(message);
	text.setOrigin(0, text.getOrigin().y);
	text.setPosition(lblPosition.x, position.y + 4);

}

