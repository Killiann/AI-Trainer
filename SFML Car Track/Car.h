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

	//Distance Lines
	sf::Color lineColor = sf::Color::Black;
	//sf::Color medDistColor = sf::Color(255, 165, 0); //orange
	//sf::Color warning = sf::Color::Red;
	std::vector<sf::VertexArray> distanceLines;
	std::vector<sf::ConvexShape>* trackShapes;
	sf::RectangleShape checkArea;
	const int lineCount = 8;
	const int lineLength = 500;
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

	sf::ConvexShape collisionBounds;
	float collisionArea;
	bool selected = false;
	int ID;

public:	
	Car(int id, sf::Vector2f pos, InputManager* input, ConsoleManager *console, ResourceManager *resourceManager, Track* track);
	void Update(float dt);
	void Draw(sf::RenderWindow& window);
	bool containsPoint(sf::Vector2f);

	inline sf::Vector2f getPosition() { return sf::Vector2f(position.x * scale, position.y * scale); }
	inline float GetRotation() { return heading * (180 / M_PI); }		
	inline bool isSelected() { return selected; }
	inline sf::RectangleShape getCollisionRect() { return checkArea; }
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

