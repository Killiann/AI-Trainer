#include "linearAlgebra.h"
#include <cassert>

namespace lin {
	/// <summary>
	/// Checks if point (P) lies to the Left of line (lA, lB)
	/// </summary>
	/// <param name="lA">Line point A</param>
	/// <param name="lB">Line point B</param>
	/// <param name="P"> Point P</param>
	/// <returns>Boolean</returns>
	bool isPointToLeft(sf::Vector2f lA, sf::Vector2f lB, sf::Vector2f P) {
		return ((lB.x - lA.x) * (P.y - lA.y) - (P.x - lA.x) * (lB.y - lA.y)) > 0;
	}

	/// <summary>
	/// Checks if point (P) lies to the right of line (lA, lB)
	/// </summary>
	/// <param name="lA">Line point A</param>
	/// <param name="lB">Line point B</param>
	/// <param name="P">Point P</param>
	/// <returns>Boolean</returns>
	bool isPointToRight(sf::Vector2f lA, sf::Vector2f lB, sf::Vector2f P) {
		return ((lB.x - lA.x) * (P.y - lA.y) - (P.x - lA.x) * (lB.y - lA.y)) < 0;
	}

	/// <summary>
	/// Check whether point (P) lies within rectangle (A, B, C, D)
	/// </summary>
	/// <param name="P">Point P</param>
	/// <param name="rA">Rectangle point A</param>
	/// <param name="rB">Rectangle point B</param>
	/// <param name="rC">Rectangle point C</param>
	/// <param name="rD">Rectangle point D</param>
	/// <returns>Boolean</returns>
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
	/// <param name="P">Point P</param>
	/// <param name="S">Shape S</param>
	/// <returns>Boolean</returns>
	bool doesConvexShapeContainPoint(sf::Vector2f P, sf::ConvexShape S) {
		bool res = true;
		for (unsigned int i = 0; i < S.getPointCount(); ++i) {
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
	/// Applies Sigmoid activation function to float n
	/// </summary>
	/// <param name="n">Float n</param>
	/// <returns>Resulting Float</returns>
	float act_sigmoid(float n) {
		return 1.f / (1.f + std::pow(EULER, -n));
	}
	/// <summary>
	/// Applies Leaky RELU activation function to float n
	/// </summary>
	/// <param name="n">Float n</param>
	/// <returns>Resulting Float</returns>
	float act_leakyRelu(float n){
		return std::max(n * 0.01f, n);
	}
	/// <summary>
	/// Applies Binary Step activation function to float n
	/// </summary>
	/// <param name="n">Float n</param>
	/// <returns>Resulting Float</returns>
	float act_binary(float n) {
		return n > 0 ? true : false;
	}
	/// <summary>
	/// Applies Tanh activation function to float n
	/// </summary>
	/// <param name="n">Float n</param>
	/// <returns>Resulting Float</returns>
	float act_tanh(float n) {
		return std::tanh(n);
	}

	/// <summary>
	/// Normalise Vector (Get Direction Vector)
	/// </summary>
	/// <param name="source">Vector</param>
	/// <returns>Normalised Vector</returns>
	sf::Vector2f Normalise(const sf::Vector2f& source) {
		float length = sqrt((source.x * source.x) + (source.y * source.y));
		if (length != 0)
			return sf::Vector2f(source.x / length, source.y / length);
		else
			return source;
	}

	//MATRICES ==========================	
	Matrix::Matrix(unsigned int r, unsigned int c) : rows(r), cols(c) {
		for (unsigned int i = 0; i < rows; ++i) {
			data.push_back(std::vector<float>(c));
			for (unsigned int j = 0; j < cols; ++j) {
				data[i][j] = 0.f;
			}
		}
	}

	/// <summary>
	/// Randomise all values in matrix
	/// </summary>
	/// <param name="min">Range minimum</param>
	/// <param name="max">Range maximum</param>
	void Matrix::Randomise(float min, float max) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distr((int)(min * 10.f), (int)(max * 10.f));

		for (size_t i = 0; i < rows;  ++i) {
			for (unsigned int j = 0; j < cols; ++j) {
				data[i][j] = distr(gen) / 10.f;
			}
		}
	}
	
	/// <summary>
	/// Multiply all values by float n
	/// </summary>
	/// <param name="n">Float n</param>
	void Matrix::Scale(float n) {
		for (unsigned int i= 0; i < rows; ++i) {
			for (unsigned int j= 0 ; j < cols; ++j) {
				data[i][j] *= n;
			}
		}
	}

	/// <summary>
	/// Add n to all values in matrix
	/// </summary>
	/// <param name="n">Float n</param>
	void Matrix::Add(float n) {
		for (unsigned int i = 0; i < rows; ++i) {
			for (unsigned int j = 0; j < cols; ++j) {
				data[i][j] += n;
			}
		}
	}

	/// <summary>
	/// Add values in Matrix m2 to Matrix m1, matrices must have same dimensions
	/// </summary>
	/// <param name="m2">Matrix m2</param>
	void Matrix::Add(Matrix m2) {
		if (m2.cols == cols && m2.rows == rows) {
			for (unsigned int i = 0; i < rows; ++i) {
				for (unsigned int j = 0; j < cols; ++j) {
					data[i][j] += m2.data[i][j];
				}
			}
		}
	}

	std::vector<float>& Matrix::operator[](unsigned int i) {
		if(data.size() > i)
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

	/// <summary>
	/// Map function f to every value in matrix
	/// </summary>
	/// <param name="f">Function F (float (f)(float n))</param>
	void Matrix::Map(float (f)(float n)) {
		for (unsigned int i = 0; i < rows; ++i) {
			for (unsigned int j = 0; j < cols; ++j) {
				data[i][j] = f(data[i][j]);
			}
		}
	}

	/// <summary>
	/// Convert matrix to 1d vector
	/// </summary>
	/// <returns>Newly generated Vector</returns>
	std::vector<float> Matrix::ToVector() {
		std::vector<float> returnVector;
		for (unsigned int i = 0; i < rows; ++i) {
			for (unsigned int j = 0; j < cols; ++j) {
				returnVector.push_back(data[i][j]);
			}
		}
		return returnVector;
	}

	/// <summary>
	/// Convert vector V to Matrix with 1 column
	/// </summary>
	/// <param name="v">Vector of Floats V</param>
	/// <returns>Newly generated Matrix</returns>
	Matrix ToMatrix(std::vector<float> v) {
		Matrix m(v.size(), 1);
		for (unsigned int i = 0; i < v.size(); ++i) {
			m[i][0] = v[i];
		}
		return m;
	}

	/// <summary>
	/// multiply Matrix m1 with Matrix m2, m1.cols must be equal to m2.rows
	/// </summary>
	/// <param name="m1">Matrix m1</param>
	/// <param name="m2">Matrix m2</param>
	/// <returns>Multiplication Result</returns>
	Matrix MultiplyMatrices(Matrix m1, Matrix m2) {
		if (m1.GetCols() == m2.GetRows()) {
			Matrix result(m1.GetRows(), m2.GetCols());
			for (unsigned int i = 0; i < result.GetRows(); ++i) {
				for (unsigned int j = 0; j < result.GetCols(); ++j) {
					float sum = 0;
					for (unsigned int k = 0; k < m1.GetCols(); ++k) {
						sum += m1.GetData()[i][k] * m2.GetData()[k][j];
					}
					result[i][j] = sum;
				}
			}
			return result;
		}
		else return Matrix();
	}

	/// <summary>
	/// Print matrix to console
	/// </summary>
	/// <param name="m">Matrix m</param>
	void printMatrix(lin::Matrix m) {
		std::vector<std::vector<float>> data = m.GetData();
		std::cout << std::endl;
		for (unsigned int i = 0; i < m.GetRows(); ++i) {
			std::cout << std::endl;
			for (unsigned int j = 0; j < m.GetCols(); ++j) {
				std::cout << data[i][j] << ", ";
			}
		}
	}
}