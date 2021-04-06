#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"

struct cMessage {

	cMessage(std::string m, std::string v, sf::Font* f) {
		msg = m;
		value = v;
		display.setFont(*f);
		display.setFillColor(sf::Color::White);
		display.setCharacterSize(12);
		UpdateDisplay();
	}

	void UpdateDisplay() {
		std::string temp = msg;
		if (value != "") temp += ": " + value;
		display.setString(temp);
	}

	std::string msg;
	std::string value;
	sf::Text display;
};

class ConsoleManager{

	sf::Font *font = nullptr; 
	bool display = true;
	bool isPressing = false;

	std::vector<cMessage> messages;
	sf::Vector2f position;
	sf::RectangleShape background;

public:
	ConsoleManager(){}
	ConsoleManager(sf::Font* f);	
	
	void AddMessage(std::string message, std::string value = "");
	void UpdateMessageValue(std::string key, std::string value);

	inline void Show() { display = true; }
	inline void Hide() { display = false; }
	inline bool IsDisplayed() { return display; }

	void Draw(sf::RenderTarget& window);
};

