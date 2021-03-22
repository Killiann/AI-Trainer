#pragma once
#include "SFML/Graphics.hpp"
#include <math.h>

class TrackPiece
{
	const double PI = 3.141592653589793238463;
	const int trackDivisions = 8;
	int incr;
	float tileS;

	sf::Sprite drawable;
	std::vector<sf::ConvexShape> innerBounds;
	std::vector<sf::ConvexShape> accessibleTrack;
	sf::Sprite trackSprite;
	bool shapeRotating = false;

	void InitCorner(int, int, int, float, float);
	void TransformVertices(sf::ConvexShape&, sf::Vector2f, float);
public:
	enum TrackType
	{
		STRAIGHT,
		CORNER_SHORT,
		CORNER_LONG,
		DOUBLE_CORNER
	};

	TrackPiece(sf::Texture& trackTexture, int tileSize, sf::Vector2f gridPos, TrackType trackType, float rotation);	
	void Draw(sf::RenderTarget& wind);
	std::vector<sf::ConvexShape> GetCollisionShapes();	
};

