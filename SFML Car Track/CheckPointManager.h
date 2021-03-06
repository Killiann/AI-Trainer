#pragma once
#include "SFML/Graphics.hpp"
#include "ConsoleManager.h"
#include "CheckPoint.h"

class CheckPointManager{
	std::vector<CheckPoint> checkpoints;
	ConsoleManager* console;

public:
	CheckPointManager(){}
	CheckPointManager(sf::Texture* cpTexture, ConsoleManager *cm);
	void Draw(sf::RenderTarget& window);

	inline std::vector<CheckPoint>* GetCheckpoints() { return &checkpoints; }
};
