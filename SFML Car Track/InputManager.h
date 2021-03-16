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

	std::vector<float> GetCarInputs() {
		std::vector<float> inputs;
 		inputs.push_back(throttle);
		inputs.push_back(brake);
		inputs.push_back(ebrake);
		inputs.push_back(steerRight);
		inputs.push_back(steerLeft);
		return inputs;
	}

	void SetThrottle(float newThottle) { throttle = newThottle; }
	void SetBrake(float newBrake) { brake = newBrake; }
	void SetEBrake(float newEBrake) { ebrake = newEBrake; }
	void SetSteerLeft(float newSteerLeft) { steerLeft = newSteerLeft; }
	void SetSteerRight(float newSteerRight) { steerRight = newSteerRight; }

	void changeSelected(int newID) { selectedCarID = newID; }	
};

