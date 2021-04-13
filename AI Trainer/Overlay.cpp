#include "Overlay.h"

//nav
void ForwardOptions(void* overlay) {
	((Overlay*)overlay)->Switch(NavItem::Options);
}
void ForwardData(void* overlay) {
	((Overlay*)overlay)->Switch(NavItem::Data);
}

//options
void ForwardMainMenu(void* overlay) {
	((Overlay*)overlay)->OpenMainMenu();
}
void ForwardSave(void* overlay) {
	((Overlay*)overlay)->SaveSim();
}
void ForwardOpExit(void* overlay) {
	((Overlay*)overlay)->ExitSim();
}

Overlay::Overlay(ResourceManager* resource, Trainer* t): resourceManager(resource), trainer(t) {
	background.setPosition(sf::Vector2f(position.x, position.y + navSize.y));
	background.setSize(size);
	background.setFillColor(backgroundColor);	

	Button btn_settings = Button(position, navSize, resourceManager, "Options", ForwardOptions, this);
	Button btn_data = Button(sf::Vector2f(position.x + navSize.x, position.y), navSize, resourceManager, "Data", ForwardData, this);
	navElements.emplace("btn_settings", std::make_shared<Button>(btn_settings));
	navElements.emplace("btn_data", std::make_shared<Button>(btn_data));

	InitOptions();
	InitData();
}

void Overlay::InitOptions() {
	//main menu button	
	Button btn_mainMenu = Button(sf::Vector2f(position.x + padding, position.y + padding + navSize.y), sf::Vector2f(150, 30), resourceManager, "Main Menu", ForwardMainMenu, this);
	optionElements.emplace("btn_mainMenu", std::make_shared<Button>(btn_mainMenu));

	//save
	Button btn_save = Button(sf::Vector2f(position.x + padding, position.y + padding + navSize.y + btnMargin), sf::Vector2f(150, 30), resourceManager, "Save", ForwardSave, this);
	optionElements.emplace("btn_save", std::make_shared<Button>(btn_save));

	//Exit
	Button btn_exit = Button(sf::Vector2f(position.x + padding, position.y + padding + navSize.y + (2* btnMargin)), sf::Vector2f(150, 30), resourceManager, "Exit", ForwardOpExit, this);
	optionElements.emplace("btn_exit", std::make_shared<Button>(btn_exit));
}

