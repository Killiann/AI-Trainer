#include "Node.h"

Node::Node(int ID, sf::Vector2f p) : id(ID), position(p){
	
	std::random_device rd;
	gen = std::mt19937(rd());
	distr = std::uniform_int_distribution<>(-300, 300);

	shape.setPosition(position);
	int radius = 12;
	shape.setRadius(radius);
	shape.setOrigin(radius, radius);
	//shape.setOutlineColor(sf::Color::Black);
	//shape.setOutlineThickness(1);
	shape.setFillColor(sf::Color::White);
}

void Node::CreateConnections(std::vector<Node> &prevLayerNodes) {
	//connections = std::vector <std::pair<Node, float>>(prevLayerNodes.size());
	connectionLines.clear();
	for (int i = 0; i < prevLayerNodes.size(); ++i) {
		std::pair<Node, float> pair(prevLayerNodes[i], (float)distr(gen) / 100);
 		//connections[i] = (pair);

		sf::VertexArray connectionLine = CreateLine(position, prevLayerNodes[i].position, pair.second);
		connectionLines.push_back(connectionLine);
	}
}

void Node::ActivationFunction() {
	//temp
	float sum = 0;
	//for (int i = 0; i < connections.size(); ++i)
	//	sum += (connections[i].first.GetValue() * connections[i].second); //add val * weight
 	
 	value = lin::sigmoid(sum);
}

//SFML
void Node::DrawLines(sf::RenderTarget& window) {
	for (auto l : connectionLines)
		window.draw(l);
}
void Node::DrawNode(sf::RenderTarget& window) {
	float colorVal = std::min(std::abs(value) * 255, 255.f);
	shape.setFillColor(sf::Color(colorVal, colorVal, colorVal));
	window.draw(shape);
}
 
sf::VertexArray Node::CreateLine(sf::Vector2f p1, sf::Vector2f p2, float weight) {
	sf::Color lineColor = weight < 0 ? sf::Color::Red : sf::Color::Green;
	sf::Vector2f line = p2 - p1;
	sf::Vector2f normal(-line.y, line.x);
	Normalise(normal);
	sf::VertexArray returnShape(sf::TriangleStrip, 4);
	returnShape[0].position = p1 - (weight / 100) * normal;
	returnShape[1].position = p1 + (weight / 100) * normal;
	returnShape[2].position = p2 - (weight / 100) * normal;
	returnShape[3].position = p2 + (weight / 100) * normal;
	
	for (int i = 0; i < returnShape.getVertexCount(); ++i)
		returnShape[i].color = lineColor;

	return returnShape;
}

sf::Vector2f Node::Normalise(const sf::Vector2f& source) {
    float length = sqrt((source.x * source.x) + (source.y * source.y));
    if (length != 0)
        return sf::Vector2f(source.x / length, source.y / length);
    else
        return source;
}