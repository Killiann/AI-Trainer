#include <linearAlgebra.h>
#include <SFML/Graphics.hpp>
#include <linearAlgebra.h>
#include <algorithm>
#include <numeric>

#include "Track.h"
#include "Car.h"
#include "ConsoleManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "CheckPointManager.h"
#include "Minimap.h"
#include "Trainer.h"

//game clock
sf::Clock clk;
sf::Time dt;
std::vector<float> allFPS;

//key presses
bool isCycleDown = false;
bool isResetDown = false;
bool isNextGenDown = false;
bool isSaveDown = false;

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
    
    //neural net positioning on screen
    sf::FloatRect dimensions = sf::FloatRect(sf::Vector2f(window.getViewport(window.getDefaultView()).width * 0.75, window.getViewport(window.getDefaultView()).height * 0.25),
        sf::Vector2f(window.getViewport(window.getDefaultView()).width / 4, window.getViewport(window.getDefaultView()).height / 4));

    //NN Trainer
    Trainer trainer(&resourceManager, &consoleManager, &inputManager, &checkPointManager, track, dimensions);

    Minimap minimap = Minimap(&trainer.GetCars(), &track, &consoleManager);
    
    //dev
    sf::Vector2i mousePos;
    sf::Vector2f mouseCoords;    

    consoleManager.AddMessage("=========CAR========");
    consoleManager.AddMessage("steer angle");
    consoleManager.AddMessage("velocity.x");
    consoleManager.AddMessage("velocity.y");
    consoleManager.AddMessage("absolute velocity");
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
    consoleManager.AddMessage("========SCENE=======");
    consoleManager.AddMessage("Generation Size");
    consoleManager.AddMessage("Mutation Rate");
    consoleManager.AddMessage("Generation");
    consoleManager.AddMessage("Current Time");
    consoleManager.AddMessage("Time Limit");
    consoleManager.AddMessage("Best Fitness");

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
                    for (int i = 0; i < trainer.GetCars().size(); ++i) {
                        if (trainer.GetCars()[i].containsPoint(mouseCoords)) {
                            if (i != trainer.GetCurrentID()) {
                                trainer.GetCars()[i].Select();
                                trainer.GetCars()[trainer.GetCurrentID()].Deselect();
                                trainer.SetCurrentID(i);
                            }
                        }
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                //cycle through cars
                if (event.key.code == sf::Keyboard::Key::Num1) {
                    if (!isCycleDown) {
                        isCycleDown = true;
                        trainer.GetCars()[trainer.GetCurrentID()].Deselect();
                        if (trainer.GetCurrentID() == (trainer.GetCars().size() - 1))
                            trainer.SetCurrentID(0);
                        else trainer.SetCurrentID(trainer.GetCurrentID() + 1);
                        trainer.GetCars()[trainer.GetCurrentID()].Select();
                    }
                }

                //reset scene
                if (event.key.code == sf::Keyboard::Key::Num0) {
                    if (!isResetDown) {
                        trainer.ResetScene();
                        isResetDown = true;
                    }
                }
                //reset scene
                if (event.key.code == sf::Keyboard::Key::Num2) {
                    if (!isNextGenDown) {
                        trainer.NextGeneration();
                        isNextGenDown = true;
                    }
                }

                //save best car
                if (event.key.code == sf::Keyboard::Key::S) {
                    if (!isSaveDown) {
                        trainer.SaveBestCar();
                    }
                }
            }
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Key::Num1) {
                    if (isCycleDown) isCycleDown = false;
                }
                if (event.key.code == sf::Keyboard::Key::Num0) {
                    if (isResetDown) isResetDown = false;
                }
                if (event.key.code == sf::Keyboard::Key::Num2) {
                    if (isNextGenDown) isNextGenDown = false;
                }
                if (event.key.code == sf::Keyboard::Key::S) {
                    if (isSaveDown) isSaveDown = false;
                }
            }

                   
        }

        //update 
        inputManager.Update();
        consoleManager.Update();

        //update trainer (cars + NN)
        trainer.Update(dt.asSeconds());
        camera.setCenter((int)trainer.GetCars()[trainer.GetCurrentID()].getPosition().x, (int)trainer.GetCars()[trainer.GetCurrentID()].getPosition().y);
        
        //draw
        window.clear(sf::Color(139, 69, 19));
        window.setView(camera);

        //world
        track.Draw(window, false);        
        checkPointManager.Draw(window);

        trainer.DrawWorld(window);

        //console 
        window.setView(window.getDefaultView());
        consoleManager.Draw(window);      

        //minimap+ Neural net
        minimap.Draw(window);
        trainer.DrawUI(window);

        window.display();
    }

    return 0;
}