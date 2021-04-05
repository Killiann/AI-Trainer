#pragma once

#include <SFML/Graphics.hpp>
#include "Track.h"
#include "CheckPoint.h"
#include "ConsoleManager.h"
#include "ResourceManager.h"

class TrackManager
{
	std::vector<Track> tracks;
	int currentTrack = 0;

	ConsoleManager* consoleManager = nullptr;
	ResourceManager* resourceManager = nullptr;

public:
	TrackManager() {}
	TrackManager(ResourceManager* resource, ConsoleManager* console);
	
	inline Track& GetCurrentTrack() { return tracks[currentTrack]; }
	Track& GetNextTrack() {
		if (currentTrack == tracks.size() - 1) currentTrack = 0;
		else currentTrack++;

		return GetCurrentTrack();
	}

	void DrawTrack(sf::RenderTarget& window);
};

