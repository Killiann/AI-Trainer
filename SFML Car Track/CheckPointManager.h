#pragma once
#include "SFML/Graphics.hpp"
#include "ResourceManager.h"
#include "Track.h"
#include "Car.h"

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

	inline std::vector<CheckPoint>* GetCheckpoints() { return &checkpoints; }
};

struct Segment {
	Segment(int id, CheckPoint *_c1, CheckPoint *_c2) {
		c1 = _c1;
		c2 = _c2;
		ID = id;
	}
	int ID;
	CheckPoint* c1, * c2;
	std::vector<float> timings;
	float getLast() { return timings[timings.size() - 1]; }
	float GetFastest() { return *std::min_element(timings.begin(), timings.end()); }
};

class CheckPointTracker {
	std::vector<CheckPoint> checkpoints;
	std::vector<Segment> segments;
	Car* car;
	int currentSegmentID;
public:
	CheckPointTracker(CheckPointManager* checkpointManager, Car* c);
	void Update();
};