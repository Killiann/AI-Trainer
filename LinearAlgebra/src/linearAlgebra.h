#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include <random>

namespace lin {

	bool isPointToLeft(sf::Vector2f A, sf::Vector2f B, sf::Vector2f P);
	bool isPointToRight(sf::Vector2f A, sf::Vector2f B, sf::Vector2f P);

	bool doesRectContainPoint(sf::Vector2f P, sf::Vector2f A, sf::Vector2f B, sf::Vector2f C, sf::Vector2f D);
	bool doesConvexShapeContainPoint(sf::Vector2f P, sf::ConvexShape shape);
	/// <summary>
	/// Line Structure in the form Ax + By = C
	/// </summary>
	struct Line {
		sf::Vector2f p1;
		sf::Vector2f p2;
		Line() { A = B = C = 0.f; }
		Line(sf::Vector2f _p1, sf::Vector2f _p2) : p1(_p1), p2(_p2) {
			A = p2.y - p1.y;
			B = p1.x - p2.x;
			C = (A * p1.x) + (B * p1.y);
		}
		inline float GetLength() {
			return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
		}
		float A, B, C;
	};

	//euler const
	const double EULER = std::exp(1.0);
	float sigmoid(float input);

	//Matrices
	class Matrix {
		int rows, cols;
		std::vector<std::vector<float>> data;

	public:

		Matrix(float r, float c);
		void Randomise(float min, float max);
		void Scale(float n);
		void Add(float n);
		void Add(Matrix m2);

		inline int GetRows() { return rows; }
		inline int GetCols() { return cols; }
		inline std::vector<std::vector<float>> GetData() { return data; }
		inline void SetData(std::vector<std::vector<float>> d) { data = d; }

		std::vector<float>& operator[](int i);
		Matrix operator*(float n); //scale
		Matrix operator*(Matrix m2); //multiplyMatrices
		Matrix operator+(float n); //add float
		Matrix operator+(Matrix m2); //add matrix
		void operator=(Matrix m);

	};

	Matrix MultiplyMatrices(Matrix m1, Matrix m2);
	void printMatrix(lin::Matrix m);
}
