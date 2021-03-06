#pragma once
#include "SFML/Graphics.hpp"

namespace lin {
	bool isPointToLeft(sf::Vector2f A, sf::Vector2f B, sf::Vector2f P);
	bool isPointToRight(sf::Vector2f A, sf::Vector2f B, sf::Vector2f P);

	bool doesRectContainPoint(sf::Vector2f P, sf::Vector2f A, sf::Vector2f B, sf::Vector2f C, sf::Vector2f D);

	/// <summary>
	/// Line Structure in the form Ax + By = C
	/// </summary>
	struct Line {
		sf::Vector2f p1;
		sf::Vector2f p2;
		Line() { A = B = C = 0.f; }
		Line(sf::Vector2f _p1, sf::Vector2f _p2) : p1(_p1), p2(_p2) {
			A = p2.y - p1.y;
			B = p1.x - p2.x;
			C = (A * p1.x) + (B * p1.y);
		}
		float A, B, C;
	};
}
