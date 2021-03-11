#include "Network.h"
Network::Network(sf::FloatRect dims, std::vector<int> layerData) : dimensions(dims){
	for (int i = 0; i < layerData.size(); ++i) {
		//create new layer
		Layer layer = Layer(i, layerData[i], dimensions, dimensions.left + (dimensions.width / layerData.size()) * i +
			((dimensions.width / layerData.size()) / 2));
		layers.push_back(layer);
	}
}

Network::Network(sf::FloatRect dims, std::vector<int> layerData, std::vector<float> weights) : dimensions(dims) {

}

//std::vector<float> Network::CalculateOutput(std::vector<float> inputs) {
//
//}

//SFML
void Network::Draw(sf::RenderTarget &window) {
	for (auto& l : layers)
		l.Draw(window);
}