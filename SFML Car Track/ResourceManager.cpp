#include "ResourceManager.h"

ResourceManager::ResourceManager() {
    //load textures
    std::cout << "Loading textures.." << std::endl;    
    if (!carTexture.loadFromFile("Resources/car_01.png")) {
        std::cout << "Could not get car (01) texture." << std::endl;
    }
    if (!driftTexture.loadFromFile("Resources/drift.png")) {
        std::cout << "C1ould not get drift texture." << std::endl;
    }
    if (!trackTexture.loadFromFile("Resources/tracksheet.png")) {
        std::cout << "Could not get tracksheet.png.\n";
    }
    std::cout << "Done. \nLoading fonts..." << std::endl;
    
    //load fonts
    if (!consoleFont.loadFromFile("Resources/Roboto-Black.ttf")) {
        std::cout << "Could not get Console Font." << std::endl;
    }
    if (!pixelFont.loadFromFile("Resources/DotGothic.ttf")) {
        std::cout << "Could not get Pixel Font (DotGothic)" << std::endl;
    }    
    std::cout << "Done." << std::endl;
}