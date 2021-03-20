#include <SFML/Graphics.hpp>
#include <iostream>
#include "Track.h"
#include "Car.h"
#include "ConsoleManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "CheckPointManager.h"
#include "Minimap.h"
#include <algorithm>
#include <numeric>

#include "Network.h"
#include <linearAlgebra.h>

//game clock
sf::Clock clk;
sf::Time dt;
std::vector<float> allFPS;

int generationSize = 5;
bool isCycleDown = false;

float doub(float n) {
    return n * 2;
}

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
    CheckPointManager checkPointManager(resourceManager.GetCheckpointTexture(), &consoleManager);

    //world 
    Track track = Track(&resourceManager);    
   
    //setup
    //neural net positioning on screen
    sf::FloatRect dimensions = sf::FloatRect(sf::Vector2f(window.getViewport(window.getDefaultView()).width * 0.75, window.getViewport(window.getDefaultView()).height * 0.25),
        sf::Vector2f(window.getViewport(window.getDefaultView()).width / 4, window.getViewport(window.getDefaultView()).height / 4));

    std::vector<Car> cars;
    std::vector<Network> brains;
    int currentID = 0;
    for (int i = 0; i < generationSize; ++i) {
        //cars
        Car car = Car(i, sf::Vector2f(532.0f, 770.0f), &inputManager, &consoleManager, &resourceManager, checkPointManager, &track);
        cars.push_back(car);

        //networks
        Network nn(5, { 3, 3 }, 5, dimensions);
        brains.push_back(nn);
    }
    cars[currentID].Select();

    Minimap minimap = Minimap(&cars, &track, &consoleManager);
    
    //dev
    sf::Vector2i mousePos;
    sf::Vector2f mouseCoords;    

    consoleManager.AddMessage("=========CAR========");
    consoleManager.AddMessage("steer angle");
    consoleManager.AddMessage("velocity.x");
    consoleManager.AddMessage("velocity.y");
    consoleManager.AddMessage("skid count");
    consoleManager.AddMessage("current segment");
    consoleManager.AddMessage("fastest time");
    consoleManager.AddMessage("last lap");
    consoleManager.AddMessage("on track");
    consoleManager.AddMessage("fitness");
    consoleManager.AddMessage("alive");
    consoleManager.AddMessage("========DATA========");    
    consoleManager.AddMessage("mousePos");
    consoleManager.AddMessage("mouseCoords");
    consoleManager.AddMessage("dt");
    consoleManager.AddMessage("framerate");
    consoleManager.AddMessage("average framerate");
    consoleManager.AddMessage("====================");

    lin::Matrix m1(4, 6);
    lin::Matrix m2(6, 2);
    m1.Randomise(-1, 1);
    m2.Randomise(-1, 1);

    allFPS.reserve(100000);
    while (window.isOpen())
    {
        //handle mousePos conversions 
        mousePos = sf::Mouse::getPosition(window);
        mousePos = sf::Vector2i(mousePos.x - window.getPosition().x, mousePos.y - window.getPosition().y);
        mouseCoords = window.mapPixelToCoords(sf::Mouse::getPosition(window), camera);
        consoleManager.UpdateMessageValue("mousePos", std::to_string(mousePos.x) + " " + std::to_string(mousePos.y));
        consoleManager.UpdateMessageValue("mouseCoords", "x:" + std::to_string((int)mouseCoords.x) + "  y:" + std::to_string((int)mouseCoords.y));

        //handle framerate - avg fps can be added but is limited to vector size (just for testing)
        float fps = 1.f / clk.getElapsedTime().asSeconds();
        allFPS.push_back(fps);
        consoleManager.UpdateMessageValue("average framerate", std::to_string((int)(std::accumulate(allFPS.begin(), allFPS.end(), 0.0) / allFPS.size())));
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
                        if (c.containsPoint(mouseCoords)) {
                            if (c.getID() != currentID) {
                                c.Select();
                                cars[currentID].Deselect();
                                currentID = c.getID();
                            }
                        }
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Key::Num1) {
                    if (!isCycleDown) {
                        isCycleDown = true;
                        cars[currentID].Deselect();
                        if (currentID == (cars.size() - 1))
                            currentID = 0;
                        else ++currentID;
                        cars[currentID].Select();
                    }
                }
            }
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Key::Num1) {
                    if (isCycleDown) isCycleDown = false;
                }
            }
        }

        //update 
        inputManager.Update();
        cars[currentID].SetInputs(inputManager.GetCarInputs());
        consoleManager.Update();

        /*for (auto& c : cars)
            c.Update(dt.asSeconds());*/

        for (int i = 0; i < cars.size(); ++i) {
            cars[i].Update(dt.asSeconds());
            brains[i].FeedForward(cars[i].GetDistances());
        }

        camera.setCenter((int)cars[currentID].getPosition().x, (int)cars[currentID].getPosition().y);
        
        //draw
        window.clear(sf::Color(139, 69, 19));
        window.setView(camera);

        //world
        track.Draw(window, false);        
        checkPointManager.Draw(window);

        for (auto& c : cars)
            c.Draw(window);

        //console
        window.setView(window.getDefaultView());
        consoleManager.Draw(window);      
        brains[currentID].Draw(window);

        //minimap
        minimap.Draw(window);
        window.display();
    }

    return 0;
}