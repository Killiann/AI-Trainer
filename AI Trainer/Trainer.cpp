#include "Trainer.h"

Trainer::Trainer(ResourceManager* rMngr, Track& t, sf::FloatRect nnDim)
: resourceManager(rMngr), track(t), nnDimensions(nnDim) {
	
	////get number of cores in the system and alter generatiion size accordingly
	//threadCount = std::thread::hardware_concurrency();
	//generationSize = (threadCount + 1) * carsPerThread;

	////setup trainer
	//if (generationSize > 0) {
	//	NewScene();
	//	LoadBestCar();
	//	NextGeneration();
	//}
	//else std::cout << "Trainer setup failed: generation size must be bigger than 0." << std::endl;
}

void Trainer::SetupTrainer(int threads, int cars, std::vector<int> hiddenLayers, int hlActivationID, int olActivationID) {
	threadCount = threads;
	carsPerThread = cars;
	hiddenNodes = hiddenLayers;
	hiddenActivationID = hlActivationID;
	outputActivationID = olActivationID;

	generationSize = (threadCount + 1) * carsPerThread;

	NewScene();
}

// initialise new first generation with networks that have random weights and biases
void Trainer::NewScene() {
	cars.clear();
	networks.clear();
	for (int i = 0; i < generationSize; ++i) {
		Car car = Car(i, sf::Vector2f(550.0f, 800.0f), resourceManager, &track);
		cars.push_back(car);

		Network nn(inputNodes, hiddenNodes, outputNodes, nnDimensions, hiddenActivationID, outputActivationID);
		networks.push_back(nn);
	}
	currentId = 0;
	currentGeneration = 1;
	cars[currentId].Select();
	bestNetwork = Network();
	bestFitness = 0;	

	generationTimer.restart(); //start timer
	totalTime.restart();
	running = true;
}

// Update a range of trainer cars / networks. Used to split up the task across threads
void Trainer::UpdateRange(float dt, int min, int max) {
	for (int i = min; i < max; ++i) {
		cars[i].Update(dt);
		cars[i].SetInputs(networks[i].FeedForward(cars[i].GetVision()));
		if (cars[i].HasPassedFinish())currentLapTimes.push_back(cars[i].GetFastestLap());
	}
}

void Trainer::Update(float dt, ThreadPool& pool) {
	if (running) {
		//split generation update mmethods (Car physics + network FeedForward) across threads using threadpool
		std::vector<std::future<void>> results(threadCount);
		int interval = generationSize / (threadCount + 1);
		for (int i = 0; i < threadCount; ++i) {
			auto func = std::bind(&Trainer::UpdateRange, this, dt, interval * i, interval * (i + 1));
			results[i] = pool.submit(func);
		}
		UpdateRange(dt, threadCount * interval, (threadCount + 1) * interval);
		for (int i = 0; i < threadCount; ++i) {
			results[i].get();
		}

		//check if can move on to next generation
		bool allDead = true;
		for (auto& c : cars)
			if (c.IsAlive() && c.HasStarted() && allDead)
				allDead = false;

		if ((allDead && generationTimer.getElapsedTime().asSeconds() > 5) || generationTimer.getElapsedTime().asSeconds() > maxGenTime) NextGeneration();
	}
}

void Trainer::DrawEntities(sf::RenderTarget& window) {
	if (running) {
		if (cars.size() != 0)
			for (auto& c : cars)
				c.Draw(window);
	}
}

void Trainer::DrawUI(sf::RenderTarget& window) {
	if (running) {
		if (networks.size() != 0)
			networks[currentId].Draw(window);
	}
}

