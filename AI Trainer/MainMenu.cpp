#include "MainMenu.h"

//back (Shared)
void ForwardBack(void* menu) { ((MainMenu*)menu)->Back(); }

//navigation
void ForwardNewSim(void* menu) { ((MainMenu*)menu)->NewSim(); }
void ForwardLoadSim(void* menu) { ((MainMenu*)menu)->LoadSim(); }
void ForwardExit(void* menu) { ((MainMenu*)menu)->ExitApp(); }

//new simulation
void ForwardSubLayer(void* menu) { ((MainMenu*)menu)->SubHiddenLayer(); }
void ForwardAddLayer(void* menu) { ((MainMenu*)menu)->AddHiddenLayer(); }
void ForwardContinue(void* menu) { ((MainMenu*)menu)->CreateNewSim(); }

MainMenu::MainMenu(ResourceManager *resource): resourceManager(resource) {
	//find max available threads in system
	maxThreads = std::thread::hardware_concurrency();

	background.setPosition(position);
	background.setSize(size);
	background.setFillColor(sf::Color::White);

	//Menu title
	title.setPosition(position.x + padding, position.y + padding);
	title.setString("Car Controller Neural Net Trainer");
	title.setFont(*resource->GetRobotoLight());
	title.setFillColor(sf::Color(60, 60, 60));		

	//setup hidden layer data
	hiddenLayerData.assign(maxHiddenLayers, 1);
	
	//initialize pages
	s_marginTop = marginTop + padding + position.y; // styling needs updating with new pos
	InitializeNavigation();
	InitializeNewSimulation();

	//add shared back button
	Button btn_back = Button(sf::Vector2f(position.x + padding, position.y + size.y - 40 - padding), sf::Vector2f(150, 40), resourceManager, "Back", ForwardBack, this);		
	newSimulationElements.emplace("btn_back", std::make_shared<Button>(btn_back));
	loadSimulationElements.emplace("btn_back", std::make_shared<Button>(btn_back));
}

//initialise=================
void MainMenu::InitializeNavigation() {
	//navigation 
	Button btn_newSim = Button(sf::Vector2f(position.x + padding, position.y + padding + 100), sf::Vector2f(150, 40), resourceManager, "New", ForwardNewSim, this);
	Button btn_loadSim = Button(sf::Vector2f(position.x + padding, position.y + padding + 150), sf::Vector2f(150, 40), resourceManager, "Load", ForwardLoadSim, this);
	Button btn_exitSim = Button(sf::Vector2f(position.x + padding, position.y + padding + 200), sf::Vector2f(150, 40), resourceManager, "Exit", ForwardExit, this);

	sf::Vector2f promptPos(position.x + btn_newSim.GetSize().x + (padding * 2), position.y + padding + 100);
	Label lbl_prompt = Label(promptPos, sf::Vector2f(0, 0), resourceManager, "", 0.55);
	lbl_prompt.SetFont(resourceManager->GetRobotoLight());

	navigationElements.emplace("lbl_prompt", std::make_shared<Label>(lbl_prompt));
	navigationElements.emplace("btn_newSim", std::make_shared<Button>(btn_newSim));
	navigationElements.emplace("btn_loadSim", std::make_shared<Button>(btn_loadSim));
	navigationElements.emplace("btn_exitSim", std::make_shared<Button>(btn_exitSim));
}

