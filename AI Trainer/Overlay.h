#pragma once
#include <iomanip>
#include <sstream>
#include "UIElement.h"
#include "Trainer.h"
#include "Button.h"
#include "Label.h"

enum class NavItem {
	None,
	Options,
	Data
};

class Overlay
{	
	NavItem currentState = NavItem::None;

	ResourceManager* resourceManager = nullptr;
	Trainer* trainer = nullptr;

	const sf::Vector2f navSize = sf::Vector2f(150, 30);
	const sf::Vector2f position = sf::Vector2f(0,0);
	const sf::Vector2f size = sf::Vector2f(500, 320);

	sf::RectangleShape background;
	sf::Color backgroundColor = sf::Color(255, 255, 255);

	std::map<std::string, std::shared_ptr<UIElement>> navElements;
	std::map<std::string, std::shared_ptr<UIElement>> optionElements;
	std::map<std::string, std::shared_ptr<UIElement>> dataElements;

	sf::Color selectedColor = sf::Color(80, 80, 80);

	//styling
	unsigned int padding = 20;
	unsigned int btnMargin = 40;

	void InitOptions();
	void InitData();
	void UpdateOptions(sf::RenderWindow& window, sf::Event& event);	
	void SetNavColor();

	std::string FloatToTime(float n);
	std::string TruncateFloat(float n);
public :
	Overlay(){}
	Overlay(ResourceManager *resource, Trainer *t);
	void Update(sf::RenderWindow& window, sf::Event& event);
	void UpdateData(std::string fps);
	void Draw(sf::RenderTarget& window);	
	
	inline void Switch(NavItem screen){
		if (currentState == screen) currentState = NavItem::None;
		else currentState = screen;
		SetNavColor();
	}	
	inline void OpenMainMenu() { std::cout << "open mm\n"; }
	inline void SaveSim() { std::cout << "save sim\n"; }
	inline void ExitSim() { std::cout << "exit sim\n"; }
};

