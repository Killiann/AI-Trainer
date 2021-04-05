#pragma once
#include "Car.h"
#include "Track.h"

class Minimap
{
	Track* track = nullptr;
	std::vector<Car>* cars = nullptr; 
	ConsoleManager* consoleManager = nullptr;
	
	std::vector<sf::CircleShape> mapMarkers;
	sf::View miniMap;
	sf::RectangleShape bg;
	sf::RectangleShape boundingRectangle;
	
public:
	Minimap(){}
	Minimap(std::vector<Car>* c, Track* t, ConsoleManager *cm);
	void Draw(sf::RenderWindow& window);		
};

