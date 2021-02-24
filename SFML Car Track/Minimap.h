#pragma once
#include "Car.h"
#include "Track.h"

class Minimap
{
	Track* track;
	std::vector<Car*> cars;

	std::vector<sf::CircleShape> mapMarkers;
	sf::View miniMap;
	sf::RectangleShape bg;

	sf::RectangleShape boundingRectangle;
public:
	Minimap(std::vector<Car*> c, Track* t);
	void Draw(sf::RenderWindow& window);
};

