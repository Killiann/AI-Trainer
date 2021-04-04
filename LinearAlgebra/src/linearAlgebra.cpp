#include "linearAlgebra.h"

namespace lin {
	/// <summary>
	/// Checks if point (P) lies to the Left of line (lA, lB)
	/// </summary>
	/// <param name="lA"></param>
	/// <param name="lB"></param>
	/// <param name="P"></param>
	/// <returns></returns>
	bool isPointToLeft(sf::Vector2f lA, sf::Vector2f lB, sf::Vector2f P) {
		return ((lB.x - lA.x) * (P.y - lA.y) - (P.x - lA.x) * (lB.y - lA.y)) > 0;
	}

	/// <summary>
	/// Checks if point (P) lies to the right of line (lA, lB)
	/// </summary>
	/// <param name="lA"></param>
	/// <param name="lB"></param>
	/// <param name="P"></param>
	/// <returns></returns>
	bool isPointToRight(sf::Vector2f lA, sf::Vector2f lB, sf::Vector2f P) {
		return ((lB.x - lA.x) * (P.y - lA.y) - (P.x - lA.x) * (lB.y - lA.y)) < 0;
	}

	/// <summary>
	/// Check whether point (P) lies within rectangle (A, B, C, D)
	/// </summary>
	/// <param name="P"></param>
	/// <param name="rA"></param>
	/// <param name="rB"></param>
	/// <param name="rC"></param>
	/// <param name="rD"></param>
	/// <returns></returns>
	bool doesRectContainPoint(sf::Vector2f P, sf::Vector2f rA, sf::Vector2f rB, sf::Vector2f rC, sf::Vector2f rD) {
		bool AD = isPointToLeft(rA, rD, P);
		bool DC = isPointToLeft(rD, rC, P);
		bool CB = isPointToLeft(rC, rB, P);
		bool BA = isPointToLeft(rB, rA, P);
		return (AD && DC && CB && BA);
	}	

	/// <summary>
	/// Checks whether point (P) lies within ConvexShape(s)
	/// </summary>
	/// <param name="P"></param>
	/// <param name="shape"></param>
	/// <returns></returns>
	bool doesConvexShapeContainPoint(sf::Vector2f P, sf::ConvexShape S) {
		bool res = true;
		for (int i = 0; i < S.getPointCount(); ++i) {
			int a, b;
			if (i == S.getPointCount() - 1) {
				a = i;
				b = 0;
			}
			else {
				a = i;
				b = i + 1;
			}
			if (!isPointToLeft(S.getTransform().transformPoint(S.getPoint(a)), S.getTransform().transformPoint(S.getPoint(b)), P))
			{
				res = false;
				break;
			}
		}
		return res;
	}

	/// <summary>
	/// return the input placed along a sigmoid curve (between 0 and 1)
	/// </summary>
	/// <param name="input"></param>
	/// <returns></returns>
	float sigmoid(float input) {
		return 1 / (1 + std::pow(EULER, -input));
	}
	float leakyRelu(float input)
	{
		return std::max(input * 0.01f, input);
	}

	float binary(float input) {
		return input > 0 ? true : false;
	}

	sf::Vector2f Normalise(const sf::Vector2f& source) {
		float length = sqrt((source.x * source.x) + (source.y * source.y));
		if (length != 0)
			return sf::Vector2f(source.x / length, source.y / length);
		else
			return source;
	}

	//Matrices
	Matrix::Matrix(int r, int c) : rows(r), cols(c) {
		for (int i = 0; i < rows; ++i) {
			data.push_back(std::vector<float>(c));
			for (int j = 0; j < cols; ++j) {
				data[i][j] = 0.f;
			}
		}
	}

	void Matrix::Randomise(float min, float max) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distr(min*10, max*10);

		for (int i = 0; i < rows;  ++i) {
			for (int j = 0; j < cols; ++j) {
				data[i][j] = (float)distr(gen) / 10;
			}
		}
	}

	void Matrix::Scale(float n) {
		for (int i= 0; i < rows; ++i) {
			for (int j= 0 ; j < cols; ++j) {
				data[i][j] *= n;
			}
		}
	}

	void Matrix::Add(float n) {
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				data[i][j] += n;
			}
		}
	}

	void Matrix::Add(Matrix m2) { // only works for matrices of same dimensions r*c
		if (m2.cols == cols && m2.rows == rows);
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				data[i][j] += m2.data[i][j];
			}
		}
	}

	std::vector<float>& Matrix::operator[](int i) {
		if (data.size() > i)
			return data[i];
	}

	Matrix Matrix::operator*(float n) {
		Matrix copy = *this;
		copy.Scale(n);
		return copy;
	}

	Matrix Matrix::operator*(Matrix m2) {
		Matrix res = MultiplyMatrices(*this, m2);
		return res;
	}

	Matrix Matrix::operator+(float n) {
		Matrix copy = *this;
		copy.Add(n);
		return copy;
	}

	Matrix Matrix::operator+(Matrix m2) {
		Matrix copy = *this;
		copy.Add(m2);
		return copy;
	}

	void Matrix::operator=(Matrix m) {
		rows = m.rows;
		cols = m.cols;
		data = m.data;
	}

	void Matrix::Map(float (f)(float n)) {
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				data[i][j] = f(data[i][j]);
			}
		}
	}

	std::vector<float> Matrix::ToVector() {
		std::vector<float> returnVector;
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				returnVector.push_back(data[i][j]);
			}
		}
		return returnVector;
	}

	Matrix ToMatrix(std::vector<float> v) {
		Matrix m(v.size(), 1);
		for (int i = 0; i < v.size(); ++i) {
			m[i][0] = v[i];
		}
		return m;
	}

	//Matrix 
	Matrix MultiplyMatrices(Matrix m1, Matrix m2) {
		try {
			if (m1.GetCols() != m2.GetRows())
				throw "m1.cols and m2.rows should match.";

			Matrix result(m1.GetRows(), m2.GetCols());
			for (int i = 0; i < result.GetRows(); ++i) {
				for (int j = 0; j < result.GetCols(); ++j) {
					float sum = 0;
					for (int k = 0; k < m1.GetCols(); ++k) {
						sum += m1.GetData()[i][k] * m2.GetData()[k][j];
					}
					result[i][j] = sum;
				}
			}
			return result;
		}
		catch (std::string exception){
			std::cout << "LIN: " << exception;
		}
	}

	void printMatrix(lin::Matrix m) {
		std::vector<std::vector<float>> data = m.GetData();
		std::cout << std::endl;
		for (int i = 0; i < m.GetRows(); ++i) {
			std::cout << std::endl;
			for (int j = 0; j < m.GetCols(); ++j) {
				std::cout << data[i][j] << ", ";
			}
		}
	}
}