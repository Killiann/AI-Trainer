#include "Track.h"
Track::Track(ResourceManager *resource) : resourceManager(resource){		
	for (int y = 0; y < trackGridH; ++y) {
		for (int x = 0; x < trackGridW; ++x) {
			if (gridVals[y][x] != -1) {
				int trackPieceID = ((gridVals[y][x] - 1) / 4);
				float rotation = ((gridVals[y][x] - 1) % 4) * 90;
				trackPieces.emplace_back(*resourceManager->GetTrackTexture(), tileSize, sf::Vector2f(x, y), (TrackPiece::TrackType)trackPieceID, rotation);
			}
		}
	}

	//save all track shapes in one place 
	trackShapes.reserve(2000);
	for (auto i : trackPieces) {
		std::vector<sf::ConvexShape> collShapes = i.GetCollisionShapes();
		trackShapes.insert(trackShapes.end(), collShapes.begin(), collShapes.end());
	}
}

void Track::Draw(sf::RenderWindow& window, bool devOverview) {
	for (auto& t : trackPieces)
		t.Draw(window);
	
	if (devOverview) {
		for (auto& c : checkedArea)
			window.draw(c);
	}
}

void Track::addCheckedArea(sf::ConvexShape trackShape) {
	sf::RectangleShape rect;
	rect.setFillColor(sf::Color(25, 255, 25, 100));
	rect.setPosition(trackShape.getGlobalBounds().left, trackShape.getGlobalBounds().top);
	rect.setSize(sf::Vector2f(trackShape.getGlobalBounds().width, trackShape.getGlobalBounds().height));
	checkedArea.push_back(rect);
}