#include <SFML/Graphics.hpp>
#include "Track.h"
#include "MousePoint.h";

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(800, 800), "Track Testing", sf::Style::Default, settings);            

    sf::View camera;
    camera.setSize(1600, 1600);
    camera.setCenter(600, 400);   

    Track track = Track();
    MousePoint mp = MousePoint(track.GetTrackShapes());

    while (window.isOpen())
    {
        window.setView(camera);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        //camera.setCenter(sf::Mouse::getPosition().x, sf::Mouse::getPosition().);
        mp.Update(window.mapPixelToCoords(sf::Mouse::getPosition(window)), track.GetTileWidth());
        
        window.clear(sf::Color::White);        
        track.Draw(window);
        mp.Draw(window);        
        window.display();
    }

    return 0;
}