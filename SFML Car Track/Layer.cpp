#include "Layer.h"

Layer::Layer(int ID, int nodeCount, sf::FloatRect nd, float xp) : id(ID), networkDimensions(nd), xPos(xp) {	
	//temp 
	if (nodeCount <= 4) {
		networkDimensions.top += networkDimensions.height * 0.15;
		networkDimensions.height *= 0.7;
	}

	for (int i = 0; i < nodeCount; ++i) {
		//generate nodes in layer
		Node node = Node(i, sf::Vector2f(xPos, networkDimensions.top + (networkDimensions.height / nodeCount) * i + (networkDimensions.height / nodeCount) / 2));
		nodes.push_back(node);
	}
}

void Layer::ActivateNodes() {

}

//SFML
void Layer::Draw(sf::RenderTarget& window) {
	for (auto& n : nodes)
		n.Draw(window);
}