#include "Minimap.h"

Minimap::Minimap(std::vector<Car*> c, Track* t, ConsoleManager* cm) : cars(c), track(t), consoleManager(cm){
	//setup map markers
	for (auto& i : cars) {
		sf::CircleShape circle;
		circle.setOutlineThickness(15.0f);
		circle.setOutlineColor(sf::Color::Red);
		circle.setFillColor(sf::Color::Transparent);
		float radius = 40;
		circle.setRadius(radius);
		circle.setOrigin(radius, radius);
		mapMarkers.push_back(circle);
	}

	//bg
	bg.setSize(sf::Vector2f(15000.0f, 10000.0f));
	bg.setPosition(-2000, -2000);
	bg.setFillColor(sf::Color::White);

	//init view
	miniMap.setSize(3000, 2000);
	miniMap.setViewport(sf::FloatRect(0.75f, 0.0f, 0.25f, 0.25f));	
	miniMap.setCenter(1100, 600);
	
	int padding = mapMarkers[0].getRadius() + mapMarkers[0].getOutlineThickness();

	boundingRectangle = sf::RectangleShape(sf::Vector2f(miniMap.getSize().x - (padding*2), miniMap.getSize().y -(padding * 2)));
	boundingRectangle.setPosition(miniMap.getCenter().x - (miniMap.getSize().x / 2) + padding,
		miniMap.getCenter().y - (miniMap.getSize().y / 2) + padding);
	boundingRectangle.setFillColor(sf::Color(255,255,255, 55));
}

void Minimap::Draw(sf::RenderWindow& window) {
	window.setView(miniMap);
	//window.draw(bg);
	window.draw(boundingRectangle);
	track->Draw(window, consoleManager->IsDisplayed());
	for (int i = 0; i < cars.size(); ++i) {
		if (cars[i]->isSelected() && consoleManager->IsDisplayed())
			window.draw(cars[i]->getScanArea());
			
		sf::Vector2f pos = cars[i]->getPosition();
		if (pos.x < boundingRectangle.getPosition().x)
			pos.x = boundingRectangle.getPosition().x;
		else if (pos.x > boundingRectangle.getPosition().x + boundingRectangle.getSize().x)
			pos.x = boundingRectangle.getPosition().x + boundingRectangle.getSize().x;
		if (pos.y < boundingRectangle.getPosition().y)
			pos.y = boundingRectangle.getPosition().y;
		else if (pos.y > boundingRectangle.getPosition().y + boundingRectangle.getSize().y)
			pos.y = boundingRectangle.getPosition().y + boundingRectangle.getSize().y;

		mapMarkers[i].setPosition(pos);
		if (cars[i]->isSelected() && mapMarkers[i].getOutlineColor() == sf::Color::Red)mapMarkers[i].setOutlineColor(sf::Color::Green);
		else if (!cars[i]->isSelected() && mapMarkers[i].getOutlineColor() == sf::Color::Green) mapMarkers[i].setOutlineColor(sf::Color::Red);
		window.draw(mapMarkers[i]);
	}
	window.setView(window.getDefaultView());
}