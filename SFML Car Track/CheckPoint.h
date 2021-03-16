#pragma once
#include "SFML/Graphics.hpp"
#include "ConsoleManager.h"

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
	sf::RectangleShape collisionRect;

	sf::Vector2f center;
	sf::CircleShape c;

	ConsoleManager* console;

public:
	CheckPoint(){}
	CheckPoint(int ID, int ts, sf::Vector2f pos, posInTile pit, sf::Texture* texture, ConsoleManager* cm);
	void Draw(sf::RenderTarget& window);
	inline sf::FloatRect getGlobalBounds() { return collisionRect.getGlobalBounds(); }
	inline int getID() { return ID; }
	inline sf::Vector2f GetCenter() { return center; }
};
