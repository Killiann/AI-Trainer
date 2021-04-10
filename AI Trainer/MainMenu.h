#pragma once
#include <SFML/Graphics.hpp>

#include "ResourceManager.h"
#include "Button.h"
#include "Textbox.h"
#include "Dropdown.h"
#include "Label.h"
#include "UIElement.h"

class MainMenu
{
	sf::Vector2f size = sf::Vector2f(900.f, 500.f);
	sf::Vector2f position = sf::Vector2f(300.f, 200.f);
	float padding = 40.f;
	sf::RectangleShape background;
	ResourceManager* resourceManager;

	sf::Text title;

	std::vector<std::shared_ptr<UIElement>> navigationElements;
	std::vector<Button> newSimulationElements;
	std::vector<Button> loadSimulationElements;

	bool exit = false;

	enum class MenuState {
		Navigation,
		NewSimulation,
		LoadSimulation
	};

	MenuState currentState = MenuState::Navigation;

	void NavigationState(sf::RenderWindow& window, sf::Event& event);
	void NewSimulationState(sf::RenderWindow& window, sf::Event& event);
	void LoadSimulationState(sf::RenderWindow& window, sf::Event& event);

public:
	MainMenu(ResourceManager* resource);
	void Update(sf::RenderWindow& window, sf::Event& event);
	void Draw(sf::RenderTarget& window);	

	//navigation
	inline void NewSim() { currentState = MenuState::NewSimulation; }
	inline void LoadSim() { currentState = MenuState::LoadSimulation; }
	inline void ExitApp() { exit = true; }

	//new simulation
	inline void Back() { currentState = MenuState::Navigation; }
};

