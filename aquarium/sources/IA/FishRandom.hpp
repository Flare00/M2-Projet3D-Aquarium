#ifndef __FISH_RANDOM_HPP__
#define __FISH_RANDOM_HPP__

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <Engine/Component/Component.hpp>
#include <Engine/EngineBehavior.hpp>
#include <Engine/Component/ModelInstanced.hpp>

/// <summary>
/// Fish Random system
/// </summary>
class FishRandom : public Component, public EngineBehavior
{
protected:

	ModelInstanced* fish;

	std::vector<glm::vec3> points;
	std::vector<glm::vec3> velocities;
	std::vector<float> timeBeforeChange;

	glm::vec2 timeMinMax;
	float velocityMax;
	glm::vec3 halfSizeBound;
	glm::vec3 center;
	int number;
public:


	/// <summary>
	/// Create a Random Fish Zone
	/// </summary>
	/// <param name="fish">The fish model for instanciation</param>
	/// <param name="sizeBound">the boundary size</param>
	/// <param name="number">Number of fish</param>
	/// <param name="timeMinMax">Time minimum and maximum of displacement (can be less if out of bound)</param>
	/// <param name="velocityMinMax">The minimum and maximum velocity for one axis</param>
	FishRandom(ModelInstanced* fish, glm::vec3 sizeBound, glm::vec3 center, int number = 500, glm::vec2 timeMinMax = glm::vec2(100.0f, 1000.0f), float velocityMax = 0.01)
	{
		this->fish = fish;
		this->halfSizeBound = sizeBound / 2.0f;
		
		this->number = number;
		this->timeMinMax = timeMinMax;
		this->velocityMax = velocityMax;
		this->center = center;
		points.resize(number);
		velocities.resize(number);
		timeBeforeChange.resize(number);

		for (int i = 0; i < number; i++) {
			float x = ValueBetween(-halfSizeBound.x + center.x, halfSizeBound.x + center.x);
			float y = ValueBetween(-halfSizeBound.y + center.y, halfSizeBound.y + center.y);
			float z = ValueBetween(-halfSizeBound.z + center.z, halfSizeBound.z + center.z);

			this->points[i] = glm::vec3(x, y, z);
		}

		this->fish->SetPositions(this->points);
	}

	/// <summary>
	/// Function called after the Gameobject attachement was set, automatically add the linked component.
	/// </summary>
	void PostAttachment() override {
	}

	/// <summary>
	/// The loop fonction to update positions.
	/// </summary>
	/// <param name="deltaT"></param>
	void loop(double deltaT) override {
		UpdatePositions(deltaT);
	}


	/// <summary>
	/// Update positions of the random fishes
	/// </summary>
	void UpdatePositions(double deltaT) {
		for (int i = 0; i < number; i++) {
			timeBeforeChange[i] -= deltaT;
			if (timeBeforeChange[i] > 0) {
				glm::vec3 tmp = this->points[i] + this->velocities[i];
				bool valid = true;
				for (int j = 0; j < 3 && valid; j++) {
					if (tmp[j] < -halfSizeBound[j] + center[j] || tmp[j] > halfSizeBound[j] + center[j]) {
						valid = false;
					}
				}

				if (valid) {
					this->points[i] = tmp;
				}
				else {
					timeBeforeChange[i] = -1;
				}

			}
			if (timeBeforeChange[i] <= 0) {
				this->velocities[i] = glm::ballRand(velocityMax);
				this->timeBeforeChange[i] = ValueBetween(timeMinMax);
			}
		}

		//Set for instanciation
		this->fish->SetPositions(this->points);
	}

private :
	float ValueBetween(glm::vec2 minmax) {
		return ValueBetween(minmax.x, minmax.y);
	}


	float ValueBetween(float min, float max) {
		float random = ((float)rand()) / ((float)RAND_MAX);
		float diff = max - min;
		return min + (random * diff);
	}
};

#endif // !__FISH_HPP__
