#include <linearAlgebra.h>
#include <SFML/Graphics.hpp>
#include <linearAlgebra.h>
#include <algorithm>
#include <numeric>

#include "ConsoleManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "TrackManager.h"
#include "Minimap.h"
#include "Trainer.h"

#include "ThreadPool.h"

#include "MainMenu.h"

//game clock
sf::Clock clk;
sf::Time dt;

std::deque<float> allFPS;
sf::Vector2f mouseCoords;

int main()
{
    //window setup
    sf::ContextSettings settings;    
    settings.antialiasingLevel = 8;    
    sf::RenderWindow window(sf::VideoMode(1500, 900), "Racer AI Trainer", sf::Style::Close, settings);                
    sf::View camera;
    window.setFramerateLimit(90);

    //main view
    camera.setSize(3000, 1800);
    camera.setCenter(1200, 700);
   
    //neural net positioning on UI
    sf::FloatRect dimensions = sf::FloatRect(
        sf::Vector2f(window.getViewport(window.getDefaultView()).width * 0.75f, window.getViewport(window.getDefaultView()).height * 0.25f),
        sf::Vector2f(window.getViewport(window.getDefaultView()).width / 4.f, window.getViewport(window.getDefaultView()).height / 4.f)
    );

    //managers 
    ResourceManager resourceManager;
    ConsoleManager consoleManager(resourceManager.GetConsoleFont());
    TrackManager trackManager(&resourceManager, &consoleManager);
   
    //NN Trainers
    Trainer trainer(&resourceManager, &consoleManager, trackManager.GetCurrentTrack(), dimensions);
    InputManager inputManager(&consoleManager, &trainer);          

    //Minimap setup
    Minimap minimap = Minimap(&trainer.GetCars(), &trackManager.GetCurrentTrack(), &consoleManager);    

    MainMenu menu(&resourceManager);

    //initialise thread pool
    ThreadPool pool(12);
    pool.init();

    //main loop
    while (window.isOpen())
    {     
        //handle fps
        float fps = 1.f / clk.getElapsedTime().asSeconds();
        allFPS.push_back(fps);
        if (allFPS.size() > 30) allFPS.pop_front();
        consoleManager.UpdateMessageValue("framerate", std::to_string((int)(std::accumulate(allFPS.begin(), allFPS.end(), 0.0) / allFPS.size())) + " fps");

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        //sf::Vector2i mousePos = sf::Vector2i(mouseP.x - window.getPosition().x, mouseP.y - window.getPosition().y);
        mouseCoords = window.mapPixelToCoords(sf::Mouse::getPosition(window), camera);

        //time between loops
        dt = clk.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized)
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0.f, 0.f, (float)event.size.width, (float)event.size.height);
                window.setView(sf::View(visibleArea));
            }                                 
                   
            menu.Update(window, event);
            inputManager.UpdateUIControls(event, mouseCoords);
        }

        //update 
        inputManager.Update();
        trainer.Update(dt.asSeconds(), pool);


        //draw entities
        window.clear(sf::Color(139, 69, 19));
        window.setView(camera);
        
        trackManager.DrawTrack(window);
        trainer.DrawEntities(window);

        //UI------- 
        window.setView(window.getDefaultView());
        
        consoleManager.Draw(window);
        minimap.Draw(window);
        trainer.DrawUI(window);

        //temp
        menu.Draw(window);

        window.display();
    }

    pool.shutdown();
    return 0;
}