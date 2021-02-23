#include "Track.h"
Track::Track() {
	//load track texture
	if (!trackTexture.loadFromFile("tracksheet.png")){}
	
	for (int y = 0; y < trackGridH; ++y) {
		for (int x = 0; x < trackGridW; ++x) {
			if (gridVals[y][x] != -1) {
				int trackPieceID = ((gridVals[y][x] - 1) / 4);
				float rotation = ((gridVals[y][x] - 1) % 4) * 90;
				trackPieces.emplace_back(trackTexture, tileSize, sf::Vector2f(x, y), (TrackPiece::TrackType)trackPieceID, rotation);
			}
		}
	}
}

void Track::Draw(sf::RenderWindow& window) {
	for (auto& t : trackPieces)
		t.Draw(window);
}

std::vector<sf::ConvexShape> Track::GetTrackShapes() {
	std::vector<sf::ConvexShape> returnVec;
	returnVec.reserve(2000);
	for (auto i : trackPieces) {
		std::vector<sf::ConvexShape> collShapes = i.GetCollisionShapes();		
		returnVec.insert(returnVec.end(), collShapes.begin(), collShapes.end());
	}
	return returnVec;
}