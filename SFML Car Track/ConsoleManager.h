#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"

class ConsoleManager
{	
	sf::Font *font;
	bool display = true;
	bool isPressing = false;

	struct cMessage {
		void UpdateDisplay() {
			std::string temp = msg;
			if (value != "") temp += ": " + value;
			display.setString(temp);
		}
		cMessage(std::string m, std::string v, sf::Font *f) {
			msg = m;
			value = v;
			display.setFont(*f);
			display.setFillColor(sf::Color::White);
			display.setCharacterSize(12);
			UpdateDisplay();
		}				
		std::string msg;		
		std::string value;
		sf::Text display;		
	};
	std::vector<cMessage> messages;
	sf::Vector2f position;
	sf::RectangleShape background;
public:
	ConsoleManager(sf::Font* f);	
	void AddMessage(std::string message, std::string value = "");
	void UpdateMessageValue(std::string key, std::string value);
	void Update();
	void Draw(sf::RenderWindow& window);
};

