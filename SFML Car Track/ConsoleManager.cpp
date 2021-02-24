#include "ConsoleManager.h"
#include <vector>
#include <string>
#include <iostream>
#include <Windows.h>

ConsoleManager::ConsoleManager(sf::Font* f) : font(f){
	background.setFillColor(sf::Color::Black);
	background.setSize(sf::Vector2f(200, 400));
	background.setPosition(position);
	background.setOutlineThickness(3);
	background.setOutlineColor(sf::Color::White);		
}

void ConsoleManager::AddMessage(std::string message, std::string value) {
	messages.emplace_back(message, value, font);
}

void ConsoleManager::UpdateMessageValue(std::string key, std::string value) {
	bool found = false;
	for(cMessage &m : messages){
		if (m.msg == key) {
			m.value = value;
			m.UpdateDisplay();
			found = true;
		}
	}
	if (!found) 
		std::cout << "Console message with key: " << key << " not found." << std::endl;	
}

void ConsoleManager::Update() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C)) {
		if (!isPressing) {
			display = !display;
			isPressing = true;
		}
	}
	if (isPressing)
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C))
			isPressing = false;
}

void ConsoleManager::Draw(sf::RenderWindow& window) {    
	if (display) {
		window.draw(background);

		int yOffset = 0;
		for (cMessage& m : messages) {
			m.display.setPosition(sf::Vector2f(position.x + 5, position.y + 5 + yOffset));
			yOffset += m.display.getCharacterSize() + 2;
			window.draw(m.display);
		}
	}
}