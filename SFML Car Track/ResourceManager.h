#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class ResourceManager
{
	//textures
	sf::Texture carTexture;
	sf::Texture driftTexture;
	sf::Texture trackTexture;

	//fonts
	sf::Font consoleFont;
public:
	ResourceManager();
	
	sf::Texture* GetCarTexture() { return &carTexture; }
	sf::Texture* GetDriftTexture() { return &driftTexture; }
	sf::Texture* GetTrackTexture() { return &trackTexture; }

	sf::Font* GetConsoleFont() { return &consoleFont; }
};

