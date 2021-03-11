#pragma once
#include <map>
#include <SFML/Graphics.hpp>

class Node
{
	int id;
	std::map<Node, float> connections;
	float value;

	//SFML
	sf::Vector2f position;
	sf::CircleShape shape;

public:
	Node(int ID, sf::Vector2f position);
	void ActivationFunction();
	inline float GetValue() { return value; }

	//SFML
	void Draw(sf::RenderTarget& window);
};

