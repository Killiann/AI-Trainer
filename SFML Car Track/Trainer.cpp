#include "Trainer.h"

Trainer::Trainer(ResourceManager* rMngr, ConsoleManager* coMngr, InputManager* iMngr, CheckPointManager* chMngr, Track& t, sf::FloatRect nnDim)
: resourceManager(rMngr), consoleManager(coMngr), inputManager(iMngr), checkPointManager(chMngr), track(t), nnDimensions(nnDim) {
	Setup();
	bestFitness = 0;
	LoadBestCar();
	NextGeneration();
}

void Trainer::Setup() {

	for (int i = 0; i < generationSize; ++i) {
		Car car = Car(i, sf::Vector2f(550.0f, 800.0f), inputManager, consoleManager, resourceManager, *checkPointManager, &track);
		cars.push_back(car);

		//networks
		Network nn(inputNodes, hiddenNodes, outputNodes, nnDimensions);
		networks.push_back(nn);
	}
	cars[currentId].Select();
	timer.restart();
	threads.reserve(10);
	bestNetwork = Network();
	bestFitness = 0;
}

void UpdateRange(std::vector<Car>& cars, std::vector<Network>& networks, float dt, int min, int max) {
	for (int i = min; i < max; ++i) {
		cars[i].Update(dt);
		cars[i].SetInputs(networks[i].FeedForward(cars[i].GetNetworkInput()));
	}
}

void Trainer::Update(float dt) {
	frameCount++;
	if (frameCount == 30){
		SortCars();
		frameCount = 0;
	}

	for (int i = 0; i < generationSize; ++i) {
		cars[i].Update(dt);
		cars[i].SetInputs(networks[i].FeedForward(cars[i].GetNetworkInput()));
	}

	//check if can move on to next gen
	bool allDead = true;
	for (auto& c : cars) 
		if (c.IsAlive() && c.HasStarted() && allDead)
			allDead = false;
	
	if ((allDead && timer.getElapsedTime().asSeconds() > 5) || timer.getElapsedTime().asSeconds() > 90) NextGeneration();

	//consoleManager
	consoleManager->UpdateMessageValue("Generation Size", std::to_string(generationSize));
	consoleManager->UpdateMessageValue("Mutation Rate", "5%");
	consoleManager->UpdateMessageValue("Generation", std::to_string(currentGeneration));
	consoleManager->UpdateMessageValue("Time Limit", std::to_string(generationTime));
	consoleManager->UpdateMessageValue("Best Fitness", std::to_string(bestFitness));
	consoleManager->UpdateMessageValue("Current Time", std::to_string(timer.getElapsedTime().asSeconds()));\
}

void Trainer::DrawWorld(sf::RenderTarget& window) {
	if(cars.size() != 0)
		for (int i = 0; i < generationSize; ++i) {
			cars[i].Draw(window);
		}
}

void Trainer::DrawUI(sf::RenderTarget& window) {
	if(networks.size() != 0)
		networks[currentId].Draw(window);
}

void Trainer::ResetScene() {
	cars.clear();
	networks.clear();
	Setup();
	currentGeneration = 1;
}

void Trainer::NextGeneration() {
	//get top 10% of networks
	SortCars();
	std::vector<Network> bestNetworks = networks;
	bestNetworks.resize(generationSize / 10);

	if (cars[0].GetFitness() > bestFitness) {
		bestFitness = cars[0].GetFitness();
		bestNetwork = bestNetworks[0];
	}

	//add best car so far to pool
	bestNetworks.insert(bestNetworks.begin(), bestNetwork);

	//create 50 similar copies of top 5 with mutation
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(0, 1);

	//reset scene with new networks
 	cars.clear();
	networks.clear();
	//networks.push_back(bestNetworks[)
	for (int i = 0; i < generationSize; ++i) {
		//reset cars
		Car car = Car(i, sf::Vector2f(550.0f, 800.0f), inputManager, consoleManager, resourceManager, *checkPointManager, &track);
		cars.push_back(car);

		//select two of top 5 networks
		int n1(-1), n2(-1);
		while (n1 < 0 || n2 < 0) {
			int selectedNetwork = -1;
			int count = 0;
			while (selectedNetwork == -1 && count < bestNetworks.size()) {
				if (distr(gen) == 1) selectedNetwork = count;
				if (count == bestNetworks.size() - 1) selectedNetwork = count;
				++count;
			}
			n1 == -1 ? n1 = selectedNetwork : n2 = selectedNetwork;
		}

		//new nn
		Network nn(inputNodes, hiddenNodes, outputNodes, nnDimensions);
		//get weight of selected network and muddle up values + create new network using them
		std::vector<lin::Matrix> parentWeights = bestNetworks[n1].GetWeights();
		bool mutate = (distr(gen) == 1);
 		bool crossOver = (distr(gen) == 1);
		crossOver = true;

		for (int i = 0; i < parentWeights.size(); ++i) {
			if (crossOver) {
				parentWeights[i].Add(bestNetworks[n2].GetWeights()[i]);
				parentWeights[i].Map(Divide);
			}
			if(mutate) parentWeights[i].Map(Mutate);
		}
		nn.SetWeights(parentWeights);

		//same with biases
		std::vector<lin::Matrix> parentBiases = bestNetworks[n1].GetBiases();
		for (int i = 0; i < parentBiases.size();++i) {
			if (crossOver) {
				parentBiases[i].Add(bestNetworks[n2].GetBiases()[i]);
				parentBiases[i].Map(Divide);
			}
			if(mutate) parentBiases[i].Map(Mutate);
		}
		nn.SetBiases(parentBiases);
		networks.push_back(nn);
	}
	currentGeneration++;
	timer.restart(); 
}

float Trainer::Mutate(float n){
	std::random_device rd;
	std::mt19937 gen(rd()); 
	std::uniform_int_distribution<> perc(0, 1000);
	//std::uniform_int_distribution<> distr(-100, 100);
	std::uniform_int_distribution<> rnd(-500, 500);
	if ((float)perc(gen) / 1000.f <= 0.005) {
		n = rnd(gen);
	} 

	return n;
}

float Trainer::Divide(float n) {
	return n / 2;
}

void Trainer::SortCars() {
	cars[currentId].Deselect();
	bool sorted = false;
	while (!sorted) {
		sorted = true;
		for (int i = 0; i < cars.size() - 1; ++i) {
			if (cars[i].GetFitness() < cars[i + 1].GetFitness()) {
				std::swap(cars[i], cars[i + 1]);
				std::swap(networks[i], networks[i + 1]);
				sorted = false;
			}
		}
	}
	currentId = 0;
	cars[currentId].Select();
}