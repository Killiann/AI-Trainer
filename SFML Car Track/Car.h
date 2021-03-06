#pragma once
#include <SFML/Graphics.hpp>
#include <linearAlgebra.h>

#include "InputManager.h"
#include "ConsoleManager.h"
#include "ResourceManager.h"
#include "CheckPointManager.h"
#include "CheckPointTracker.h"
#include "Track.h"

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

	const float scale = 17.0f;
	
	//inputs
	bool smoothSteer = true;
	bool safeSteer = true;

	//consts
	const float gravity = 9.81;
	const float mass = 1200;
	const float inertiaScale = 1.0;
	const float halfWidth = 1.2;
	const float cgToFront = 2.0; //cg = center of gravity; 
	const float cgToRear = 2.0;
	const float cgToFrontAxle = 1.25;
	const float cgToRearAxle = 1.25;
	const float cgHeight = 0.55;
	const float wheelRadius = 0.3;
	const float wheelWidth = 0.2;
	const float tireGrip = 4.0;
	const float lockGrip = 0.7;
	const float engineForce = 8000.0;
	const float brakeForce = 12000.0;
	const float eBrakeForce = brakeForce / 2.5;
	const float weightTransfer = 0.2;
	const float maxSteer = 0.4;
	const float cornerStiffnessFront = 10.0;
	const float cornerStiffnessRear = 10.2;
	const float airResist = 2.5;
	const float rollResist = 8.0;

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
	void CheckPointHandling();

	//Distance Lines
	sf::Color lineColor = sf::Color::Black;
	std::vector<sf::VertexArray> distanceLines;
	std::vector<sf::ConvexShape>* trackShapes;
	sf::RectangleShape scanArea;
	const int lineCount = 8;
	const int lineLength = 500;
	
	sf::Font* infoFont;
	std::vector<sf::Text> infoText;

	sf::ConvexShape collisionBounds;
	sf::RectangleShape globalBounds;

	float collisionArea;
	bool selected = false;
	int ID;

	CheckPointTracker checkPointTracker;
	sf::FloatRect nextCheckpointBounds;
public:	
	Car(int id, sf::Vector2f pos, InputManager* input, ConsoleManager *console, ResourceManager *resourceManager, CheckPointManager *checkpointManager, Track* track);
	void Update(float dt);
	void Draw(sf::RenderWindow& window);
	bool containsPoint(sf::Vector2f);

	inline sf::Vector2f getPosition() { return sf::Vector2f(position.x * scale, position.y * scale); }
	inline float GetRotation() { return heading * (180 / M_PI); }		
	inline bool isSelected() { return selected; }
	inline sf::RectangleShape getScanArea() { return scanArea; }
	inline sf::RectangleShape getGlobalBounds() { return globalBounds; }
	inline int getID() { return ID; }
	inline void Select() {
		selected = true;
		collisionBounds.setOutlineColor(sf::Color::Green);
	}
	inline void Deselect() {
		selected = false;
		collisionBounds.setOutlineColor(sf::Color::Red);
	}
};

