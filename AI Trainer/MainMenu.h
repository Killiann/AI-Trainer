#pragma once
#include <SFML/Graphics.hpp>
#include <thread>

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
	float padding = 35.f;
	sf::RectangleShape background;
	ResourceManager* resourceManager;
	
	unsigned int hiddenLayers = 1;
	
	unsigned int maxHiddenLayers = 4;
	unsigned int minHiddenLayers = 1;
	unsigned int maxCarsPerThread = 50;
	unsigned int maxNodes = 10;

	sf::Text title;

	std::map <std::string, std::shared_ptr<UIElement>> newSimulationElements;
	std::vector<std::shared_ptr<UIElement>> navigationElements;
	std::vector<std::shared_ptr<UIElement>> loadSimulationElements;

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

	inline void ResetHLElements() {
		for (unsigned int i = 0; i < maxHiddenLayers; ++i) {
			(*newSimulationElements.find("lbl_layerCount_" + std::to_string(i))).second->Hide();
			(*newSimulationElements.find("txt_layerCount_" + std::to_string(i))).second->Hide();
		}
		for (unsigned int i = 0; i < hiddenLayers; ++i) {
			(*newSimulationElements.find("lbl_layerCount_" + std::to_string(i))).second->Show();
			(*newSimulationElements.find("txt_layerCount_" + std::to_string(i))).second->Show();
		}
	}

public:
	MainMenu(ResourceManager* resource);
	void Update(sf::RenderWindow& window, sf::Event& event);
	void Draw(sf::RenderTarget& window);	

	//navigation
	inline void NewSim() { currentState = MenuState::NewSimulation; }
	inline void LoadSim() { currentState = MenuState::LoadSimulation; }
	inline void ExitApp() { exit = true; }

	//new simulation
	//new sim
	inline void AddHiddenLayer() { 
		if (hiddenLayers < maxHiddenLayers) ++hiddenLayers; 
		(*newSimulationElements.find("lbl_hiddenLayers")).second->SetTextString("Hidden Layers: " + std::to_string(hiddenLayers));
		ResetHLElements();
	}
	inline void SubHiddenLayer() {
		if (hiddenLayers > minHiddenLayers) --hiddenLayers;
		(*newSimulationElements.find("lbl_hiddenLayers")).second->SetTextString("Hidden Layers: " + std::to_string(hiddenLayers));
		ResetHLElements();
 	}
	inline void Back() { currentState = MenuState::Navigation; }
};

