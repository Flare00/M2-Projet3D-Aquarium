#ifndef __PHYSICS_HPP__
#define __PHYSICS_HPP__

#include <vector>
#include <Engine/SettedShaders.hpp>
#include <Engine/GameObject.hpp>
#include <Engine/Engine.hpp>
#include <Physics/Physics/CPhysic.hpp>
#include <Physics/Octree/Octree.hpp>
#include <Physics/CollisionDetection.hpp>

SettedShaders settedPhysicsShaders;

class Physics
{
protected:

	uint_fast32_t layerCollisionMatrix[32];
	CollisionDetection detection;

	Octree* octree = nullptr;

public:
	Physics(double octreeSize = 512.0)
	{
		octree = new Octree(glm::vec3(0), octreeSize);
		for (int i = 0; i < 32; i++) {
			layerCollisionMatrix[i] = ~(0 & 0);
		}
	}
	void Init() {}

	void Compute(double deltatime, GameObject* root, int nbStep = 1) {
		this->Compute(deltatime, root->getComponentsByTypeRecursive<CPhysic>(), nbStep);
	}

	void Compute(double deltatime, std::vector<CPhysic*> elems, int nbStep = 1) {
		double stepDelta = deltatime / (double)nbStep;


		for (int i = 0; i < nbStep; i++) {
			double currentStep = stepDelta * ((double)(i + 1));

			for (size_t j = 0, max = elems.size(); j < max; j++) {
				elems[j]->Compute(currentStep);
			}

			octree->Refresh(octree);

			std::vector<OctreeCollisionPossibility> octreePossibility = octree->GetAllCollisionPossibility();

			for (size_t j = 0, max = octreePossibility.size(); j < max; j++) {
				std::vector<GameObject*> possibility = octreePossibility[j].possibleCollisions;
				for (int k = 0, maxK = possibility.size(); k < maxK; k++) {
					for (int l = k + 1; l < maxK; l++) {
						// test collision between k et l.
						if (IsPhysicsBetweenLayers(possibility[k]->GetLayer(), possibility[l]->GetLayer())) {
							CollisionDetection::Data data = detection.Detection(possibility[k], possibility[l]);
						}
					}
				}
			}
		}
	}


	GameObject* Raycast(std::vector<GameObject*> elems, glm::vec3 origin, glm::vec3 direction) {
		// Use the Depth/Stencil (maybe faster) ? Or use collision ?
		for (size_t i = 0, max = elems.size(); i < max; i++) {

		}
		return nullptr;
	}

	void AddGameObjectToOctree(GameObject* gameobject) {
		octree->Insert(gameobject);
	}

	void AddGameObjectToOctreeRecursive(GameObject* gameobject) {
		octree->Insert(gameobject);
		for (int i = gameobject->getChilds().size() - 1; i <= 0; i++) {
			AddGameObjectToOctreeRecursive(gameobject->getChilds()[i]);
		}
	}


private:
	bool IsPhysicsBetweenLayers(uint_fast8_t l1, uint_fast8_t l2) {
		uint_fast32_t v = layerCollisionMatrix[l1];
		return (v & (1 << l2)) > 0;
	}
};

#endif