void MainMenu::InitializeNewSimulation() {
	//threads used
	Label lbl_performance = Label(sf::Vector2f(position.x + padding, position.y + padding + marginTop), sf::Vector2f(190, 30), resourceManager, "Performance:", 0.6);
	Label lbl_threadCount = Label(sf::Vector2f(position.x + padding, position.y + padding + marginTop + margin), sf::Vector2f(190, 30), resourceManager, "Number of threads (max:" + std::to_string(maxThreads) + "):", 0.5f);
	Textbox txt_threadCount = Textbox(sf::Vector2f(position.x + padding + 210, position.y + padding + marginTop + margin), sf::Vector2f(30, 30), resourceManager, true, maxThreads);
	newSimulationElements.emplace("lbl_performance", std::make_shared<Label>(lbl_performance));
	newSimulationElements.emplace("lbl_threadCount", std::make_shared<Label>(lbl_threadCount));
	newSimulationElements.emplace("txt_threadCount", std::make_shared<Textbox>(txt_threadCount));

	//cars per thread
	Label lbl_carsPerThread = Label(sf::Vector2f(position.x + padding, position.y + padding + marginTop + margin * 2), sf::Vector2f(190, 30), resourceManager, "Cars per thread (max:" + std::to_string(maxCarsPerThread) + "):", 0.5f);
	Textbox txt_carsPerThread = Textbox(sf::Vector2f(position.x + padding + 210, position.y + padding + marginTop + margin * 2), sf::Vector2f(30, 30), resourceManager, true, maxCarsPerThread);
	newSimulationElements.emplace("lbl_carsPerThread", std::make_shared<Label>(lbl_carsPerThread));
	newSimulationElements.emplace("txt_carsPerThread", std::make_shared<Textbox>(txt_carsPerThread));

	//activation function selection - hidden layer
	Label lbl_activation = Label(sf::Vector2f(position.x + padding, position.y + padding + marginTop + margin * 3), sf::Vector2f(190, 30), resourceManager, "Activation Functions:", 0.6);
	Dropdown dd_hiddenActivation = Dropdown(sf::Vector2f(position.x + padding, position.y + padding + marginTop + margin * 4), sf::Vector2f(190, 30), resourceManager, "Hidden Layers");
	dd_hiddenActivation.AddItem("Sigmoid");
	dd_hiddenActivation.AddItem("Leaky RELU");
	dd_hiddenActivation.AddItem("Binary Step");
	dd_hiddenActivation.AddItem("Tanh");

	//output layer
	Dropdown dd_outputActivation = Dropdown(sf::Vector2f(position.x + padding, position.y + padding + marginTop + margin * 5), sf::Vector2f(190, 30), resourceManager, "Output Layer");
	dd_outputActivation.AddItem("Sigmoid");
	dd_outputActivation.AddItem("Leaky RELU");
	dd_outputActivation.AddItem("Binary Step");
	dd_outputActivation.AddItem("Tanh");
	newSimulationElements.emplace("lbl_activation", std::make_shared<Label>(lbl_activation));
	newSimulationElements.emplace("dd_outputActivation", std::make_shared<Dropdown>(dd_outputActivation));
	newSimulationElements.emplace("dd_hiddenActivation", std::make_shared<Dropdown>(dd_hiddenActivation));

	//hidden layer setup
	Label lbl_hidden = Label(sf::Vector2f(position.x + padding + marginLeft, position.y + padding + marginTop), sf::Vector2f(190, 30), resourceManager, "Hidden Layer Setup:", 0.6);
	Button btn_subLayer = Button(sf::Vector2f(position.x + padding + marginLeft, position.y + padding + marginTop + margin), sf::Vector2f(30, 30), resourceManager, "-", ForwardSubLayer, this);
	Button btn_addLayer = Button(sf::Vector2f(position.x + padding + marginLeft + 210, position.y + padding + marginTop + margin), sf::Vector2f(30, 30), resourceManager, "+", ForwardAddLayer, this);
	Label lbl_hiddenLayers = Label(sf::Vector2f(position.x + padding + marginLeft + 30, position.y + padding + marginTop + margin), sf::Vector2f(180, 30), resourceManager, "Hidden Layers: " + std::to_string(hiddenLayers), 0.5);
	lbl_hiddenLayers.CenterText();
	btn_subLayer.SetFontScale(0.6);
	btn_subLayer.SetFontScale(0.6);
	btn_subLayer.OffsetText(0, -6);
	newSimulationElements.emplace("lbl_hidden", std::make_shared<Label>(lbl_hidden));
	newSimulationElements.emplace("btn_subLayer", std::make_shared<Button>(btn_subLayer));
	newSimulationElements.emplace("btn_addLyer", std::make_shared<Button>(btn_addLayer));
	newSimulationElements.emplace("lbl_hiddenLayers", std::make_shared<Label>(lbl_hiddenLayers));

	//for each available hidden layer
	for (unsigned int i = 0; i < maxHiddenLayers; ++i) {
		Label lbl_layerCount = Label(sf::Vector2f(position.x + padding + marginLeft, position.y + padding + marginTop + ((2 + i) * margin)), sf::Vector2f(190, 30), resourceManager, "Nodes in layer " + std::to_string(i + 1) + ": (max: " + std::to_string(maxNodes) + ")", 0.5f);
		Textbox txt_layerCount = Textbox(sf::Vector2f(position.x + padding + marginLeft + 210, position.y + padding + marginTop + ((2 + i) * margin)), sf::Vector2f(30, 30), resourceManager, true, maxNodes);
		newSimulationElements.emplace("lbl_layerCount_" + std::to_string(i), std::make_shared<Label>(lbl_layerCount));
		newSimulationElements.emplace("txt_layerCount_" + std::to_string(i), std::make_shared<Textbox>(txt_layerCount));
	}
	UpdateHLElements();

	//Continue Button
	Button btn_continue = Button(sf::Vector2f(position.x + size.x - 150 - padding, position.y + size.y - 40 - padding), sf::Vector2f(150, 40), resourceManager, "Continue", ForwardContinue, this);
	newSimulationElements.emplace("btn_continue", std::make_shared<Button>(btn_continue));

	//settings
	InitalizeSettings();
	UpdateSettings();
}

