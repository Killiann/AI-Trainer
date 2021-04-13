#include <SFML/Graphics.hpp>
#include <numeric>

#include "InputManager.h"
#include "ResourceManager.h"
#include "TrackManager.h"
#include "Trainer.h"
#include "MainMenu.h"
#include "Overlay.h"

#include "ThreadPool.h"

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
        sf::Vector2f(window.getViewport(window.getDefaultView()).width / 2 - 200, 15),
        sf::Vector2f(400, 200)
    );

    //managers 
    ResourceManager resourceManager;
    TrackManager trackManager(&resourceManager);
   
    //NN Trainers
    Trainer trainer(&resourceManager, trackManager.GetCurrentTrack(), dimensions);
    InputManager inputManager(&trainer);          

    //UI
    MainMenu menu(&resourceManager, &trainer);
    Overlay overlay(&resourceManager, &trainer);

    //misc
    std::deque<float> allFPS;
    sf::Vector2f mouseCoords;
    bool displayDev = false;

    //main clock
    sf::Clock clk;
    sf::Time dt;

    //initialise thread pool
    ThreadPool pool(12);
    pool.init();

    //main loop
    while (window.isOpen())
    {
        //time between loops
        dt = clk.restart();
        
        //handle fps
        float fps = 1.f / clk.getElapsedTime().asSeconds();
        allFPS.push_back(fps);
        if (allFPS.size() > 30) allFPS.pop_front();

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        mouseCoords = window.mapPixelToCoords(sf::Mouse::getPosition(window), camera);

        //manage events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized)
            {
                sf::FloatRect visibleArea(0.f, 0.f, (float)event.size.width, (float)event.size.height);
                window.setView(sf::View(visibleArea));
            }                                 

            //update UI
            menu.Update(window, event);
            if(trainer.IsRunning())
                overlay.Update(window, event);

            inputManager.UpdateUIControls(event, mouseCoords);
        }

        //update 
        inputManager.Update();
        trainer.Update(dt.asSeconds(), pool);
        std::string frames = std::to_string((int)(std::accumulate(allFPS.begin(), allFPS.end(), 0.0) / allFPS.size()));
        overlay.UpdateData(frames);

        //draw entities
        window.clear(sf::Color(139, 69, 19));
        window.setView(camera);        
        trackManager.DrawTrack(window, displayDev);
        trainer.DrawEntities(window);

        //UI------- 
        window.setView(window.getDefaultView());        
        trainer.DrawUI(window);        
        menu.Draw(window);        
        overlay.Draw(window);

        window.display();
    }

    pool.shutdown();
    return 0;
}