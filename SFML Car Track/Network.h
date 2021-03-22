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
	Network() {};
	Network(int inputLayer, std::vector<int> hiddenLayers, int outputLayer, sf::FloatRect d);
	std::vector<float> FeedForward(std::vector<float> inputs);

	inline std::vector<lin::Matrix> GetWeights() {
		std::vector<lin::Matrix> ret;
		ret.push_back(weights_ih);
		ret.push_back(weights_ho);
		for (int i = 0; i < weights_hl.size(); ++i)
			ret.push_back(weights_hl[i]);
		
		return ret;
	}

	inline std::vector<lin::Matrix> GetBiases() {
		std::vector<lin::Matrix> ret;
		ret.push_back(biases_o);
		for (int i = 0; i < biases_hl.size(); ++i)
			ret.push_back(biases_hl[i]);

		return ret;
	}

	void SetWeights(std::vector<lin::Matrix> newWeights);
	void SetBiases(std::vector<lin::Matrix> newBiases);

	void Draw(sf::RenderTarget& window);
};