#ifndef __PHYSICS_HPP__
#define __PHYSICS_HPP__

#include <vector>
#include <Engine/SettedShaders.hpp>
#include <Engine/GameObject.hpp>
#include <Engine/Engine.hpp>
#include <Physics/Physics/CPhysic.hpp>
#include <Physics/CollisionDetection.hpp>

SettedShaders settedPhysicsShaders;

/// <summary>
/// Physics engine.
/// </summary>
class Physics
{
public:

	/// <summary>
	/// Raycast Hit data.
	/// </summary>
	struct RaycastHit {
		/// <summary>
		/// A RaycastHit data object, to handle raycast.
		/// </summary>
		/// <param name="hit">If the raycast hit something or not.</param>
		/// <param name="obj">The Object hitted by the raycast.</param>
		/// <param name="hitPosition">The Coordinate of the hit.</param>
		/// <param name="vertexIndex"> The index of the nearest vertex of the hitted object.</param>
		bool hit;
		GameObject* obj;
		glm::vec3 hitPosition;
		int vertexIndex;


		RaycastHit(bool hit, GameObject* obj, glm::vec3 hitPosition, int vertexIndex) {
			this->hit = hit;
			this->obj = obj;
			this->hitPosition = hitPosition;
			this->vertexIndex = vertexIndex;
		}

	};
protected:

	/// <summary>
	/// Layer collision matrix, store if an element collide with another one, using binary, with a list of 32, 32bit int (giving a total of 32 layer possible)
	/// </summary>
	uint_fast32_t layerCollisionMatrix[32];

	/// <summary>
	/// Utility to detect collision between two ICollider.
	/// </summary>
	CollisionDetection detection;

	//Octree* octree = nullptr; // Use way too much execution time and developpment time for this project.

public:

	/// <summary>
	/// Physics System using in the engine.
	/// </summary>
	Physics()
	{
		//octree = new Octree(glm::vec3(0), octreeSize);
		for (int i = 0; i < 32; i++) {
			layerCollisionMatrix[i] = ~(0 & 0);
		}
	}

	/// <summary>
	/// Init the physics system.
	/// </summary>
	void Init() {}

	/// <summary>
	/// Compute the physics for a scene, with a root Gameobject.
	/// </summary>
	/// <param name="deltatime">Time since last frame.</param>
	/// <param name="root">Root gameobject of the scene, or another gameobject.</param>
	/// <param name="nbStep">Number of computing steps for the physics.</param>
	void Compute(double deltatime, GameObject* root, int nbStep = 1) {
		this->Compute(deltatime, root->getComponentsByTypeRecursive<CPhysic>(), nbStep);
	}

	/// <summary>
	/// Compute the physics for a scene, with a CPhysic object list.
	/// </summary>
	/// <param name="deltatime">Time since last frame.</param>
	/// <param name="elems">CPhysic elements for Physics Computing.</param>
	/// <param name="nbStep">Number of computing steps for the physics.</param>
	void Compute(double deltatime, std::vector<CPhysic*> elems, int nbStep = 1) {
		double stepDelta = deltatime / (double)nbStep;


		for (int i = 0; i < nbStep; i++) {
			double currentStep = stepDelta * ((double)(i + 1));

			for (size_t j = 0, max = elems.size(); j < max; j++) {
				elems[j]->Compute(currentStep);
			}

			/*octree->Refresh(octree);
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
			}*/
		}
	}

	/// <summary>
	/// Send a Raycast using the physic system.
	/// </summary>
	/// <param name="elems">a root gameobject to compute collision.</param>
	/// <param name="origin">Origin of the ray.</param>
	/// <param name="direction">Direction of the ray.</param>
	/// <returns>Return a RaycastHit object that contain hit informations</returns>
	RaycastHit Raycast(GameObject * root, glm::vec3 origin, glm::vec3 direction) {
		// Use the Depth/Stencil (maybe faster) ? Or use collision ?

		return RaycastHit(false, nullptr, glm::vec3(0), -1);
	}

	/*void AddGameObjectToOctree(GameObject* gameobject) {
		octree->Insert(gameobject);
	}

	void AddGameObjectToOctreeRecursive(GameObject* gameobject) {
		octree->Insert(gameobject);
		for (int i = gameobject->getChilds().size() - 1; i <= 0; i++) {
			AddGameObjectToOctreeRecursive(gameobject->getChilds()[i]);
		}
	}*/


private:

	/// <summary>
	/// Compute if we need to compute physics between two element by their Layers
	/// </summary>
	/// <param name="l1">First layer to compare</param>
	/// <param name="l2">Second layer to compare</param>
	/// <returns></returns>
	bool IsPhysicsBetweenLayers(uint_fast8_t l1, uint_fast8_t l2) {
		uint_fast32_t v = layerCollisionMatrix[l1];
		return (v & (1 << l2)) > 0;
	}
};

#endif