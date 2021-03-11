#pragma once
#include <SFML/Graphics.hpp>
#include "Node.h"

class Layer
{
	int id;
	std::vector<Node> nodes;

	//SFML
	sf::FloatRect networkDimensions;
	float xPos;

public:
	Layer(int id, int nodeCount, sf::FloatRect netWorkDimensions, float xPos);
	void ActivateNodes();
	inline int GetNodeCount() { return nodes.size(); }

	//SFML
	void Draw(sf::RenderTarget& window);
};

