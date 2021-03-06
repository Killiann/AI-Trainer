#include "CheckPointManager.h"

CheckPointManager::CheckPointManager(sf::Texture *cpTexture, ConsoleManager *cm) : console(cm){	
	checkpoints.push_back(CheckPoint(0, 432, sf::Vector2f(1, 2), RT, cpTexture, console));
	checkpoints.push_back(CheckPoint(1, 432, sf::Vector2f(3, 1), LT, cpTexture, console));
	checkpoints.push_back(CheckPoint(2, 432, sf::Vector2f(4, 0), BL, cpTexture, console));
	checkpoints.push_back(CheckPoint(3, 432, sf::Vector2f(5, 1), LB, cpTexture, console));
	checkpoints.push_back(CheckPoint(4, 432, sf::Vector2f(5, 2), BR, cpTexture, console));
	checkpoints.push_back(CheckPoint(5, 432, sf::Vector2f(4, 3), LB, cpTexture, console));
	checkpoints.push_back(CheckPoint(6, 432, sf::Vector2f(2, 2), RB, cpTexture, console));
	checkpoints.push_back(CheckPoint(7, 432, sf::Vector2f(1, 3), LB, cpTexture, console));
}

void CheckPointManager::Draw(sf::RenderTarget& window) {
	for (auto& c : checkpoints)
		c.Draw(window);
}