void MainMenu::InitalizeSettings() {
	//title
	Label lbl_settingsHeader = Label(sf::Vector2f(position.x + padding + s_marginLeft, position.y + padding + marginTop), sf::Vector2f(150, 30), resourceManager, "Settings:", 0.6);
	newSimulationElements.emplace("lbl_settingsHeader", std::make_shared<Label>(lbl_settingsHeader));

	//thread count
	Label lbl_settingsThreadCount = Label(sf::Vector2f(position.x + padding + s_marginLeft, s_marginTop + (settingsMargin * 1)), sf::Vector2f(leftColW, rowH), resourceManager, "Thread Count: ", 0.5);
	Label lbl_settingsThreadCountVal = Label(sf::Vector2f(position.x + padding + s_marginLeft + leftColW, s_marginTop + (settingsMargin * 1)), sf::Vector2f(rightColW, rowH), resourceManager, std::to_string(threadCount), 0.5);
	newSimulationElements.emplace("lbl_settingsThreadCount", std::make_shared<Label>(lbl_settingsThreadCount));
	newSimulationElements.emplace("lbl_settingsThreadCountVal", std::make_shared<Label>(lbl_settingsThreadCountVal));

	//cars per thread
	Label lbl_settingsCarsPerThread = Label(sf::Vector2f(position.x + padding + s_marginLeft, s_marginTop + (settingsMargin * 2)), sf::Vector2f(leftColW, rowH), resourceManager, "Cars per thread: ", 0.5);
	Label lbl_settingsCarsPerThreadVal = Label(sf::Vector2f(position.x + padding + s_marginLeft + leftColW, s_marginTop + (settingsMargin * 2)), sf::Vector2f(rightColW, rowH), resourceManager, std::to_string(carsPerThread), 0.5);
	newSimulationElements.emplace("lbl_settingsCarsPerThread", std::make_shared<Label>(lbl_settingsCarsPerThread));
	newSimulationElements.emplace("lbl_settingsCarsPerThreadVal", std::make_shared<Label>(lbl_settingsCarsPerThreadVal));

	//generation size
	Label lbl_settingsGenSize = Label(sf::Vector2f(position.x + padding + s_marginLeft, s_marginTop + (settingsMargin * 3)), sf::Vector2f(leftColW, rowH), resourceManager, "Generation Size: ", 0.5);
	Label lbl_settingsGenSizeVal = Label(sf::Vector2f(position.x + padding + s_marginLeft + leftColW, s_marginTop + (settingsMargin * 3)), sf::Vector2f(rightColW, rowH), resourceManager, std::to_string(generationSize), 0.5);
	newSimulationElements.emplace("lbl_settingsGenSize", std::make_shared<Label>(lbl_settingsGenSize));
	newSimulationElements.emplace("lbl_settingsGenSizeVal", std::make_shared<Label>(lbl_settingsGenSizeVal));

	//hidden layer function
	Label lbl_hlActivation = Label(sf::Vector2f(position.x + padding + s_marginLeft, s_marginTop + (settingsMargin * 4)), sf::Vector2f(leftColW, rowH), resourceManager, "HL Activation Function:", 0.5);
	Label lbl_hlActivationVal = Label(sf::Vector2f(position.x + padding + s_marginLeft + leftColW, s_marginTop + (settingsMargin * 4)), sf::Vector2f(rightColW, rowH), resourceManager, activationFuncs[hiddenFuncID], 0.5);
	newSimulationElements.emplace("lbl_hlActivation", std::make_shared<Label>(lbl_hlActivation));
	newSimulationElements.emplace("lbl_hlActivationVal", std::make_shared<Label>(lbl_hlActivationVal));

	//output layer function
	Label lbl_olActivation = Label(sf::Vector2f(position.x + padding + s_marginLeft, s_marginTop + (settingsMargin * 5)), sf::Vector2f(leftColW, rowH), resourceManager, "OL Activation Function:", 0.5);
	Label lbl_olActivationVal = Label(sf::Vector2f(position.x + padding + s_marginLeft + leftColW, s_marginTop + (settingsMargin * 5)), sf::Vector2f(rightColW, rowH), resourceManager, activationFuncs[outputFuncID], 0.5);
	newSimulationElements.emplace("lbl_olActivation", std::make_shared<Label>(lbl_olActivation));
	newSimulationElements.emplace("lbl_olActivationVal", std::make_shared<Label>(lbl_olActivationVal));

	//hidden layer
	for (unsigned int i = 0; i < maxHiddenLayers; ++i) {
		Label lbl_hiddenLayer = Label(sf::Vector2f(position.x + padding + s_marginLeft, s_marginTop + (settingsMargin * (6 + i))), sf::Vector2f(leftColW, rowH), resourceManager, "Hidden layer " + std::to_string(i + 1) + " nodes:", 0.5);
		Label lbl_hiddenLayerVal = Label(sf::Vector2f(position.x + padding + s_marginLeft + leftColW, s_marginTop + (settingsMargin * (6 + i))), sf::Vector2f(rightColW, rowH), resourceManager, std::to_string(hiddenLayerData[i]), 0.5);
		newSimulationElements.emplace("lbl_hiddenLayer_" + std::to_string(i), std::make_shared<Label>(lbl_hiddenLayer));
		newSimulationElements.emplace("lbl_hiddenLayerVal_" + std::to_string(i), std::make_shared<Label>(lbl_hiddenLayerVal));
	}
}