void Overlay::InitData() {
	unsigned int leftColW = 350;
	unsigned int rowH = 20;
	unsigned int rowMargin = 25;
	//fps
	Label lbl_fps = Label(sf::Vector2f(position.x + padding, position.y + padding + navSize.y), sf::Vector2f(leftColW, rowH), resourceManager, "FPS: ", 0.5f);
	Label lbl_fps_val = Label(sf::Vector2f(position.x + padding + leftColW, position.y + padding + navSize.y), sf::Vector2f(rowH, rowH), resourceManager, "69", 0.5f);
	dataElements.emplace("lbl_fps", std::make_shared<Label>(lbl_fps));
	dataElements.emplace("lbl_fps_val", std::make_shared<Label>(lbl_fps_val));

	//generation size
	Label lbl_genSize = Label(sf::Vector2f(position.x + padding, position.y + padding + navSize.y + rowMargin), sf::Vector2f(leftColW, rowH), resourceManager, "Generation Size: ", 0.5f);
	Label lbl_genSize_val = Label(sf::Vector2f(position.x + padding + leftColW, position.y + padding + navSize.y + rowMargin), sf::Vector2f(rowH, rowH), resourceManager, "69", 0.5f);
	dataElements.emplace("lbl_genSize", std::make_shared<Label>(lbl_genSize));
	dataElements.emplace("lbl_genSize_val", std::make_shared<Label>(lbl_genSize_val));

	//generation number
	Label lbl_currentGen = Label(sf::Vector2f(position.x + padding, position.y + padding + navSize.y + rowMargin * 2), sf::Vector2f(leftColW, rowH), resourceManager, "Current Generation: ", 0.5f);
	Label lbl_currentGen_val = Label(sf::Vector2f(position.x + padding + leftColW, position.y + padding + navSize.y + rowMargin * 2), sf::Vector2f(rowH, rowH), resourceManager, "69", 0.5f);
	dataElements.emplace("lbl_currentGen", std::make_shared<Label>(lbl_currentGen));
	dataElements.emplace("lbl_currentGen_val", std::make_shared<Label>(lbl_currentGen_val));

	//best lap time
	Label lbl_lapTime = Label(sf::Vector2f(position.x + padding, position.y + padding + navSize.y + rowMargin * 3), sf::Vector2f(leftColW, rowH), resourceManager, "Best Lap Time: ", 0.5f);
	Label lbl_lapTime_val = Label(sf::Vector2f(position.x + padding + leftColW, position.y + padding + navSize.y + rowMargin * 3), sf::Vector2f(rowH, rowH), resourceManager, "69", 0.5f);
	dataElements.emplace("lbl_lapTime", std::make_shared<Label>(lbl_lapTime));
	dataElements.emplace("lbl_lapTime_val", std::make_shared<Label>(lbl_lapTime_val));

	//best lap time(previous generation)
	Label lbl_lapTimePrev = Label(sf::Vector2f(position.x + padding, position.y + padding + navSize.y + rowMargin * 4), sf::Vector2f(leftColW, rowH), resourceManager, "Best Lap time (prev gen): ", 0.5f);
	Label lbl_lapTimePrev_val = Label(sf::Vector2f(position.x + padding + leftColW, position.y + padding + navSize.y + rowMargin * 4), sf::Vector2f(rowH, rowH), resourceManager, "69", 0.5f);
	dataElements.emplace("lbl_lapTimePrev", std::make_shared<Label>(lbl_lapTimePrev));
	dataElements.emplace("lbl_lapTimePrev_val", std::make_shared<Label>(lbl_lapTimePrev_val));

	//best fitness
	Label lbl_fitness = Label(sf::Vector2f(position.x + padding, position.y + padding + navSize.y + rowMargin * 5), sf::Vector2f(leftColW, rowH), resourceManager, "Best Fitness: ", 0.5f);
	Label lbl_fitness_val = Label(sf::Vector2f(position.x + padding + leftColW, position.y + padding + navSize.y + rowMargin * 5), sf::Vector2f(rowH, rowH), resourceManager, "69", 0.5f);
	dataElements.emplace("lbl_fitness", std::make_shared<Label>(lbl_fitness));
	dataElements.emplace("lbl_fitness_val", std::make_shared<Label>(lbl_fitness_val));

	//best fitness(previous generation
	Label lbl_fitnessPrev = Label(sf::Vector2f(position.x + padding, position.y + padding + navSize.y + rowMargin * 6), sf::Vector2f(leftColW, rowH), resourceManager, "Best Fitness (prev gen): ", 0.5f);
	Label lbl_fitnessPrev_val = Label(sf::Vector2f(position.x + padding + leftColW, position.y + padding + navSize.y + rowMargin * 6), sf::Vector2f(rowH, rowH), resourceManager, "69", 0.5f);
	dataElements.emplace("lbl_fitnessPrev", std::make_shared<Label>(lbl_fitnessPrev));
	dataElements.emplace("lbl_fitnessPrev_val", std::make_shared<Label>(lbl_fitnessPrev_val));

	//current generation time
	Label lbl_currentTime = Label(sf::Vector2f(position.x + padding, position.y + padding + navSize.y + rowMargin *7), sf::Vector2f(leftColW, rowH), resourceManager, "Current Gen Time: ", 0.5f);
	Label lbl_currentTime_val = Label(sf::Vector2f(position.x + padding + leftColW, position.y + padding + navSize.y + rowMargin * 7), sf::Vector2f(rowH, rowH), resourceManager, "69", 0.5f);
	dataElements.emplace("lbl_currentTime", std::make_shared<Label>(lbl_currentTime));
	dataElements.emplace("lbl_currentTime_val", std::make_shared<Label>(lbl_currentTime_val));
	
	//total time
	Label lbl_totalTime = Label(sf::Vector2f(position.x + padding, position.y + padding + navSize.y + rowMargin * 8), sf::Vector2f(leftColW, rowH), resourceManager, "Total Run Time: ", 0.5f);
	Label lbl_totalTime_val = Label(sf::Vector2f(position.x + padding + leftColW, position.y + padding + navSize.y + rowMargin * 8), sf::Vector2f(rowH, rowH), resourceManager, "69", 0.5f);
	dataElements.emplace("lbl_totalTime", std::make_shared<Label>(lbl_totalTime));
	dataElements.emplace("lbl_totalTime_val", std::make_shared<Label>(lbl_totalTime_val));

	//hl activation function
	Label lbl_hlActivation = Label(sf::Vector2f(position.x + padding, position.y + padding + navSize.y + rowMargin * 9), sf::Vector2f(leftColW, rowH), resourceManager, "Hidden Layer Activation Function: ", 0.5f);
	Label lbl_hlActivation_val = Label(sf::Vector2f(position.x + padding + leftColW, position.y + padding + navSize.y + rowMargin * 9), sf::Vector2f(rowH, rowH), resourceManager, "69", 0.5f);
	dataElements.emplace("lbl_hlActivation", std::make_shared<Label>(lbl_hlActivation));
	dataElements.emplace("lbl_hlActivation_val", std::make_shared<Label>(lbl_hlActivation_val));

	//output activation function
	Label lbl_olActivation = Label(sf::Vector2f(position.x + padding, position.y + padding + navSize.y + rowMargin * 10), sf::Vector2f(leftColW, rowH), resourceManager, "Output  Layer Activation Function: ", 0.5f);
	Label lbl_olActivation_val = Label(sf::Vector2f(position.x + padding + leftColW, position.y + padding + navSize.y + rowMargin * 10), sf::Vector2f(rowH, rowH), resourceManager, "69", 0.5f);
	dataElements.emplace("lbl_olActivation", std::make_shared<Label>(lbl_olActivation));
	dataElements.emplace("lbl_olActivation_val", std::make_shared<Label>(lbl_olActivation_val));
}

