#include "Layer.h"

Layer::Layer(int ID, int nodeCount, sf::FloatRect nd, float xp) : id(ID), networkDimensions(nd), xPos(xp) {	
	//temp 
	if (nodeCount <= 4) {
		networkDimensions.top += networkDimensions.height * 0.15;
		networkDimensions.height *= 0.7;
	}

	for (int i = 0; i < nodeCount; ++i) {
		//generate nodes in layer
		Node node(i, sf::Vector2f(xPos, networkDimensions.top + (networkDimensions.height / nodeCount) * i + (networkDimensions.height / nodeCount) / 2));
		nodes.push_back(node);
	}
}
Layer::Layer() {
	id = -1;
	xPos = 0;
}

void Layer::InitConnections(Layer &prevLayer) {
	std::vector<Node> prevLayerNodes = prevLayer.GetNodes();
	for (auto& n : nodes) {
		n.CreateConnections(prevLayerNodes);
	}
}

void Layer::ActivateNodes() {
 	for (int i = 0; i < nodes.size(); ++i) {
		nodes[i].ActivationFunction();
	}
}

//SFML
void Layer::Draw(sf::RenderTarget& window) {
	for (auto& n : nodes)
		n.DrawLines(window);
	for (auto& n : nodes)
		n.DrawNode(window);
}