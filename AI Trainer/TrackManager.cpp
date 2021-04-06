#include "TrackManager.h"

TrackManager::TrackManager(ResourceManager* resource, ConsoleManager* console): resourceManager(resource) , consoleManager(console) {
    //track 1 setup
    std::vector<std::vector<int>> track1_grid = {
    { -1 ,-1 ,-1 ,-1 ,11 ,12},
    { -1 ,-1 ,11 ,1  ,13 ,9 },
    { 11 ,1  ,13 ,8  ,6  ,12},
    { 10 ,3  ,9  ,10 ,3  ,9 }
    };

    //add checkpoints
    std::vector<CheckPoint> track1_checkpoints;
    sf::Texture* cpTexture = resourceManager->GetCheckpointTexture();
    track1_checkpoints.emplace_back(0, sf::Vector2f(1, 2), PosInTile::RT, cpTexture, consoleManager);
    track1_checkpoints.emplace_back(1, sf::Vector2f(2, 2), PosInTile::TL, cpTexture, consoleManager);
    track1_checkpoints.emplace_back(2, sf::Vector2f(3, 1), PosInTile::LT, cpTexture, consoleManager);
    track1_checkpoints.emplace_back(4, sf::Vector2f(4, 1), PosInTile::LT, cpTexture, consoleManager);
    track1_checkpoints.emplace_back(3, sf::Vector2f(4, 1), PosInTile::TL, cpTexture, consoleManager);
    track1_checkpoints.emplace_back(5, sf::Vector2f(4, 0), PosInTile::RT, cpTexture, consoleManager);
    track1_checkpoints.emplace_back(6, sf::Vector2f(5, 0), PosInTile::BR, cpTexture, consoleManager);
    track1_checkpoints.emplace_back(7, sf::Vector2f(5, 1), PosInTile::LB, cpTexture, consoleManager);
    track1_checkpoints.emplace_back(8, sf::Vector2f(4, 1), PosInTile::BR, cpTexture, consoleManager);
    track1_checkpoints.emplace_back(9, sf::Vector2f(4, 2), PosInTile::RT, cpTexture, consoleManager);
    track1_checkpoints.emplace_back(10, sf::Vector2f(5, 2), PosInTile::BR, cpTexture, consoleManager);
    track1_checkpoints.emplace_back(11, sf::Vector2f(5, 3), PosInTile::LB, cpTexture, consoleManager);
    track1_checkpoints.emplace_back(12, sf::Vector2f(4, 3), PosInTile::LB, cpTexture, consoleManager);
    track1_checkpoints.emplace_back(13, sf::Vector2f(2, 2), PosInTile::RB, cpTexture, consoleManager);
    track1_checkpoints.emplace_back(14, sf::Vector2f(1, 3), PosInTile::LB, cpTexture, consoleManager);

    Track track1 = Track(resourceManager, track1_checkpoints, track1_grid, 6, 4);
    tracks.push_back(track1);
}

void TrackManager::DrawTrack(sf::RenderTarget& window){
    tracks[currentTrack].Draw(window, consoleManager->IsDisplayed());
}