void Trainer::NextGeneration() {
	if (running) {
		//get best cars of the generation (capped to survivor pool size + sorted)
		SortCars(0, cars.size() - 1);
		std::vector<Network> bestNetworks = networks;
		bestNetworks.resize(surviverPool);

		//check if best fitness so far has been beaten
		if (cars[0].GetFitness() > bestFitness) {
			bestFitness = cars[0].GetFitness();
			bestNetwork = bestNetworks[0];
		} bestFitnessPrev = cars[0].GetFitness();

		//update fastest lap
		std::sort(currentLapTimes.begin(), currentLapTimes.end());
		if(currentLapTimes.size() > 0) bestLapTimePrev = currentLapTimes[0];
		bestLapTime = std::min(bestLapTime, bestLapTimePrev);
		currentLapTimes.clear();

		//add best network so far to pool
		bestNetworks.insert(bestNetworks.begin(), bestNetwork);

		//reset scene with new networks using bestNetworks[]
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distr(0, 1);

		cars.clear();
		networks.clear();
		for (int i = 0; i < generationSize; ++i) {
			//reset cars
			Car car = Car(i, sf::Vector2f(550.0f, 800.0f), resourceManager, &track);
			cars.push_back(car);

			//select two networks from pool, biased to top performers
			int n1(-1), n2(-1);
			while (n1 < 0 || n2 < 0) {
				int selectedNetwork = -1;
				unsigned int count = 0;
				while (selectedNetwork == -1 && count < bestNetworks.size()) {
					if (distr(gen) == 1) selectedNetwork = count;
					if (count == bestNetworks.size() - 1) selectedNetwork = count;
					++count;
				}
				n1 == -1 ? n1 = selectedNetwork : n2 = selectedNetwork;
			}

			//temp
			bool crossOver = true;
			bool mutate = true;

			//get weights of one of the networks
			Network nn(inputNodes, hiddenNodes, outputNodes, nnDimensions, hiddenActivationID, outputActivationID);
			std::vector<lin::Matrix> parentWeights = bestNetworks[n1].GetWeights();

			for (unsigned int i = 0; i < parentWeights.size(); ++i) {
				//combine weights with other parent and average
				if (crossOver) {
					parentWeights[i].Add(bestNetworks[n2].GetWeights()[i]);
					parentWeights[i].Map(Divide);
				}
				//mutate 
				if (mutate) Mutate(parentWeights[i]);
			}
			nn.SetWeights(parentWeights);

			//same with biases
			std::vector<lin::Matrix> parentBiases = bestNetworks[n1].GetBiases();
			for (unsigned int i = 0; i < parentBiases.size(); ++i) {
				if (crossOver) {
					parentBiases[i].Add(bestNetworks[n2].GetBiases()[i]);
					parentBiases[i].Map(Divide);
				}
				if (mutate) Mutate(parentBiases[i]);
			}
			nn.SetBiases(parentBiases);
			networks.push_back(nn);
		}
		currentGeneration++;
		generationTimer.restart();
	}
}	

//used when averaging weights
float Trainer::Divide(float n) {
	return n / 2;
}

//could not map function to matrix
//as it needs to stay a member function to access local variables
void Trainer::Mutate(lin::Matrix &m){
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> perc(0, 1000);
	std::uniform_int_distribution<> rnd(-100, 100);

	for (unsigned int r = 0; r < m.GetRows(); ++r) {
		for (unsigned int c = 0; c < m.GetCols(); ++c) {						
			//alter value slightly
			if ((float)perc(gen) / 100.f <= slightMutationRate) 		
				m[r][c] += (float)rnd(gen) / 300.f;									

			//randomise value
			if ((float)perc(gen) / 100.f <= mutationRate) 	
				m[r][c] = ((float)rnd(gen) / 100.f) * 5.f; 		
		}
	}	
}

//Quick sort cars
void Trainer::SortCars(int low, int high) {
	if (low < high) {
		//partitioning index
		int pi = Partition(low, high);

		// Separately sort elements before and after partition		
		SortCars(low, pi - 1);
		SortCars(pi + 1, high);
	}
}
//Partition for quick sort
int Trainer::Partition(int low, int high) {
	float pivot = cars[high].GetFitness();
	int i = (low - 1);

	for (int j = low; j <= high - 1; ++j) {
		if (cars[j].GetFitness() > pivot) {
			i++;
			std::swap(cars[i], cars[j]);
			std::swap(networks[i], networks[j]);
		}
	}
	std::swap(cars[i + 1], cars[high]);
	std::swap(networks[i + 1], networks[high]);
	return (i + 1);
}


