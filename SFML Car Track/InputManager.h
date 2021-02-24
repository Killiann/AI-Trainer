#pragma once
#include <SFML/Graphics.hpp>
#include "ConsoleManager.h"
class InputManager
{
	float throttle = 0;
	float brake = 0;
	float ebrake = 0;
	float steerLeft = 0;
	float steerRight = 0;	

	int selectedCarID = 0;

	ConsoleManager *consoleManager;
public:
	InputManager(ConsoleManager* c);
	void Update();
	
	float GetThrottle() { return throttle; }
	float GetBrake() { return brake; }
	float GetEBrake() { return ebrake; }
	float GetSteerLeft() { return steerLeft; }
	float GetSteerRight() { return steerRight; }

	void changeSelected(int newID) { selectedCarID = newID; }	
};

