#pragma once

#include <linearAlgebra.h>
#include <thread>

#include "Car.h"
#include "Network.h"
#include "Track.h"
#include "ThreadPool.h"

struct TrainerData {
	TrainerData() {}
	unsigned int generationSize = 0;
	unsigned int currentGeneration = 0;
	float bestLapTime = 0.f; //ms
	float bestLapTimePrev = 0.f; //ms
	float bestFitness = 0.f;
	float bestFitnessPrev = 0.f;
	float currentTime = 0.f;
	float totalTime = 0.f;
	std::string hlActivation;
	std::string olActivation;
};

class Trainer
{	
	std::vector<std::string> activationFuncs{ "Sigmoid", "Leaky RELU", "Binary Step", "Tanh" };
	unsigned int hiddenActivationID = 0;
	unsigned int outputActivationID = 0;

	//training settings
	int threadCount = 1;
	int carsPerThread = 50;
	int generationSize = 50;
	int currentGeneration = 1;
	bool running = false;

	sf::Clock totalTime;
	sf::Clock generationTimer;
	const float maxGenTime = 40; //seconds

	const int surviverPool = 10;
	const float mutationRate = 0.01f;
	const float slightMutationRate = 0.05f;	
	
	//network settings
	const int inputNodes = 8;
	const int outputNodes = 5;
	std::vector<int> hiddenNodes = { 6, 5 };	
	sf::FloatRect nnDimensions;

	//entities
	Track track;
	std::vector<Car> cars;
	std::vector<Network> networks;
	int currentId = 0;

	std::map<float, Network> topPerformers;
	int topPerformersCount = 10;				
	Network bestNetwork;

	//data
	std::vector<float> currentLapTimes;
	float bestLapTime = 0.f;
	float bestLapTimePrev = 0.f;
	float bestFitness = 0.f;
	float bestFitnessPrev = 0.f;

	//private funcs
	void NewScene();
	void UpdateRange(float, int, int);
	void Mutate(lin::Matrix& m);	
	
	//quick sort
	int Partition(int low, int high);
	void SortCars(int low, int high);

	//managers
	ResourceManager* resourceManager;
	ConsoleManager* consoleManager;
	
public:
	Trainer(ResourceManager* rMngr, ConsoleManager* coMngr, Track& track, sf::FloatRect nnDim);	
	void SetupTrainer(int threadCount, int carsPerThread, std::vector<int> hiddenLayers, int hlActivationID, int olActivationID);

	void Update(float dt, ThreadPool &pool);
	void DrawEntities(sf::RenderTarget& window);
	void DrawUI(sf::RenderTarget& window);	
	void NextGeneration();

	static float Divide(float n);

	inline bool IsRunning() { return running; }
	inline std::vector<Car>& GetCars() { return cars; }
	inline int GetCurrentID() { return currentId; }
	inline void SetCurrentID(int newID) { currentId = newID; }
	inline void SaveBestCar() { bestNetwork.SaveToFile("best.txt", bestFitness); }
	inline void LoadBestCar() { bestFitness = bestNetwork.LoadFromFile("best.txt"); }	
	inline void ResetScene() { if (generationSize > 0) NewScene(); else std::cout << "Could not reset scene."; }

	//get data for UI
	inline TrainerData GetData() {
		TrainerData tData;
		tData.generationSize = generationSize;
		tData.currentGeneration = currentGeneration;
		tData.bestLapTime = bestLapTime;
		tData.bestLapTimePrev = bestLapTimePrev;
		tData.bestFitness = bestFitness;
		tData.bestFitnessPrev = bestFitnessPrev;
		tData.currentTime = generationTimer.getElapsedTime().asMilliseconds();
		tData.totalTime = totalTime.getElapsedTime().asMilliseconds();
		tData.hlActivation = activationFuncs[hiddenActivationID];
		tData.olActivation = activationFuncs[outputActivationID];
		return tData;
	}
};

