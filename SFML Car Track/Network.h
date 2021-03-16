#pragma once
#include "Layer.h"
#include <SFML/Graphics.hpp>

class Network
{
	//Network
	std::vector<Layer> layers;
	Layer inputLayer, outputLayer;

	//SFML
	sf::FloatRect dimensions;
	bool hidden = true;

public:
	Network(sf::FloatRect dimensions, std::vector<int> layerData); //for generating a new network with random weights
	Network(sf::FloatRect dimensions, std::vector<int> layerData, std::vector<float> weights); // for generating a network with predefined weights

	std::vector<float> CalculateOutput(std::vector<float> inputs);

	//SFML
	inline void Hide() { hidden = true; }
	inline void Show() { hidden = false; }
	inline sf::FloatRect GetDimensions() { return dimensions; }
	void Draw(sf::RenderTarget& window);
};

