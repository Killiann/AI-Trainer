#pragma once
#include "SFML/Graphics.hpp"
#include "CheckPoint.h"

class CheckPointManager{
	std::vector<CheckPoint> checkpoints;

public:
	CheckPointManager(){}
	CheckPointManager(sf::Texture* cpTexture, std::vector<CheckPoint> cPoints);

	/// <summary>
	/// Get all checkpoints reference
	/// </summary>
	/// <returns>std::vector<CheckPoint> checkpoints</returns>
	inline std::vector<CheckPoint> &GetCheckpoints() { return checkpoints; }

	void Draw(sf::RenderTarget& window, bool isOverlay);
};
