#ifndef __SPLINE_HPP__
#define __SPLINE_HPP__

#include <vector>
#include <glm/glm.hpp>

/// <summary>
/// Spline system using Catmull-Rom (to implement)
/// </summary>
class Spline {
private:
	std::vector<glm::vec3> points;

public :
	Spline() {}
	Spline(std::vector<glm::vec3> points)
	{
		this->points = points 
	}
	
	/// <summary>
	/// Fonction d'interpolation de Catmull Rom
	/// </summary>
	/// <param name="t">Position de long de la spline</param>
	/// <returns></returns>
	glm::vec3 Interpolate(float t) {
		//trouver l'index i
		int i = (int)t; //floor;
		t = t - floor;

		size_t ptsSize = points.size();
		// verifier si i est dans les limites
		i = i % ptsSize;
		if (i < 0) {
			i = ptsSize - 1;
		}

		//récupération des points pour l'interpolation
		glm::vec3 p0 = points[(i - 1 + ptsSize) % ptsSize];
		glm::vec3 p1 = points[i];
		glm::vec3 p2 = points[(i + 1) % ptsSize];
		glm::vec3 p3 = points[(i + 2) % ptsSize];

		//Interpolation
		return 0.5f * (2.0f * p1 + (p2 - p0) * t + (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t * t + (3.0f * p1 - p0 - 3.0f * p2 + p3) * t * t * t);
	}

	/// <summary>
	/// Retourne la longueur de la spline.
	/// </summary>
	/// <returns>La longueur de la spline.</returns>
	float Length() {
		// Initialisez la longueur totale à 0
		float length = 0.0f;

		// Calculez la longueur entre chaque point de contrôle adjacent
		for (size_t i = 0, max = points.size()-1; i < max; i++) {
			glm::vec3 p0 = points[i];
			glm::vec3 p1 = points[i + 1];
			length += glm::length(p1 - p0);
		}

		// Retournez la longueur totale
		return length;
	}

	/// <summary>
	/// Add a point to the Spline
	/// </summary>
	/// <param name="point">The point to add</param>
	void AddPoint(glm::vec3 point) {
		points.push_back(point);
	}

	/// <summary>
	/// Return the list of points;
	/// </summary>
	/// <returns>The list of points</returns>
	std::vector<glm::vec3> GetPoints() {
		return this->points;
	}

	/// <summary>
	/// Set the list of points.
	/// </summary>
	/// <param name="points">The new list of points</param>
	void SetPoints(std::vector<glm::vec3> points) {
		this->points = points
	}

	/// <summary>
	/// Remove a point.
	/// </summary>
	/// <param name="points">The index to remove</param>
	void RemovePoint(int index) {
		this->points.erase(this->points.begin() + index);
	}

	/// <summary>
	/// Remove all points.
	/// </summary>
	void ClearPoints(int index) {
		this->points.clear();
	}
};

#endif // ! __SPLINE_HPP__
