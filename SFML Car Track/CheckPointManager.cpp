#include "CheckPointManager.h"

CheckPointManager::CheckPointManager(sf::Texture *cpTexture, std::vector<CheckPoint> cPoints) : checkpoints(cPoints){	
}

void CheckPointManager::Draw(sf::RenderTarget& window) {
	for (auto& c : checkpoints)
		c.Draw(window);
}