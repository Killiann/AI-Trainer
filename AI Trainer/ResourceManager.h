#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class ResourceManager
{
	//textures
	sf::Texture carTexture;
	sf::Texture driftTexture;
	sf::Texture trackTexture;
	sf::Texture checkpointTexture;

	//fonts
	sf::Font consoleFont;
	sf::Font pixelFont;

public:
	ResourceManager();
	
	sf::Texture* GetCarTexture() { return &carTexture; }
	sf::Texture* GetDriftTexture() { return &driftTexture; }
	sf::Texture* GetTrackTexture() { return &trackTexture; }
	sf::Texture* GetCheckpointTexture() { return &checkpointTexture; }

	sf::Font* GetConsoleFont() { return &consoleFont; }
	sf::Font* GetPixelFont() { return &pixelFont; }
};

