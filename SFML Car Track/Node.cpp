#include "Node.h"

Node::Node(int ID, sf::Vector2f p) : id(ID), position(p){
	shape.setPosition(position);
	shape.setRadius(10);
	shape.setOrigin(10, 10);
	shape.setOutlineColor(sf::Color::Red);
	shape.setOutlineThickness(1);
	shape.setFillColor(sf::Color::White);
}

void Node::ActivationFunction() {

}

//SFML
void Node::Draw(sf::RenderTarget& window) {
	window.draw(shape);
}
