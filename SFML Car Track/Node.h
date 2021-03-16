#pragma once
#include <map>
#include <SFML/Graphics.hpp>
#include <random>
#include <linearAlgebra.h>

class Node
{
	int id;
	//std::vector<std::pair<Node, float>> connections;
	float value;

	//rng
	std::mt19937 gen;
	std::uniform_int_distribution<> distr;

	//SFML
	sf::Vector2f position;
	sf::CircleShape shape;
	std::vector<sf::VertexArray> connectionLines;
	
	sf::VertexArray CreateLine(sf::Vector2f p1, sf::Vector2f p2, float weight);
	sf::Vector2f Normalise(const sf::Vector2f& source);

public:
	Node(int ID, sf::Vector2f position);
	void CreateConnections(std::vector<Node>& prevLayerNodes);
	void ActivationFunction();
	inline float GetValue() { return value; }
	inline void SetValue(float newVal) { value = newVal; }

	//SFML
	void DrawLines(sf::RenderTarget& window);
	void DrawNode(sf::RenderTarget& window);
};

