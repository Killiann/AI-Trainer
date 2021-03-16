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
	Layer();
	void InitConnections(Layer &prevLayer);
	void ActivateNodes();

	inline int GetNodeCount() { return nodes.size(); }

	inline std::vector<Node>& GetNodes() { return nodes; }

	inline std::vector<float> GetNodeValues() {
		std::vector<float> retVec;
		for (auto &n : nodes) 
			retVec.push_back(n.GetValue());
		return retVec;
	}

	//SFML
	void Draw(sf::RenderTarget& window);
};

