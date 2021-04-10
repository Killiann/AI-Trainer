#include "MainMenu.h"

//navigation
void ForwardNewSim(void* menu) { ((MainMenu*)menu)->NewSim(); }
void ForwardLoadSim(void* menu) { ((MainMenu*)menu)->LoadSim(); }
void ForwardExit(void* menu) { ((MainMenu*)menu)->ExitApp(); }

//back (Shared)
void ForwardBack(void* menu) { ((MainMenu*)menu)->Back(); }

//new simulation
void ForwardSubLayer(void* menu) { ((MainMenu*)menu)->SubHiddenLayer(); }
void ForwardAddLayer(void* menu) { ((MainMenu*)menu)->AddHiddenLayer(); }

MainMenu::MainMenu(ResourceManager *resource): resourceManager(resource) {
	background.setPosition(position);
	background.setSize(size);
	background.setFillColor(sf::Color::White);

	title.setPosition(position.x + padding, position.y + padding);
	title.setString("Car Controller Neural Net Trainer");
	title.setFont(*resource->GetRobotoLight());
	title.setFillColor(sf::Color(60, 60, 60));		

	//navigation 
	Button btn_newSim = Button(sf::Vector2f(position.x + padding, position.y + padding + 100), sf::Vector2f(150, 40), resourceManager, "New", ForwardNewSim, this);
	Button btn_loadSim = Button(sf::Vector2f(position.x + padding, position.y + padding + 150), sf::Vector2f(150, 40), resourceManager, "Load", ForwardLoadSim, this);
	Button btn_exitSim = Button(sf::Vector2f(position.x + padding, position.y + padding + 200), sf::Vector2f(150, 40), resourceManager, "Exit", ForwardExit, this);
		
	sf::Vector2f promptPos(position.x + btn_newSim.GetSize().x + (padding * 2), position.y + padding + 100);
	Label lbl_prompt = Label(promptPos, sf::Vector2f(0, 0), resource, "", 0.55);
	lbl_prompt.SetFont(resource->GetRobotoLight());	 
	
	navigationElements.emplace_back(std::make_shared<Label>(lbl_prompt));
	navigationElements.emplace_back(std::make_shared<Button>(btn_newSim));
	navigationElements.emplace_back(std::make_shared<Button>(btn_loadSim));
	navigationElements.emplace_back(std::make_shared<Button>(btn_exitSim));	

	//shared back button
	Button back = Button(sf::Vector2f(position.x + padding, position.y + size.y - 40 - padding), sf::Vector2f(150, 40), resourceManager, "Back", ForwardBack, this);
	
	//new simulation 

	unsigned int marginTop = 70;
	unsigned int margin = 40;

	Label lbl_activation = Label(sf::Vector2f(position.x + padding, position.y + padding + marginTop), sf::Vector2f(180, 30), resourceManager, "Activation Functions", 0.5);
	Dropdown dd_hiddenActivation = Dropdown(sf::Vector2f(position.x + padding + 210, position.y + padding + marginTop), sf::Vector2f(150, 30), resourceManager, "Hidden Layers");
	dd_hiddenActivation.AddItem("Sigmoid");
	dd_hiddenActivation.AddItem("Leaky RELU");
	dd_hiddenActivation.AddItem("Binary Step");
	dd_hiddenActivation.AddItem("Tanh");

	Dropdown dd_outputActivation = Dropdown(sf::Vector2f(position.x + padding + 380, position.y + padding + marginTop), sf::Vector2f(150, 30), resourceManager, "Output Layer");
	dd_outputActivation.AddItem("Sigmoid");
	dd_outputActivation.AddItem("Leaky RELU");
	dd_outputActivation.AddItem("Binary Step");
	dd_outputActivation.AddItem("Tanh");

	newSimulationElements.emplace("lbl_activation", std::make_shared<Label>(lbl_activation));
	newSimulationElements.emplace("dd_outputActivation", std::make_shared<Dropdown>(dd_outputActivation));
	newSimulationElements.emplace("dd_hiddenActivation", std::make_shared<Dropdown>(dd_hiddenActivation));

	//threads used
	Label lbl_threadCount = Label(sf::Vector2f(position.x + padding, position.y + padding + marginTop + margin), sf::Vector2f(190, 30), resourceManager, "Number of threads (max:" + std::to_string(std::thread::hardware_concurrency()) + "):", 0.5f);
	Textbox txt_threadInput = Textbox(sf::Vector2f(position.x + padding + 210, position.y + padding + marginTop + margin), sf::Vector2f(30, 30), resourceManager, true, std::thread::hardware_concurrency());
	
	newSimulationElements.emplace("lbl_threadCount", std::make_shared<Label>(lbl_threadCount));
	newSimulationElements.emplace("txt_threadInput", std::make_shared<Textbox>(txt_threadInput));	

	//cars per thread
	Label lbl_carsPerThread = Label(sf::Vector2f(position.x + padding, position.y + padding + marginTop + (margin * 2)), sf::Vector2f(190, 30), resourceManager, "Cars per thread (max:" + std::to_string(maxCarsPerThread) + "):", 0.5f);
	Textbox txt_carsPerThread = Textbox(sf::Vector2f(position.x + padding + 210, position.y + padding + marginTop + (margin * 2)), sf::Vector2f(30, 30), resourceManager, true, maxCarsPerThread);
	
	newSimulationElements.emplace("lbl_carsPerThread", std::make_shared<Label>(lbl_carsPerThread));
	newSimulationElements.emplace("txt_carsPerThread", std::make_shared<Textbox>(txt_carsPerThread));	

	//hidden layers
	Button btn_subLayer = Button(sf::Vector2f(position.x + padding, position.y + padding + marginTop + (3 * margin)), sf::Vector2f(30, 30), resourceManager, "-", ForwardSubLayer, this);
	Button btn_addLayer = Button(sf::Vector2f(position.x + padding + 210, position.y + padding + marginTop + (3 * margin)), sf::Vector2f(30, 30), resourceManager, "+", ForwardAddLayer, this);
	Label lbl_hiddenLayers = Label(sf::Vector2f(position.x + padding + 30, position.y + padding + marginTop + (3 * margin)), sf::Vector2f(180, 30), resourceManager, "Hidden Layers: " + std::to_string(hiddenLayers), 0.5);
	lbl_hiddenLayers.CenterText();
	btn_subLayer.SetFontScale(0.6);
	btn_subLayer.SetFontScale(0.6);
	btn_subLayer.OffsetText(0, -6);			

	newSimulationElements.emplace("btn_subLayer", std::make_shared<Button>(btn_subLayer));
	newSimulationElements.emplace("btn_addLyer", std::make_shared<Button>(btn_addLayer));
	newSimulationElements.emplace("lbl_hiddenLayers", std::make_shared<Label>(lbl_hiddenLayers));

	//for each possible hidden layer
	for (unsigned int i = 0; i < maxHiddenLayers; ++i) {
		Label lbl_layerCount = Label(sf::Vector2f(position.x + padding, position.y + padding + marginTop + ((4 + i) * margin)), sf::Vector2f(190, 30), resourceManager, "Nodes in layer " + std::to_string(i + 1) + ": (max: " + std::to_string(maxNodes) +")", 0.5f);
		Textbox txt_layerCount = Textbox(sf::Vector2f(position.x + padding + 210, position.y + padding + marginTop + ((4 + i) * margin)), sf::Vector2f(30, 30), resourceManager, true, maxNodes);
		newSimulationElements.emplace("lbl_layerCount_" + std::to_string(i), std::make_shared<Label>(lbl_layerCount));
		newSimulationElements.emplace("txt_layerCount_" + std::to_string(i), std::make_shared<Textbox>(txt_layerCount));
	}	
	ResetHLElements();

	//settings
	Label lbl_settingsHeader = Label(sf::Vector2f(position.x + padding + 250, position.y + padding + marginTop + margin), sf::Vector2f(150, 30), resourceManager, "Settings:", 0.6);
	newSimulationElements.emplace("lbl_settingsHeader", std::make_shared<Label>(lbl_settingsHeader));

	newSimulationElements.emplace("btn_back", std::make_shared<Button>(back));

	//load simulation
	loadSimulationElements.push_back(std::make_shared<Button>(back));
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
			case(1): navigationElements[0]->SetText("Start training a new model with custom parameters."); break;
				//load sim
			case(2): navigationElements[0]->SetText("Continue training a previously created model."); break;
				//exit application
			case(3): navigationElements[0]->SetText("Exit application.");
			}
		}
	}
	//close
	if (exit)
		window.close();
}

void MainMenu::NewSimulationState(sf::RenderWindow& window, sf::Event& event) {
	for (const auto& m : newSimulationElements)
		m.second->Update(window, event);
}

void MainMenu::LoadSimulationState(sf::RenderWindow& window, sf::Event& event) {
	for (auto& e : loadSimulationElements)
		e->Update(window, event);
}

void MainMenu::Draw(sf::RenderTarget& window){
	window.draw(background);
	window.draw(title);

	switch (currentState) {
	case(MenuState::Navigation):		
		for (auto& n : navigationElements)
			n->Draw(window);
		break;
	case(MenuState::NewSimulation):
		for (const auto& m : newSimulationElements)
			m.second->Draw(window);
		(*newSimulationElements.find("dd_hiddenActivation")).second->Draw(window);
		(*newSimulationElements.find("dd_outputActivation")).second->Draw(window);
		break;
	case(MenuState::LoadSimulation):
		for (auto& n : loadSimulationElements)
			n->Draw(window);
		break;
	}
}