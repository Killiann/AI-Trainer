#include <SFML/Graphics.hpp>
#include <iostream>
#include "Track.h"
#include "Car.h"
#include "ConsoleManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Minimap.h"
#include <algorithm>
#include <numeric>

//game clock
sf::Clock clk;
sf::Time dt;
std::vector<float> allFPS;

int main()
{
    //window setups
    sf::ContextSettings settings;    
    settings.antialiasingLevel = 8;    
    sf::RenderWindow window(sf::VideoMode(1500, 1000), "Track Testing", sf::Style::Default, settings);                
    sf::View camera;
    window.setFramerateLimit(90);
    camera.setSize(1500, 1000);

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

    int currentID = 0;
    std::vector<Car*> cars;
    cars.push_back(&car);
    cars.push_back(&car1);
    cars.push_back(&car2);
    cars.push_back(&car3);
    cars.push_back(&car4);
    cars.push_back(&car5);
    
    Minimap minimap = Minimap(cars, &track, &consoleManager);
    
    //dev
    sf::Vector2i mousePos;
    sf::Vector2f mouseCoords;    
    consoleManager.AddMessage("mousePos");
    consoleManager.AddMessage("mouseCoords");
    consoleManager.AddMessage("dt");
    consoleManager.AddMessage("framerate");
    consoleManager.AddMessage("average framerate");

    allFPS.reserve(100000);
    while (window.isOpen())
    {   
        //handle mousePos conversions 
        mousePos = sf::Mouse::getPosition(window);
        mousePos = sf::Vector2i(mousePos.x - window.getPosition().x, mousePos.y - window.getPosition().y);
        mouseCoords = window.mapPixelToCoords(sf::Mouse::getPosition(window), camera);
        consoleManager.UpdateMessageValue("mousePos", std::to_string(mousePos.x) + " " + std::to_string(mousePos.y));
        consoleManager.UpdateMessageValue("mouseCoords", std::to_string(mouseCoords.x) + " " + std::to_string(mouseCoords.y));

        //handle framerate - avg fps can be added but is limited to vector size (just for testing)
        float fps = 1.f / clk.getElapsedTime().asSeconds();
        allFPS.push_back(fps);
        consoleManager.UpdateMessageValue("average framerate", std::to_string(std::accumulate(allFPS.begin(), allFPS.end(), 0.0) / allFPS.size()));        
        consoleManager.UpdateMessageValue("framerate", std::to_string(fps));
        
        //time between loops
        dt = clk.restart();      
        consoleManager.UpdateMessageValue("dt", std::to_string(dt.asSeconds()));

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

            if (event.type == sf::Event::MouseButtonPressed)
            {
                //select cars
                if (event.mouseButton.button == sf::Mouse::Left)
                {                                        
                    for (auto& c : cars) {                        
                        if (c->containsPoint(mouseCoords)) {
                            if (c->getID() != currentID) {
                                c->Select();
                                 cars[currentID]->Deselect();
                                currentID = c->getID();
                            }
                        }
                    }
                }
            }
        }
        
        //update 
        inputManager.Update();
        consoleManager.Update();        

        for (auto& c : cars)
            c->Update(dt.asSeconds());        

        camera.setCenter((int)cars[currentID]->getPosition().x, (int)cars[currentID]->getPosition().y);
        
        //draw
        window.clear(sf::Color(139, 69, 19));
        window.setView(camera);

        //world
        track.Draw(window, false);        
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