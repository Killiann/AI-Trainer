#include "Network.h"
Network::Network(sf::FloatRect dims, std::vector<int> layerData) : dimensions(dims){
	layers.reserve(layerData.size());
	for (int i = 0; i < layerData.size(); ++i) {
		//create new layer
		Layer layer = Layer(i, layerData[i], dimensions, dimensions.left + (dimensions.width / layerData.size()) * i +
			((dimensions.width / layerData.size()) / 2));
		layers.push_back(layer);
	}
	//populate nodes with connections
	for (int i = 1; i < layers.size(); ++i) {
		layers[i].InitConnections(layers[i - 1]);
	}
	inputLayer = layers[0];
	outputLayer = layers[layers.size() - 1];
}

Network::Network(sf::FloatRect dims, std::vector<int> layerData, std::vector<float> weights) : dimensions(dims) {
}

std::vector<float> Network::CalculateOutput(std::vector<float> inputs) {
	std::vector<Node> inputNodes = inputLayer.GetNodes();
	for (int i = 0; i < inputNodes.size(); ++i) {
		inputNodes[i].SetValue(inputs[i]);
	}

	for (int i = 1; i < layers.size(); ++i) {
  		layers[i].ActivateNodes();
	}

	return outputLayer.GetNodeValues();
}

//SFML
void Network::Draw(sf::RenderTarget &window) {
	for (int i=layers.size()-1; i >= 0; --i)
		layers[i].Draw(window);
}