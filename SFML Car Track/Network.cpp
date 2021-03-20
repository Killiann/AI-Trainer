#include "Network.h"

Network::Network(int inputLayer, std::vector<int> hiddenLayers, int outputLayer, sf::FloatRect d) : dimensions(d) {
	if (hiddenLayers.size() > 0) {
		//setup nodes
		nodes_i = lin::Matrix(inputLayer, 1);
		nodes_o = lin::Matrix(outputLayer, 1);
		for (int i = 0; i < hiddenLayers.size(); ++i) {
			nodes_hl.emplace_back(hiddenLayers[i], 1);
		}

		//setup weights
		weights_ih = lin::Matrix(hiddenLayers[0], inputLayer);
		weights_ho = lin::Matrix(outputLayer, hiddenLayers[hiddenLayers.size() - 1]);
		weights_ih.Randomise(-1, 1);
		weights_ho.Randomise(-1, 1);
		if (hiddenLayers.size() > 1) {
			for (int i = 0; i < hiddenLayers.size() - 1; ++i) {
				//create hidden layer weights
				weights_hl.emplace_back(hiddenLayers[i + 1], hiddenLayers[i]);
				weights_hl[i].Randomise(-1, 1);
			}

			//create hidden layer biases 
			for (int i = 0; i < hiddenLayers.size(); ++i) {
				biases_hl.emplace_back(hiddenLayers[i], 1);
				biases_hl[i].Randomise(-1, 1);
			}
		}
		biases_o = lin::Matrix(outputLayer, 1);
		biases_o.Randomise(-1, 1);	

		//SFML
		SetupRendering();
	}
	else std::cout << "Need more than 0 hidden layers in network.";
}

std::vector<float> Network::FeedForward(std::vector<float> inputs) {
	//convert inputs to matrix to use to multiply
	if (inputs.size() == nodes_i.GetRows()) {
		lin::Matrix inputMatrix = lin::ToMatrix(inputs);
		nodes_i = inputMatrix;

		//activate first layer of hidden layer
		nodes_hl[0] = lin::MultiplyMatrices(weights_ih, inputMatrix);
		nodes_hl[0].Add(biases_hl[0]);
		nodes_hl[0].Map(lin::sigmoid);

		//activate rest of hidden layer nodes
		for (int i = 1; i < nodes_hl.size(); ++i) {
			nodes_hl[i] = lin::MultiplyMatrices(weights_hl[i - 1], nodes_hl[i - 1]);
			nodes_hl[i].Add(biases_hl[i]);
			nodes_hl[i].Map(lin::sigmoid);
		}

		//activate output layer
		nodes_o = lin::MultiplyMatrices(weights_ho, nodes_hl[nodes_hl.size() - 1]);
		nodes_o.Add(biases_o);
		nodes_o.Map(lin::sigmoid);

		//SFML
		UpdateRender();
	}
	else std::cout << "Could not feed forward network. Incorrect number of inputs.\n";
	return nodes_o.ToVector();
}

void Network::Draw(sf::RenderTarget& window) {

	for (auto l : connectionLines)
		window.draw(l);

	for (auto c : inputNodes)
		window.draw(c);

	for (int i = 0; i < hiddenNodes.size(); ++i) {
		for (auto n : hiddenNodes[i])
			window.draw(n);
	}

	for (auto o : outputNodes)
		window.draw(o);
}

