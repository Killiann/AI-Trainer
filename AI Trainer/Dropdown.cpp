#include "Dropdown.h"

Dropdown::Dropdown(sf::Vector2f ddPosition, std::string ddText, sf::Font* ddFont) : ddName(ddText), font(ddFont){
	


}

void Dropdown::AddItem(std::string itemName) {

}

void Dropdown::Draw(sf::RenderTarget& window) {
	window.draw(background);
	for (unsigned int i = 0; i < items.size(); ++i) {
		window.draw(items[i]);
		window.draw(itemNames[i]);
	}
}