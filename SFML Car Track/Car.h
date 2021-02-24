#pragma once
#include <SFML/Graphics.hpp>
#include "InputManager.h"
#include "ConsoleManager.h"
#include "ResourceManager.h"
#include "Track.h"
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <cassert>

class Car
{
	//physics + rendering
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2f velocity_c;
	sf::Vector2f accel;
	sf::Vector2f accel_c;
	
	float heading = 0.0;
	
	float absVel = 0.0;
	float yawRate = 0.0;
	float steer = 0.0;
	float steerAngle = 0.0;	

	const float scale = 8.0f;
	
	//inputs
	bool smoothSteer = true;
	bool safeSteer = true;

	//consts
	const float gravity = 9.81 * scale;
	const float mass = 500.0 * scale; //og:1200
	const float inertiaScale = 1.0 * scale;
	const float halfWidth = 1.2 * scale;
	const float cgToFront = 2.0 * scale; //cg = center of gravity; 
	const float cgToRear = 2.0 * scale;
	const float cgToFrontAxle = 1.25 * scale;
	const float cgToRearAxle = 1.25 * scale;
	const float cgHeight = 0.55 * scale;
	const float wheelRadius = 0.3 * scale;
	const float wheelWidth = 0.2 * scale;
	const float tireGrip = 3.3; //og:2.0
	const float lockGrip = 0.7; //og:0.7 
	const float engineForce = 50000.0 * scale; //og:8000
	const float brakeForce = 22000.0 * scale;
	const float eBrakeForce =(brakeForce / 2.5);
	const float weightTransfer = 0.2;
	const float maxSteer = 0.9;
	const float cornerStiffnessFront = 7.0;
	const float cornerStiffnessRear = 7.4;
	const float airResist = 5.5; //og:2.5
	const float rollResist = 12; //og:8

	float inertia = mass * inertiaScale;
	float wheelBase = cgToFrontAxle + cgToRearAxle;
	float axleWeightRatioFront = cgToRearAxle / wheelBase;
	float axleWeightRatioRear = cgToFrontAxle / wheelBase;

	//drawables
	//-->Car
	sf::Texture* driftTexture;
	sf::Sprite carBody;
	std::vector<sf::RectangleShape> wheels;
	std::vector<sf::Sprite> skidMarks;	

	//manage
	ConsoleManager* consoleManager;
	InputManager* inputManager;
	ResourceManager* resourceManager;
	Track* track;
	int tileSize;

	//private functions			
	void DoPhysics(float dt);
	float ApplySmoothSteer(float steerInput, float dt);
	float ApplySafeSteer(float steerInput);
	void addSkidMarks();
	void CalculateDistances();

	//Distance Lines
	sf::Color lineColor = sf::Color::Black;
	sf::Color medDistColor = sf::Color(255, 165, 0); //orange
	sf::Color warning = sf::Color::Red;
	std::vector<sf::VertexArray> distanceLines;
	std::vector<sf::ConvexShape>* trackShapes;
	const int lineCount = 8;
	const int lineLength = 300;
	struct Line {
		sf::Vector2f p1;
		sf::Vector2f p2;		
		Line() { A = B = C = 0.f; }
		Line(sf::Vector2f _p1, sf::Vector2f _p2) : p1(_p1), p2(_p2) {
			A = p2.y - p1.y;
			B = p1.x - p2.x;
			C = (A * p1.x) + (B * p1.y);
		}
		float A, B, C;
	};
	sf::Font* infoFont;
	std::vector<sf::Text> infoText;

	bool selected = false;
	int ID;

public:	
	Car(int id, sf::Vector2f pos, InputManager* input, ConsoleManager *console, ResourceManager *resourceManager, Track* track);
	void Update(float dt);
	void Draw(sf::RenderWindow& window);
	sf::Vector2f getPosition() { return position; }
	float GetRotation() { return heading * (180 / M_PI); }	

	void Select() { 			
		selected = true; 
	}
	void Deselect() { selected = false; }
	bool isSelected() { return selected; }
};

