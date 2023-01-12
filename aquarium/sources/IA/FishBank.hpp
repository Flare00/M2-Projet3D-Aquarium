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

	std::vector<std::vector<size_t>> links;
	std::vector<glm::vec3> velocities;

	double avancement = 0.0;
	double speed = 0.02;
	float distRepos;
	double movementForce = 1.0;

	float k = 1.0f;
public:

	/// <summary>
	/// Create a FishBank
	/// </summary>
	/// <param name="fish">The model of the fish</param>
	/// <param name="spline">The spline that the Fish bank follow</param>
	/// <param name="distRepos">The rest distance between fish</param>
	/// <param name="nbLayer">The number of layer for the construction</param>
	/// <param name="initialAvancement">The default start position on the spline.</param>
	FishBank(ModelInstanced* fish, Spline* spline, float distRepos, int nbLayer, double initialAvancement = 0.0f) {
		this->fish = fish;
		this->spline = spline;
		this->avancement = initialAvancement;
		this->distRepos = distRepos;
		this->movementForce = movementForce / 20.0;

		this->fish->SetPositions(ModelGenerator::MultiLayerSphere(this->links, nbLayer, distRepos));
		
		glm::vec3 init = spline->Interpolate(this->avancement);
		std::vector<glm::vec3> pos = this->fish->GetPositions();
		glm::vec3 d =  init - pos[0];


		for (size_t i = 0, max = pos.size(); i < max; i++) {
			pos[i] += d;
			velocities.push_back(glm::vec3(0));
		}

		this->fish->SetPositions(pos);

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
		UpdatePositions(deltaT);
	}


	/// <summary>
	/// Update the position of the central fish, and move the fish bank.
	/// Also update the distance between elements.
	/// </summary>
	void UpdatePositions(double deltaT) {
		std::vector<glm::vec3> positions = this->fish->GetPositions();
		positions[0] = spline->Interpolate(this->avancement);
		for (int i = 1, max = this->links.size(); i < max; i++) {
			glm::vec3 force(0.0f);
			for (int j = 0, maxJ = this->links[i].size(); j < maxJ; j++) {
				glm::vec3 np = positions[links[i][j]] - positions[i];
				float d = glm::length(np);
				force += k * (d - distRepos) * glm::normalize(np);
			}
			this->velocities[i] += force * (float)deltaT;
			positions[i] += velocities[i] * (float)deltaT;
		}


		//Set for instanciation
		this->fish->SetPositions(positions);
	}
};

#endif // !__FISH_HPP__
