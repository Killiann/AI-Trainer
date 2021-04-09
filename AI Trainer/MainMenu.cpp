#include "MainMenu.h"

//navigation
void ForwardNewSim(void* menu) { ((MainMenu*)menu)->NewSim(); }
void ForwardLoadSim(void* menu) { ((MainMenu*)menu)->LoadSim(); }
void ForwardExit(void* menu) { ((MainMenu*)menu)->ExitApp(); }

//new simulation
void ForwardBack(void* menu) { ((MainMenu*)menu)->Back(); }

MainMenu::MainMenu(ResourceManager *resource): resourceManager(resource) {
	background.setPosition(position);
	background.setSize(size);
	background.setFillColor(sf::Color::White);

	title.setPosition(position.x + padding, position.y + padding);
	title.setString("Car Controller Neural Net Trainer");
	title.setFont(*resource->GetRobotoLight());
	title.setFillColor(sf::Color(60, 60, 60));		

	//navigatio
	Button newSim = Button(sf::Vector2f(position.x + padding, position.y + padding + 100), sf::Vector2f(150, 40), resourceManager, "New", ForwardNewSim, this);
	Button loadSim = Button(sf::Vector2f(position.x + padding, position.y + padding + 150), sf::Vector2f(150, 40), resourceManager, "Load", ForwardLoadSim, this);
	Button exitSim = Button(sf::Vector2f(position.x + padding, position.y + padding + 200), sf::Vector2f(150, 40), resourceManager, "Exit", ForwardExit, this);
	
	prompt.setPosition(position.x + newSim.GetSize().x + (padding * 2), position.y + padding + 100);
	prompt.setFont(*resourceManager->GetRobotoLight());
	prompt.setScale(0.55, 0.55);
	prompt.setFillColor(sf::Color(60, 60, 60));		
	 
	
	navigationElements.emplace_back(std::make_shared<Button>(newSim));
	navigationElements.emplace_back(std::make_shared<Button>(loadSim));
	navigationElements.emplace_back(std::make_shared<Button>(exitSim));


	//new simulation 
	Button back = Button(sf::Vector2f(position.x + padding, position.y + padding + 100), sf::Vector2f(150, 40), resourceManager, "Back", ForwardBack, this);
	newSimulationElements.push_back(back);
	loadSimulationElements.push_back(back);
	//load simulation
}

void MainMenu::Update(sf::RenderWindow& window, sf::Event& event) {
	switch (currentState) {
	case(MenuState::Navigation): NavigationState(window, event); break;
	case(MenuState::NewSimulation): NewSimulationState(window, event); break;
	case(MenuState::LoadSimulation): LoadSimulationState(window, event); break;	
	}
}

//menu states (pages)
void MainMenu::NavigationState(sf::RenderWindow& window, sf::Event& event) {
	//main menu on load
	for (unsigned int i = 0; i < navigationElements.size(); ++i) {
		navigationElements[i]->Update(window, event);
		if (navigationElements[i]->IsHovering()) {
			switch (i) {
				//new sim	
			case(0): prompt.setString("Start training a new model with custom parameters."); break;
				//load sim
			case(1): prompt.setString("Continue training a previously created model."); break;
				//exit application
			case(2):prompt.setString("Exit application.");
			}
		}
	}
	//close
	if (exit)
		window.close();
}

void MainMenu::NewSimulationState(sf::RenderWindow& window, sf::Event& event) {
	for (auto& e : newSimulationElements)
		e.Update(window, event);
}

void MainMenu::LoadSimulationState(sf::RenderWindow& window, sf::Event& event) {
	for (auto& e : loadSimulationElements)
		e.Update(window, event);
}

void MainMenu::Draw(sf::RenderTarget& window){
	window.draw(background);
	window.draw(title);

	switch (currentState) {
	case(MenuState::Navigation):
		window.draw(prompt);
		for (auto& n : navigationElements)
			n->Draw(window);
		break;
	case(MenuState::NewSimulation):
		for (auto& n : newSimulationElements)
			n.Draw(window);
		break;
	case(MenuState::LoadSimulation):
		for (auto& n : loadSimulationElements)
			n.Draw(window);
		break;
	}
}