void Overlay::Update(sf::RenderWindow& window, sf::Event &event) {
	//keep nav updating
	for (auto& e : navElements)
		e.second->Update(window, event);

	switch (currentState) {
	case(NavItem::Options):UpdateOptions(window, event); break;
	case(NavItem::Data):UpdateData(window, event); break;
	}
}

void Overlay::UpdateOptions(sf::RenderWindow& window, sf::Event& event) {
	for (auto& e : optionElements)
		e.second->Update(window, event);
}

void Overlay::UpdateData(sf::RenderWindow& window, sf::Event& event) {
	for (auto& e : dataElements)
		e.second->Update(window, event);
}

void Overlay::Draw(sf::RenderTarget &window) {
	if(currentState != NavItem::None)
		window.draw(background);

	for (auto& e : navElements)
		e.second->Draw(window);

	switch (currentState) {
	case(NavItem::Options):
		for (auto& e : optionElements)
			e.second->Draw(window); break;

	case(NavItem::Data):
		for (auto& e : dataElements)
			e.second->Draw(window); break;
	}
}

void Overlay::SetNavColor() {
	(*navElements.find("btn_data")).second->SetColor(sf::Color(40, 40, 40));
	(*navElements.find("btn_settings")).second->SetColor(sf::Color(40, 40, 40));
	switch (currentState) {
	case(NavItem::Options): {
		(*navElements.find("btn_settings")).second->SetColor(selectedColor);
	}break;
	case(NavItem::Data) :{
		(*navElements.find("btn_data")).second->SetColor(selectedColor);
		}break;
	}
}