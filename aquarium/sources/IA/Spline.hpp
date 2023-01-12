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
	std::vector<float> keyframeTime;
	float totalLength;
	bool cyclic;
public:
	Spline() {}
	Spline(std::vector<glm::vec3> points, bool cyclic = true)
	{
		this->points = points;
		this->cyclic = cyclic;

		if (cyclic) {
			this->points.push_back(points[0]);
		}
		else {
			this->points.insert(this->points.begin(), this->points[0]);
			this->points.push_back(points[points.size() - 1]);
		}

		ComputeKeyframeTimes();
	}

	/// <summary>
	/// Use a given progression to return the current points with his own progression.
	/// </summary>
	/// <param name="progression"></param>
	/// <returns>Return the pair of points index and his progression</returns>
	std::pair<size_t, float> FindSegment(float progression) {
		size_t index = 0;
		while (index < keyframeTime.size() - 2 && keyframeTime[index + 1] <= progression)
			index++;

		if (cyclic) {
			index = index % keyframeTime.size();
		}
		float time = (progression - keyframeTime[index]) / (keyframeTime[(index + 1) % keyframeTime.size()] - keyframeTime[index]);

		return { index,time };
	}

	/// <summary>
	/// Catmull-Rom function, give the current position on the spline using a progression value between 0 and 1.
	/// </summary>
	/// <param name="t">Progression on the spline (between 0 and 1)</param>
	/// <returns>The 3D coordinate of the progression on the spline.</returns>
	glm::vec3 Interpolate(float progression) {
		auto [index, time] = FindSegment(progression);

		if (cyclic) {
			index = index % points.size();
		}
		else {
			if (index == 0) index++;
			if (index == points.size() - 1) index--;
		}
		return 0.5f * ((2.f * points[index]) + //Control Point 0
			(-points[(index - 1 + points.size()) % points.size()] + points[(index + 1) % points.size()]) * time + //Control Point 1
			(2.f * points[(index - 1 + points.size()) % points.size()] - 5.f * points[index] + 4.f * points[(index + 1) % points.size()] - points[(index + 2) % points.size()]) * time * time + //Control Point 2
			(-points[(index - 1 + points.size()) % points.size()] + 3.f * points[index] - 3.f * points[(index + 1) % points.size()] + points[(index + 2) % points.size()]) * time * time * time); //Control Point 3
	}

	/// <summary>
	/// Compute the Keyframe time of each point, based on the length of each segment.
	/// </summary>
	void ComputeKeyframeTimes() {
		if (keyframeTime.size() != points.size())
			keyframeTime.resize(points.size());

		//Calcul la longueur total
		totalLength = 0;
		for (size_t i = 1; i < points.size(); i++)
			totalLength += glm::length(points[i] - points[i - 1]);


		//Calcul la longueur actuel et assigne au keyframeTime sa valeur entre 0 et 1.
		double current_length = 0;
		keyframeTime[0] = 0;
		for (size_t i = 1; i < points.size(); i++) {
			current_length += glm::length(points[i] - points[i - 1]);
			keyframeTime[i] = current_length / totalLength;
		}
	}

	/// <summary>
	/// Add a point to the Spline
	/// </summary>
	/// <param name="point">The point to add</param>
	void AddPoint(glm::vec3 point) {
		points.push_back(point);
		ComputeKeyframeTimes();
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
		this->points = points;
		ComputeKeyframeTimes();
	}

	/// <summary>
	/// Remove a point.
	/// </summary>
	/// <param name="points">The index to remove</param>
	void RemovePoint(int index) {
		this->points.erase(this->points.begin() + index);
		ComputeKeyframeTimes();
	}

	/// <summary>
	/// Remove all points.
	/// </summary>
	void ClearPoints(int index) {
		this->points.clear();
		totalLength = 0;
		keyframeTime.clear();
	}

	/// <summary>
	/// Return the length of the 3D Spline
	/// </summary>
	/// <returns>the length of the 3D Spline.</returns>
	float GetLength() {
		return this->totalLength;
	}
};

#endif // ! __SPLINE_HPP__
