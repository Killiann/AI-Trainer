#pragma once

#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <math.h>
#include <algorithm>
#include <cassert>
#include <linearAlgebra.h>

#include "ResourceManager.h"
#include "CheckPointManager.h"
#include "CheckPointTracker.h"
#include "Track.h"

class Car
{ 
	//physics
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2f velocity_c;
	sf::Vector2f accel;
	sf::Vector2f accel_c;

	float I_throttle = 0.f;
	float I_brake = 0.f;
	float I_ebrake = 0.f;
	float I_leftSteer = 0.f;
	float I_rightSteer = 0.f;
	
	float heading = 0.f;
	
	float absVel = 0.f;
	float yawRate = 0.f;
	float steer = 0.f;
	float steerAngle = 0.f;	
	
	//steering settings
	bool smoothSteer = true;
	bool safeSteer = true;

	//simulation settings 
	const float gravity = 9.81f;		//m/s
	const float mass = 1200.f;		//kg
	const float inertiaScale = 1.f;
	const float halfWidth = 1.2f;	//m
	const float cgToFront = 2.f;	//cg = center of gravity; 
	const float cgToRear = 2.f;
	const float cgToFrontAxle = 1.25f;
	const float cgToRearAxle = 1.25f;
	const float cgHeight = 0.55f;
	const float wheelRadius = 0.3f;
	const float wheelWidth = 0.2f;
	const float lockGrip = 0.7f;
	const float engineForce = 8000.0;
	const float brakeForce = 12000.0;
	const float eBrakeForce = brakeForce / 2.5f;
	const float weightTransfer = 0.2f;
	const float maxSteer = 0.4f;
	const float cornerStiffnessFront = 10.f;
	const float cornerStiffnessRear = 10.2f;
	const float airResist = 2.5f;	
	float tireGrip = 4.f;
	float rollResist = 8.f;

	float inertia = mass * inertiaScale;
	float wheelBase = cgToFrontAxle + cgToRearAxle;
	float axleWeightRatioFront = cgToRearAxle / wheelBase;
	float axleWeightRatioRear = cgToFrontAxle / wheelBase;

	//rendering car
	const float scale = 17.f;
	sf::Texture* driftTexture = nullptr;
	sf::Sprite carBody;
	std::vector<sf::RectangleShape> wheels;
	std::vector<sf::Sprite> skidMarks;	

	sf::ConvexShape collisionBounds;
	sf::RectangleShape globalBounds;

	//track info
	Track* track = nullptr;
	unsigned int tileSize = 0;
	std::vector<sf::ConvexShape>* trackShapes = nullptr;

	//Distances to track edge
	const unsigned int lineCount = 7;
	const unsigned int lineLength = 500;
	sf::RectangleShape scanArea; //area scanned for line intersections
	std::vector<float> distances;

	//secondary rendering
	sf::Color lineColor = sf::Color::Black;
	sf::Font* infoFont = nullptr;
	std::vector<sf::VertexArray> distanceLines;					
	std::vector<sf::Text> infoText;

	//checkpoint tracking
	CheckPointTracker checkPointTracker;
	sf::FloatRect nextCheckpointBounds;

	//member info
	int ID = -1;
	bool selected = false;
	bool onTrack = true;
	bool alive = true;
	float fitness = 0.f;
	sf::Clock timeAlive;

	//private functions			
	float ApplySmoothSteer(float steerInput, float dt);
	float ApplySafeSteer(float steerInput);
	float CalculateFitness();
	void DoPhysics(float dt);
	void addSkidMarks();
	void CalculateDistances();
	void CheckPointHandling();

	bool passedFinish = false;

public:	
	Car(){}
	Car(int id, sf::Vector2f pos, ResourceManager *resource, Track* track);
	
	void Update(float dt);
	void Draw(sf::RenderTarget& window, bool devOverlay);
	bool containsPoint(sf::Vector2f);	
	bool IsOnTrack();
	
	void operator=(Car c);

	//gets

	/// <summary>
	/// Get car ID
	/// </summary>
	/// <returns>int, car ID</returns>
	inline int getID() { return ID; }
	/// <summary>
	/// Get line count used for vision
	/// </summary>
	/// <returns>int, line count</returns>
	inline int GetLineCount() { return lineCount; }
	/// <summary>
	/// Get max line length used
	/// </summary>
	/// <returns>int, maximum line size</returns>
	inline int GetMaxLineLength() { return lineLength; }
	/// <summary>
	/// Check if car is alive 
	/// </summary>
	/// <returns>bool, is alive</returns>
	inline bool IsAlive() { return alive; }
	/// <summary>
	/// Check if car has passed start line
	/// </summary>
	/// <returns>bool, has passed start</returns>
	inline bool HasStarted() { return checkPointTracker.Started(); }
	/// <summary>
	/// Check if car is selected
	/// </summary>
	/// <returns>bool, is selected</returns>
	inline bool isSelected() { return selected; }
	/// <summary>
	/// Get car fitness level
	/// </summary>
	/// <returns>float, fitness</returns>
	inline float GetFitness() { return fitness; }	
	/// <summary>
	/// Get car rotation (heading)
	/// </summary>
	/// <returns>float, rotation</returns>
	inline float GetRotation() { return heading * (180.f / (float)M_PI); }		

	/// <summary>
	/// Get distances to sides of track (vision)
	/// </summary>
	/// <returns>std::vector<float> distances</returns>
	inline std::vector<float> GetDistances() { return distances; }	
	/// <summary>
	/// Get current position of car
	/// </summary>
	/// <returns>sf::Vector2f, car position</returns>
	inline sf::Vector2f getPosition() { return sf::Vector2f(position.x * scale, position.y * scale); }
	/// <summary>
	/// Get area of track to scan for finding distances to edges of track
	/// </summary>
	/// <returns>sf::RectangleShape, scan area</returns>
	inline sf::RectangleShape getScanArea() { return scanArea; }
	/// <summary>
	/// Get global bounds of car
	/// </summary>
	/// <returns>sf::RectangleShape, global bounds</returns>
	inline sf::RectangleShape getGlobalBounds() { return globalBounds; }

	//toggle selection
	
	/// <summary>
	/// Select car
	/// </summary>
	inline void Select() {
		selected = true;
		collisionBounds.setOutlineColor(sf::Color::Green);
	}

	/// <summary>
	/// Deselect car
	/// </summary>
	inline void Deselect() {
		selected = false;
		collisionBounds.setOutlineColor(sf::Color::Red);
	}			

	/// <summary>
	/// Get car vision (input for neural network)
	/// </summary>
	/// <returns>std::vector<float>, vision values (distances + absolute velocity)</returns>
	inline std::vector<float> GetVision() {
		std::vector<float> nnInput = distances;
		nnInput.push_back(absVel / 30);
		return nnInput;
	}
	/// <summary>
	/// Set car inputs (taken from neural network)
	/// </summary>
	/// <param name="inputs"></param>
	inline void SetInputs(std::vector<float> inputs) {
		if (inputs.size() > 0) {
			I_throttle = inputs[0];
			I_brake = inputs[1];
			I_ebrake = inputs[2];
			I_rightSteer = inputs[3];
			I_leftSteer = inputs[4];
		}
	}	

	//checkpoint management
	
	/// <summary>
	/// Check if car has passed finish line
	/// </summary>
	/// <returns>bool, has finished</returns>
	inline bool HasPassedFinish() { return passedFinish; }
	/// <summary>
	/// Get last lap time
	/// </summary>
	/// <returns>float, last lap time (ms)</returns>
	inline float GetLastLap() { return checkPointTracker.GetLastLapTime(); }
};

