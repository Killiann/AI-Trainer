#pragma once

#include <linearAlgebra.h>
#include <thread>

#include "Car.h"
#include "Network.h"
#include "Track.h"
#include "ThreadPool.h"

class Trainer
{	
	//training settings
	int threadCount;
	int generationSize;
	int currentGeneration = 1;

	sf::Clock timer;
	const float maxGenTime = 40; //seconds

	const int carsPerThread = 50;
	const int surviverPool = 10;
	const float mutationRate = 0.01f;
	const float slightMutationRate = 0.05f;	
	
	//network settings
	int inputNodes = 8;
	std::vector<int> hiddenNodes = { 6, 5 };
	int outputNodes = 5;
	sf::FloatRect nnDimensions;

	//entities
	Track track;
	std::vector<Car> cars;
	std::vector<Network> networks;
	int currentId = 0;

	std::map<float, Network> topPerformers;
	int topPerformersCount = 10;			
	float bestFitness;
	Network bestNetwork;

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

	void Update(float dt, ThreadPool &pool);
	void DrawEntities(sf::RenderTarget& window);
	void DrawUI(sf::RenderTarget& window);	
	void NextGeneration();

	static float Divide(float n);

	inline std::vector<Car>& GetCars() { return cars; }
	inline int GetCurrentID() { return currentId; }
	inline void SetCurrentID(int newID) { currentId = newID; }
	inline void SaveBestCar() { bestNetwork.SaveToFile("best.txt", bestFitness); }
	inline void LoadBestCar() { bestFitness = bestNetwork.LoadFromFile("best.txt"); }	
	inline void ResetScene() { if (generationSize > 0) NewScene(); else std::cout << "Could not reset scene."; }
};

