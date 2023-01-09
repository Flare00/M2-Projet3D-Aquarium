#ifndef __FISH_BANK_HPP__
#define __FISH_BANK_HPP__

#include <vector>
#include <glm/glm.hpp>

#include <Engine/GameObject.hpp>
#include <Physics/Collider/BoundingBoxCollider.hpp>
#include <Engine/EngineBehavior.hpp>

/// <summary>
/// Fish bank system, using spring mass (to implement)
/// </summary>
class FishBank : public EngineBehavior 
{
public:
	std::vector<GameObject*> bank;
	glm::vec3 initialPos;
	double minDist, maxDist;
	BoundingBoxCollider limits;


	FishBank(int numberOfFish, double minDist, double maxDist, glm::vec3 initialPos, BoundingBoxCollider limits)
	{
		//Système masse ressort ?

	}
};

#endif // !__FISH_HPP__
