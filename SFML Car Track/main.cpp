#include <SFML/Graphics.hpp>
#include <iostream>
#include "Track.h"
#include "MousePoint.h";
#include "Car.h"
#include "ConsoleManager.h"
#include "InputManager.h"
#include "ResourceManager.h"

//game clock
sf::Clock clk;
sf::Time dt;

int main()
{
    //window setups
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1500, 1000), "Track Testing", sf::Style::Default, settings);            
    sf::View camera;
    camera.setSize(750, 500);    
    
    //managers 
    ResourceManager resourceManager = ResourceManager();
    ConsoleManager consoleManager(resourceManager.GetConsoleFont());
    InputManager inputManager(&consoleManager);    
    
    //world 
    Track track = Track(&resourceManager);    
    Car car(sf::Vector2f(216.0f, 400.0f), &inputManager, &consoleManager, &resourceManager, track.GetTrackShapes());

    while (window.isOpen())
    {
        dt = clk.restart();        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        //update 
        inputManager.Update();
        car.Update(dt.asSeconds());
        camera.setCenter(car.getPosition());
        
        //draw
        window.clear(sf::Color(139, 69, 19));
        window.setView(camera);

        //world
        track.Draw(window);        
        car.Draw(window);

        //console
        window.setView(window.getDefaultView());
        consoleManager.Draw(window);
        window.display();
    }

    return 0;
}