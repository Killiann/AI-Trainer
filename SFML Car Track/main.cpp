#include <SFML/Graphics.hpp>
#include <iostream>
#include "Track.h"
#include "MousePoint.h";
#include "Car.h"
#include "ConsoleManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Minimap.h"

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
    Car car(0, sf::Vector2f(216.0f, 385.0f), &inputManager, &consoleManager, &resourceManager, &track);
    Car car1(1, sf::Vector2f(266.0f, 385.0f), &inputManager, &consoleManager, &resourceManager, &track);
    Car car2(2, sf::Vector2f(316.0f, 385.0f), &inputManager, &consoleManager, &resourceManager, &track);
    Car car3(3, sf::Vector2f(216.0f, 435.0f), &inputManager, &consoleManager, &resourceManager, &track);
    Car car4(4, sf::Vector2f(266.0f, 435.0f), &inputManager, &consoleManager, &resourceManager, &track);
    Car car5(5, sf::Vector2f(316.0f, 435.0f), &inputManager, &consoleManager, &resourceManager, &track);
    car.Select();

    std::vector<Car*> cars;
    cars.push_back(&car);
    cars.push_back(&car1);
    cars.push_back(&car2);
    cars.push_back(&car3);
    cars.push_back(&car4);
    cars.push_back(&car5);
    

    Minimap minimap = Minimap(cars, &track);
    while (window.isOpen())
    {
        dt = clk.restart();        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized)
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
        }
        
        //update 
        inputManager.Update();
        consoleManager.Update();
        /*for (auto& c : cars)
            c->Update(dt.asSeconds());*/
        car.Update(dt.asSeconds());
        camera.setCenter(car.getPosition());
        
        //draw
        window.clear(sf::Color(139, 69, 19));
        window.setView(camera);

        //world
        track.Draw(window);        
        for (auto& c : cars)
            c->Draw(window);

        //console
        window.setView(window.getDefaultView());
        consoleManager.Draw(window);      

        //minimap
        minimap.Draw(window);
        window.display();
    }

    return 0;
}