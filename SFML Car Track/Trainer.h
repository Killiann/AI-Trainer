#pragma once

#include <linearAlgebra.h>
#include "Car.h"
#include "Network.h"
#include "Track.h"
#include <thread>

struct TrainingCar {
	TrainingCar(Car& c, Network& n) :car(c), brain(n) {
		dna_weights = brain.GetWeights();
		dna_biases = brain.GetBiases();
	}

	Car car;
	Network brain;
	std::vector<lin::Matrix> dna_weights;
	std::vector<lin::Matrix> dna_biases;
};

void UpdateRange(std::vector<Car>& cars, std::vector<Network>& networks, float dt, int min, int max);

class Trainer
{
	//setup
	int currentGeneration = 1;
	int generationSize = 10 * 10; //multiple of 10
	float mutationRate = 0.01;

	int inputNodes = 8;
	std::vector<int> hiddenNodes = { 6, 5 };
	int outputNodes = 5;
	
	sf::Clock timer;
	float generationTime = 10;

	std::vector<Car> cars;
	std::vector<Network> networks;

	std::map<float, Network> topPerformers;
	int topPerformersCount = 10;

	int currentId = 0;

	ResourceManager *resourceManager;
	ConsoleManager *consoleManager;
	InputManager *inputManager;
	CheckPointManager *checkPointManager;
	
	Track track;

	sf::FloatRect nnDimensions;

	void Setup();
	void SortCars();
	//void UpdateRange(float, int, int);

	float bestFitness;
	Network bestNetwork;

	//threading
	std::vector<std::thread> threads;

public:
	Trainer(ResourceManager* rMngr, ConsoleManager* coMngr, InputManager* iMngr, CheckPointManager* chMnger, Track& track, sf::FloatRect nnDim);
	
	void Update(float dt);
	void DrawWorld(sf::RenderTarget& window);
	void DrawUI(sf::RenderTarget& window);
	
	inline std::vector<Car>& GetCars() { return cars; }
	inline int GetCurrentID() { return currentId; }
	inline void SetCurrentID(int newID) { currentId = newID; }

	void ResetScene();
	void NextGeneration();

	static float Mutate(float n);
	static float Divide(float n);
};

