#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "TrackPiece.h"
#include "ResourceManager.h"

class Track
{
	enum TrackGuide {
		STRAIGHT_UP, //1
		STRAIGHT_RIGHT, //2 
		STRAIGHT_DOWN, //3
		STRAIGHT_LEFT, //4 
		CORNER_SHORT_UP, //5 
		CORNER_SHORT_RIGHT, //6 
		CORNER_SHORT_DOWN, //7 
		CORNER_SHORT_LEFT, //8 
		CORNER_LONG_UP, //9 
		CORNER_LONG_RIGHT, //10 
		CORNER_LONG_DOWN, //11
		CORNER_LONG_LEFT, //12 
		DOUBLE_CORNER_UP, //13 
		DOUBLE_CORNER_RIGHT, //14		
	};

	std::vector<TrackPiece> trackPieces;
	std::vector<sf::ConvexShape> trackShapes;

	static const int trackGridH = 4, trackGridW = 6;
	int gridVals[trackGridH][trackGridW] = {
		{ -1,-1,-1,-1,11,12},
		{ -1 ,-1 ,11,1 ,13,9 },
		{ 11,1 ,13,8 ,6 ,12},
		{ 10,3 ,9 ,10 ,3,9 }
	};

	static const int tileSize= 432; //texture rect size * 2
	static const int pieceDivisions = 8;
	static const int trackWidth = 4;

	std::vector<sf::RectangleShape> checkedArea;
	ResourceManager* resourceManager;

public:
	Track(ResourceManager *resourceManager);	
	void Draw(sf::RenderTarget& window, bool devOverview);
	void addCheckedArea(sf::ConvexShape trackShape);

	inline std::vector<sf::ConvexShape>* GetTrackShapes() { return &trackShapes; }
	inline void clearCheckedArea() { checkedArea.clear(); }
	inline int GetTileWidth() { return tileSize; }
};

