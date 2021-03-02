#pragma once
#include "SFML/Graphics.hpp"
#include "ResourceManager.h"
#include "Track.h"

enum posInTile {
	TL, //top left
	TR, //top right
	RT, //right top
	RB, //right bottom
	BR, //bottom right
	BL, //bottom left
	LB, //left bottom
	LT //left top
};

class CheckPoint {
	int ID;
	int tileSize;
	bool isStart;
	sf::Vector2f position;
	posInTile PIT;
	sf::Sprite sprite;
	sf::Texture* texture;

public:
	CheckPoint(int ID, int ts, sf::Vector2f pos, posInTile pit, sf::Texture* texture);
	void Draw(sf::RenderTarget& window);
	inline int getID() { return ID; }
};


class CheckPointManager
{
	std::vector<CheckPoint> checkpoints;

public:
	CheckPointManager(sf::Texture* cpTexture);
	void Draw(sf::RenderTarget& window);
};
