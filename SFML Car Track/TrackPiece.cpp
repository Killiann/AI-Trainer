#include "TrackPiece.h"

#define _USE_MATH_DEFINES

TrackPiece::TrackPiece(sf::Texture& trackTexture, int tileSize, sf::Vector2f gridPos, TrackType trackType, float rotation){
	tileS = tileSize;
	float xPos = tileS * gridPos.x;
	float yPos = tileS * gridPos.y;
	incr = (float)tileS / 8;

	//rotation = 0;
	int texTileW = trackTexture.getSize().x / 5;
	trackSprite.setTexture(trackTexture);		
	trackSprite.setScale((float)tileS / texTileW, (float)tileS / texTileW);
	trackSprite.setPosition(xPos, yPos);
	trackSprite.setOrigin(texTileW / 2, texTileW / 2);
	trackSprite.setRotation(rotation);	

	//setup track collision shapes
	switch(trackType){
	case(STRAIGHT): {
		trackSprite.setTextureRect(sf::IntRect(texTileW, 0, texTileW, texTileW));

		sf::ConvexShape inner, outer;
		inner.setPointCount(4);
		outer.setPointCount(4);

		//top shape		
		inner.setPoint(0, sf::Vector2f(0, 0)); //top left
		inner.setPoint(1, sf::Vector2f(tileS, 0)); //top right
		inner.setPoint(2, sf::Vector2f(tileS, incr)); //bottom right
		inner.setPoint(3, sf::Vector2f(0, incr)); //bottom left

		//bottom shape
		outer.setPoint(0, sf::Vector2f(0, incr)); //top left
		outer.setPoint(1, sf::Vector2f(tileS, incr)); //top right
		outer.setPoint(2, sf::Vector2f(tileS, 5*incr)); //bottom left
		outer.setPoint(3, sf::Vector2f(0, 5*incr)); //bottom right

		outer.setFillColor(sf::Color(200, 200, 200));
 		TransformVertices(outer, sf::Vector2f(xPos, yPos), rotation);

		accessibleTrack.push_back(outer);
		break;
	}

	case(CORNER_SHORT):
		trackSprite.setTextureRect(sf::IntRect(texTileW * 3, 0, texTileW, texTileW));
		InitCorner(incr, incr * 5, rotation, xPos, yPos);
		break;	
	case(CORNER_LONG): 
		trackSprite.setTextureRect(sf::IntRect(texTileW * 4, 0, texTileW, texTileW));
		InitCorner(incr * 3, incr * 7, rotation, xPos, yPos);
		break;	
	case(DOUBLE_CORNER):
		trackSprite.setTextureRect(sf::IntRect(texTileW * 2, 0, texTileW, texTileW));
		InitCorner(incr, incr * 5, rotation, xPos, yPos);
		InitCorner(incr, incr * 5, rotation + 180, xPos, yPos);
		break;
	}
}

void TrackPiece::Draw(sf::RenderWindow& window) {	
/*	for (auto& s : accessibleTrack)
		window.draw(s);

	for (auto& s : innerBounds)
		window.draw(s);	*/		

	window.draw(trackSprite);
}

void TrackPiece::InitCorner(int startPos, int endPos, int rotation, float xPos, float yPos) {
	int innerRadiusDefinition = 10;
	int outerRadiusDefinition = 15;

	sf::ConvexShape inner, outer;
	inner.setPointCount(2 + innerRadiusDefinition);
	outer.setPointCount(2 + outerRadiusDefinition);

	for (int i = 0; i < innerRadiusDefinition; ++i) {
		float theta = ((PI / 2) / innerRadiusDefinition);
		float angle = (theta * i);
		inner.setPoint(i, sf::Vector2f(startPos * cos(angle), startPos * sin(angle)));
	}
	inner.setPoint(innerRadiusDefinition, sf::Vector2f(0, startPos));
	inner.setPoint(innerRadiusDefinition + 1, sf::Vector2f(0, 0));

	for (int i = 0; i < outerRadiusDefinition; ++i) {
		float theta = ((PI / 2) / outerRadiusDefinition);
		float angle = (theta * i);
		outer.setPoint(i, sf::Vector2f(endPos * cos(angle), endPos * sin(angle)));
	}

	outer.setPoint(outerRadiusDefinition, sf::Vector2f(0, endPos));
	//outer.setPoint(outerRadiusDefinition + 1, sf::Vector2f(0, 0));

	outer.setFillColor(sf::Color(200, 200, 200));	
	TransformVertices(outer, sf::Vector2f(xPos, yPos), rotation);

	inner.setFillColor(sf::Color::Red);
	TransformVertices(inner, sf::Vector2f(xPos, yPos), rotation);

	innerBounds.push_back(inner);
	accessibleTrack.push_back(outer);
}

void TrackPiece::TransformVertices(sf::ConvexShape &transformable, sf::Vector2f pos, float rotation) {
	transformable.setPosition(tileS / 2, tileS / 2);
	transformable.setOrigin(tileS / 2, tileS / 2);
	
	sf::Transformable tr = (sf::Transformable)transformable;
	tr.setPosition(pos.x, pos.y);
	tr.setOrigin(tileS / 2, tileS / 2);
	tr.rotate(rotation);

	sf::Transform t = tr.getTransform();
	for (int i = 0; i < transformable.getPointCount(); ++i)
		transformable.setPoint(i, t.transformPoint(transformable.getPoint(i)));
}

std::vector<sf::ConvexShape> TrackPiece::GetCollisionShapes() {
	std::vector<sf::ConvexShape> allShapes;	
	allShapes.reserve(innerBounds.size() + accessibleTrack.size());
	allShapes.insert(allShapes.end(), innerBounds.begin(), innerBounds.end());
	allShapes.insert(allShapes.end(), accessibleTrack.begin(), accessibleTrack.end());
	return allShapes;
}
