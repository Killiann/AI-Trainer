#include "CheckPointManager.h"

/// <summary>
/// Initialise CheckpointManager
/// </summary>
/// <param name="cpTexture">sf::Texture* checkpoint texture</param>
/// <param name="cPoints">std::vector<CheckPoint> checkpoints</param>
CheckPointManager::CheckPointManager(sf::Texture *cpTexture, std::vector<CheckPoint> cPoints) : checkpoints(cPoints){	
}

/// <summary>
/// Draw All Checkpoints
/// </summary>
/// <param name="window">sf::RenderTarget reference</param>
/// <param name="isOverlay">Boolean, display overlay</param>
void CheckPointManager::Draw(sf::RenderTarget& window, bool isOverlay) {
	for (auto& c : checkpoints)
		c.Draw(window, isOverlay);
}