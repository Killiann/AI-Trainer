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
}