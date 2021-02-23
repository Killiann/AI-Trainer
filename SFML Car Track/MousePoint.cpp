#include "MousePoint.h"


MousePoint::MousePoint(std::vector<sf::ConvexShape> t_shapes) : trackShapes(t_shapes) {
	interactionPoint.setRadius(2);
	interactionPoint.setFillColor(sf::Color::Red);
	interactionPoint.setOrigin(2, 2);	
}

void MousePoint::Update(sf::Vector2f mPos, float tileSize) {
	distanceLines.clear();
	for (int i = 0; i < lineCount; ++i) {
		sf::VertexArray newLine = sf::VertexArray(sf::LinesStrip);
		newLine.append(sf::Vertex(mPos, sf::Color::Black));		
		float theta = ((PI * 2) / lineCount);
		float angle = (theta * i);

		Line line1 = Line(mPos, sf::Vector2f(mPos.x + (lineLength * cos(angle)),
											mPos.y + (lineLength * sin(angle))));		

		float shortestDistance = lineLength;
		for (auto &trackShape : trackShapes) {
			bool intersecting = false;
			
			for (int i = 0; i < trackShape.getPointCount(); ++i)
			{							
				Line line2;
				if (i == trackShape.getPointCount() - 1) line2 = Line(trackShape.getPoint(i), trackShape.getPoint(0));
				else line2 = Line(trackShape.getPoint(i), trackShape.getPoint(i + 1));

				float testLen = std::sqrt(std::pow(line2.p1.x - line2.p2.x, 2) + std::pow(line2.p1.y - line2.p2.y, 2));
				float trackWidth = (tileSize / 8) * 4;
				if(testLen < tileSize * 0.2 || testLen > tileSize * 0.9){
					float det = (line1.A * line2.B) - (line2.A * line1.B);
					if (det != 0) { //if not parallel
						float x = (line2.B * line1.C - line1.B * line2.C) / det;
						float y = (line1.A * line2.C - line2.A * line1.C) / det;
						if ((std::min(line2.p1.x, line2.p2.x) <= x + 1) && (x - 1 <= std::max(line2.p1.x, line2.p2.x)) &&
							(std::min(line2.p1.y, line2.p2.y) <= y + 1) && (y - 1 <= std::max(line2.p1.y, line2.p2.y)) &&
							(std::min(line1.p1.x, line1.p2.x) <= x + 1) && (x - 1 <= std::max(line1.p1.x, line1.p2.x)) &&
							(std::min(line1.p1.y, line1.p2.y) <= y + 1) && (y - 1 <= std::max(line1.p1.y, line1.p2.y))) {
							float length = std::sqrt(std::pow(line1.p1.x - x, 2) + std::pow(line1.p1.y - y, 2));
							if (shortestDistance > length) {
								interactionPoint.setPosition(sf::Vector2f(x, y));
								line1.p2 = sf::Vector2f(x, y);
								shortestDistance = length;
							}
						}
					}
				}
			}
		}

		sf::Color c = lineColor;
		newLine.append(sf::Vertex(line1.p2, c));

		distanceLines.push_back(newLine);
	}
}

void MousePoint::Draw(sf::RenderWindow& window) {
	for (auto& l : distanceLines)
		window.draw(l);

	window.draw(interactionPoint);
}