#ifndef __FISH_BANK_HPP__
#define __FISH_BANK_HPP__

#include <vector>
#include <glm/glm.hpp>

#include <Physics/Collider/BoundingBoxCollider.hpp>
#include <Engine/Component/Component.hpp>
#include <Engine/EngineBehavior.hpp>
#include <Engine/Component/Model.hpp>
#include <IA/Spline.hpp>
#include <Engine/Tools/ModelGenerator.hpp>
/// <summary>
/// Fish bank system, using spring mass (to implement)
/// </summary>
class FishBank : public Component, public EngineBehavior
{
protected:

	Spline* spline;
	ModelInstanced* fish;

	std::vector<std::vector<unsigned int>> links;
	std::vector<glm::vec3> velocities;

	double avancement = 0.0;
	double speed = 0.1;
	double minDist, maxDist;
	double movementForce = 1.0;

public:

	/// <summary>
	/// Create a FishBank
	/// </summary>
	/// <param name="spline">The spline that the Fish bank follow</param>
	/// <param name="minDist">The minimum distance between fish</param>
	/// <param name="maxDist">The maximum distance between fish</param>
	/// <param name="nbLayer">The number of layer for the construction</param>
	/// <param name="initialAvancement">The default start position on the spline.</param>
	FishBank(ModelInstanced* fish, Spline* spline, double minDist, double maxDist, int nbLayer, double movementForce = 1, double initialAvancement = 0.0f) {
		this->fish = fish;
		this->spline = spline;
		this->avancement = initialAvancement;
		this->minDist = minDist;
		this->maxDist = maxDist;
		this->movementForce = movementForce / 20.0;
		this->speed = 0.1;

		this->fish->SetPositions(ModelGenerator::MultiLayerSphere(this->links, nbLayer, minDist, 1.10f));

		for (size_t i = 0, max = this->fish->GetPositions().size(); i < max; i++) {
			velocities.push_back(glm::vec3(0));
		}

		UpdatePositions();
	}

	~FishBank() {
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
		this->avancement += deltaT * this->speed;
		if (this->avancement >= 1.0) {
			this->avancement = 0.0;
		}
		UpdatePositions();
	}


	/// <summary>
	/// Update the position of the central fish, and move the fish bank.
	/// Also update the distance between elements.
	/// </summary>
	void UpdatePositions() {
		std::vector<glm::vec3> positions = this->fish->GetPositions();
		glm::vec3 tmp = spline->Interpolate(this->avancement);
		glm::vec3 advance = tmp - positions[0];
		positions[0] = tmp;

		//Compute all velocities, by the distance of each element to linked ones.
		for (int i = 1, maxI = this->links.size(); i < maxI; i++) {
			for (int j = 0, maxJ = this->links[i].size(); j < maxJ; j++) {
				if (i != j) {
					double dist = glm::distance(positions[i], positions[links[i][j]]);
					short moving = 0; // 0 ; ne bouge pas, 1 : pousser, 2 : tirer
					double subForce = 0;

					if (dist < minDist) {
						moving = 1;
						subForce = (minDist - dist);
					}
					else if (dist > maxDist) {
						moving = 2;
						subForce = (dist - maxDist);
					}

					if (moving > 0) {
						glm::vec3 dir = glm::normalize(positions[i] - positions[links[i][j]]);
						velocities[i] += dir * (float)(subForce * this->movementForce * (moving == 1 ? 1.0 : -1.0));
						//printf("Velocity %d : [%f, %f, %f]\n", i, velocities[i].x, velocities[i].y, velocities[i].z);

					}
				}

			}
		}

		//Apply computed velocity and the main movement.
		for (int i = 1, maxI = this->links.size(); i < maxI; i++) {
			positions[i] += velocities[i] / ((float)this->links[i].size());
			positions[i] += advance*0.9f;
		}
		//Set for instanciation
		this->fish->SetPositions(positions);
	}
};

#endif // !__FISH_HPP__
