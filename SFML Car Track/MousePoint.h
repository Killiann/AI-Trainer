#pragma once
#include <SFML/Graphics.hpp>
class MousePoint
{
	struct Line {
		sf::Vector2f p1;
		sf::Vector2f p2;				
		Line() {};
		Line(sf::Vector2f _p1, sf::Vector2f _p2): p1(_p1), p2(_p2) {
			A = p2.y - p1.y;
			B = p1.x - p2.x;
			C = (A * p1.x) + (B * p1.y);
		}
		float A, B, C;
	};

	std::vector<sf::CircleShape> shapes;

	sf::Color lineColor = sf::Color::Black;
	sf::Color interSectColor = sf::Color::Red;

	const double PI = 3.141592653589793238463;
	const int lineLength = 500;
	int lineCount = 30;

	sf::CircleShape interactionPoint;

	sf::Vector2f pos;
	std::vector<sf::VertexArray> distanceLines;

	std::vector<sf::ConvexShape> trackShapes;

public:
	MousePoint(std::vector<sf::ConvexShape> t_shapes);
	void Update(sf::Vector2f mPos, float tileWidth);
	void Draw(sf::RenderWindow &window);
};

