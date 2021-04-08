#include "MainMenu.h"

void NewSim() {
	std::cout << "New\n";
}

void LoadSim() {
	std::cout << "Load\n";
}

void ExitSim() {
	std::cout << "Exit\n";
}

MainMenu::MainMenu(ResourceManager *resource): resourceManager(resource) {
	background.setPosition(position);
	background.setSize(size);
	background.setFillColor(sf::Color::White);

	title.setPosition(position.x + padding, position.y + padding);
	title.setString("Car Controller Neural Net Trainer");
	title.setFont(*resource->GetRobotoLight());
	title.setFillColor(sf::Color(60, 60, 60));		

	Button newSim = Button(sf::Vector2f(position.x + padding, position.y + padding + 100), sf::Vector2f(150, 40), resourceManager, "New", NewSim);
	Button loadSim = Button(sf::Vector2f(position.x + padding, position.y + padding + 150), sf::Vector2f(150, 40), resourceManager, "Load", LoadSim);
	Button exitSim = Button(sf::Vector2f(position.x + padding, position.y + padding + 200), sf::Vector2f(150, 40), resourceManager, "Exit", ExitSim);
	
	prompt.setPosition(position.x + newSim.GetSize().x + (padding * 2), position.y + padding + 100);
	prompt.setFont(*resourceManager->GetRobotoLight());
	prompt.setScale(0.55, 0.55);
	prompt.setFillColor(sf::Color(60, 60, 60));	

	navigation.push_back(newSim);
	navigation.push_back(loadSim);
	navigation.push_back(exitSim);

	dd = Dropdown(sf::Vector2f(position.x + padding, position.y + padding + 250), sf::Vector2f(150, 40), resourceManager, "Test");
	dd.AddItem("Item 1");
	dd.AddItem("Item 2");
	dd.AddItem("Item 3");
}

void MainMenu::Update(sf::RenderWindow& window, sf::Event& event) {
	for (unsigned int i = 0; i < navigation.size();++i) {
		navigation[i].Update(window, event);
		if (navigation[i].IsHovering()) {
			switch (i) {
			//new sim
			case(0): prompt.setString("Start training a new model with custom parameters.");break;
			//load sim
			case(1): prompt.setString("Continue training a previously created model."); break;
			//exit application
			case(2):prompt.setString("Exit application.");
			}
		}
	}
	dd.Update(window, event);
}

void MainMenu::Draw(sf::RenderTarget& window){
	window.draw(background);
	window.draw(title);
	window.draw(prompt);
	for (auto& n : navigation)
		n.Draw(window);
	dd.Draw(window);
}