void Network::SetupRendering() {
	sf::CircleShape circle;
	int radius = 13;
	circle.setRadius(radius);
	circle.setOrigin(radius, radius);
	circle.setFillColor(sf::Color::White);

	float xInterval = dimensions.width / (nodes_hl.size() + 3);
	float yInterval = radius * 3;

	float padding;

	//hidden nodes
	int nodeCount = 0;
	for (int j = 0; j < nodes_hl.size(); ++j) {
		nodeCount += nodes_hl[j].GetRows();
		hiddenNodes.push_back(std::vector<sf::CircleShape>());
		
		padding = (dimensions.height - ((yInterval * nodes_hl[j].GetRows()) - yInterval)) / 2;
		for (int i = 0; i < nodes_hl[j].GetRows(); ++i) {
			circle.setPosition(sf::Vector2f(dimensions.left + xInterval * (j + 2), dimensions.top + padding + (i * yInterval)));
			hiddenNodes[j].push_back(circle);
		}
	}

	//hidden node weights
	for (int j = 0; j < hiddenNodes.size() - 1; ++j) { //loop through layers
		for (int i = 0; i < hiddenNodes[j].size(); ++i) { //loop through nodes in layer
			for (int r = 0; r < hiddenNodes[j + 1].size(); ++r) { //loop through nodes in next layer
				sf::VertexArray connection = CreateLine(hiddenNodes[j][i].getPosition(), hiddenNodes[j + 1][r].getPosition(), weights_hl[j][r][i]);
				connectionLines.push_back(connection);
			}
		}
	}
	
	//output nodes
	padding = (dimensions.height - ((yInterval * nodes_o.GetRows()) - yInterval)) / 2;
	for (int i = 0; i < nodes_o.GetRows(); ++i) {
		circle.setPosition(sf::Vector2f(dimensions.left + xInterval * (2 + nodes_hl.size()), dimensions.top + padding + (i * yInterval)));
		outputNodes.push_back(circle);

		//weights
		for (int j = 0; j < hiddenNodes[hiddenNodes.size() - 1].size(); ++j) {
			sf::VertexArray connection = CreateLine(hiddenNodes[hiddenNodes.size() - 1][j].getPosition(), circle.getPosition(), weights_ho[i][j]);
			connectionLines.push_back(connection);
		}
	}

	//input nodes
	padding = (dimensions.height - ((yInterval * nodes_i.GetRows()) - yInterval)) / 2;
	for (int i = 0; i < nodes_i.GetRows(); ++i) {
		circle.setPosition(sf::Vector2f(dimensions.left + xInterval, dimensions.top + padding + (i * yInterval)));
		inputNodes.push_back(circle);
		
		//weights
		for (int r = 0; r < weights_ih.GetRows(); ++r) {
			sf::VertexArray connection = CreateLine(circle.getPosition(), hiddenNodes[0][r].getPosition(), weights_ih[r][i]);
			connectionLines.push_back(connection);
		}
	}
}

//update colour of nodes
void Network::UpdateRender() {
	//input nodes
	for (int i = 0; i < inputNodes.size(); ++i) {
		int cv = std::min(nodes_i[i][0] * 255, 255.f);
		inputNodes[i].setFillColor(sf::Color(cv, cv, cv));
	}

	//hidden layers 
	for (int i = 0; i < hiddenNodes.size(); ++i) {
		for (int j = 0; j < hiddenNodes[i].size(); ++j) {
			int cv = std::min(nodes_hl[i][j][0] * 255, 255.f);
			hiddenNodes[i][j].setFillColor(sf::Color(cv, cv, cv));
		}
	}

	//output layer
	for (int i = 0; i < outputNodes.size(); ++i) {
		int cv = std::min(nodes_o[i][0] * 255, 255.f);
		outputNodes[i].setFillColor(sf::Color(cv, cv, cv));
	}
}

//create weighted Line
sf::VertexArray Network::CreateLine(sf::Vector2f p1, sf::Vector2f p2, float weight) {
	sf::Color lineColor = weight < 0 ? sf::Color::Red : sf::Color::Green;
	weight *= 1.5;
	sf::Vector2f line = p2 - p1;
	sf::Vector2f normal(-line.y, line.x);
	normal = lin::Normalise(normal);
	sf::VertexArray returnShape(sf::TriangleStrip, 4);
	returnShape[0].position = p1 - weight * normal;
	returnShape[1].position = p1 + weight * normal;
	returnShape[2].position = p2 - weight * normal;
	returnShape[3].position = p2 + weight * normal;

	for (int i = 0; i < returnShape.getVertexCount(); ++i)
		returnShape[i].color = lineColor;

	return returnShape;
}