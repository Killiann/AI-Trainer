#include "InputManager.h"
#include <SFML/Graphics.hpp>

InputManager::InputManager(ConsoleManager *c) : consoleManager(c){
	consoleManager->AddMessage("=======INPUTS=======");
	consoleManager->AddMessage("throttle");
	consoleManager->AddMessage("brake");
	consoleManager->AddMessage("ebrake");
	consoleManager->AddMessage("left steer");
	consoleManager->AddMessage("right steer");
	consoleManager->AddMessage("====================");
}

void InputManager:: Update(){
	//throttle
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		throttle = 1;
	else throttle = 0;

	//brake
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		brake = 1;
	else brake = 0;

	//ebrake
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		ebrake = 1;
	else ebrake = 0;

	//left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		steerLeft = 1;
	else steerLeft = 0;

	//right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		steerRight = 1;
	else steerRight = 0;

	consoleManager->UpdateMessageValue("throttle", std::to_string(throttle));
	consoleManager->UpdateMessageValue("brake", std::to_string(brake));
	consoleManager->UpdateMessageValue("ebrake", std::to_string(ebrake));
	consoleManager->UpdateMessageValue("left steer", std::to_string(steerLeft));
	consoleManager->UpdateMessageValue("right steer", std::to_string(steerRight));
}