//update======================
void MainMenu::Update(sf::RenderWindow& window, sf::Event& event) {
	switch (currentState) {
	case(MenuState::Navigation): NavigationState(window, event); break;
	case(MenuState::NewSimulation): NewSimulationState(window, event); break;
	case(MenuState::LoadSimulation): LoadSimulationState(window, event); break;	
	}
}

void MainMenu::NavigationState(sf::RenderWindow& window, sf::Event& event) {
	//main menu on load
	for (auto&m : navigationElements) {
		m.second->Update(window, event);
		int nameId = -1;
		if (m.first == "btn_newSim") nameId = 0;
		if (m.first == "btn_loadSim") nameId = 1;
		if (m.first == "btn_exitSim") nameId = 2;

		if (m.second->IsHovering()) {
			
			switch (nameId) {
				//new sim	
			case(0): (*navigationElements.find("lbl_prompt")).second->SetText("Start training a new model with custom parameters."); break;
				//load sim
			case(1):(*navigationElements.find("lbl_prompt")).second->SetText("Continue training a previously created model."); break;
				//exit application
			case(2): (*navigationElements.find("lbl_prompt")).second->SetText("Exit application."); break;
			default:break;
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
	UpdateSettings();
}

void MainMenu::LoadSimulationState(sf::RenderWindow& window, sf::Event& event) {
	for (auto& m : loadSimulationElements)
		m.second->Update(window, event);
}

void MainMenu::UpdateSettings() {
	//get thread/ car count values (limit)
	std::string cpt_val = (*newSimulationElements.find("txt_carsPerThread")).second->GetText();
	std::string tc_val = (*newSimulationElements.find("txt_threadCount")).second->GetText();
	if (cpt_val != "") carsPerThread = std::min((unsigned)std::stoi(cpt_val), maxCarsPerThread);
	if (tc_val != "") threadCount = std::min((unsigned)std::stoi(tc_val), maxThreads);

	//get dropdown values
	Dropdown hiddenActivation = *(std::dynamic_pointer_cast<Dropdown> ((*newSimulationElements.find("dd_hiddenActivation")).second));
	Dropdown outputActivation = *(std::dynamic_pointer_cast<Dropdown> ((*newSimulationElements.find("dd_outputActivation")).second));
	hiddenFuncID = hiddenActivation.GetSelected();
	outputFuncID = outputActivation.GetSelected();

	generationSize = threadCount * carsPerThread;

	//update labels
	(*newSimulationElements.find("lbl_settingsThreadCountVal")).second->SetText(std::to_string(threadCount));
	(*newSimulationElements.find("lbl_settingsCarsPerThreadVal")).second->SetText(std::to_string(carsPerThread));
	(*newSimulationElements.find("lbl_settingsGenSizeVal")).second->SetText(std::to_string(generationSize));
	(*newSimulationElements.find("lbl_hlActivationVal")).second->SetText(activationFuncs[hiddenFuncID]);
	(*newSimulationElements.find("lbl_olActivationVal")).second->SetText(activationFuncs[outputFuncID]);

	//hidden layer data
	for (unsigned int i = 0; i < maxHiddenLayers; ++i) {
		std::string layerVal = (*newSimulationElements.find("txt_layerCount_" + std::to_string(i))).second->GetText();
		if (layerVal != "") hiddenLayerData[i] = std::min((unsigned)std::stoi(layerVal), maxNodes);
		(*newSimulationElements.find("lbl_hiddenLayerVal_" + std::to_string(i))).second->SetText(std::to_string(hiddenLayerData[i]));
		if (i >= hiddenLayers) {
			(*newSimulationElements.find("lbl_hiddenLayer_" + std::to_string(i))).second->Hide();
			(*newSimulationElements.find("lbl_hiddenLayerVal_" + std::to_string(i))).second->Hide();
		}
		else {
			(*newSimulationElements.find("lbl_hiddenLayer_" + std::to_string(i))).second->Show();
			(*newSimulationElements.find("lbl_hiddenLayerVal_" + std::to_string(i))).second->Show();
		}
	}
}

//draw========================
void MainMenu::Draw(sf::RenderTarget& window) {
	window.draw(background);
	window.draw(title);

	switch (currentState) {
	case(MenuState::Navigation):
		for (auto& m : navigationElements)
			m.second->Draw(window);
		break;
	case(MenuState::NewSimulation):
		for (const auto& m : newSimulationElements)
			m.second->Draw(window);
		(*newSimulationElements.find("dd_outputActivation")).second->Draw(window);
		(*newSimulationElements.find("dd_hiddenActivation")).second->Draw(window);
		break;
	case(MenuState::LoadSimulation):
		for (auto& m : loadSimulationElements)
			m.second->Draw(window);
		break;
	}
}
