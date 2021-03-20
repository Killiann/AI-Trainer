#pragma once
#include <SFML/Graphics.hpp>
#include <linearAlgebra.h>

class Network {
	//nodes
	lin::Matrix nodes_i; //input nodes
	lin::Matrix nodes_o; //output nodes
	std::vector<lin::Matrix> nodes_hl; //hidden layer nodes

	//weights
	lin::Matrix weights_ih; //weights input - hidden
	lin::Matrix weights_ho; //weights output - hidden
	std::vector<lin::Matrix> weights_hl; //weights in hidden layer
	
	//biases
	lin::Matrix biases_o;
	std::vector<lin::Matrix> biases_hl;

	//Rendering
	sf::FloatRect dimensions;
	std::vector<sf::CircleShape> inputNodes;
	std::vector<std::vector<sf::CircleShape>> hiddenNodes;
	std::vector<sf::CircleShape> outputNodes;

	std::vector<sf::VertexArray> connectionLines;

	void SetupRendering();
	void UpdateRender();
	sf::VertexArray CreateLine(sf::Vector2f p1, sf::Vector2f p2, float weight);

public:
	Network(int inputLayer, std::vector<int> hiddenLayers, int outputLayer, sf::FloatRect d);
	std::vector<float> FeedForward(std::vector<float> inputs);
	void Draw(sf::RenderTarget& window);
};