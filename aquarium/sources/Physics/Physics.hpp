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
		int nearestVertex;
		double distance;


		RaycastHit(bool hit, GameObject* obj, glm::vec3 hitPosition, int vertexIndex, double distance = std::numeric_limits<double>::max()) {
			this->hit = hit;
			this->obj = obj;
			this->hitPosition = hitPosition;
			this->nearestVertex = vertexIndex;
			this->distance = distance;
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

	float addDropCooldown = 0.0f;
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
		glDisable(GL_BLEND);
		addDropCooldown -= deltatime;
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
		glEnable(GL_BLEND);
	}

	/// <summary>
	/// Send a Raycast using the physic system.
	/// </summary>
	/// <param name="elems">a root gameobject to compute collision.</param>
	/// <param name="mainCamera">Main camera of the scene.</param>
	/// <param name="screenX">Position X on the screen to start the ray.</param>
	/// <param name="screenY">Position Y on the screen to start the ray.</param>
	/// <returns>Return a RaycastHit object that contain hit informations</returns>
	RaycastHit Raycast(GameObject* root, Camera* camera, double x, double y) {

		if (camera != nullptr) {
			glBindFramebuffer(GL_FRAMEBUFFER, camera->GetFrameBuffer());
			glm::mat4 invProj = glm::inverse(camera->GetProjection());

			glm::vec4 rayStartN = glm::vec4(x, y, -1.0f, 1.0f);
			glm::vec4 rayEndN = glm::vec4(x, y, 0.0f, 1.0f);
			glm::vec4 rayStartWorld = invProj * rayStartN;

			rayStartWorld /= rayStartWorld.w;
			glm::vec4 rayEndWorld = invProj * rayEndN;
			rayEndWorld /= rayEndWorld.w;

			rayStartWorld = camera->attachment->GetMatrixRecursive() * rayStartWorld;
			rayEndWorld = camera->attachment->GetMatrixRecursive() * rayEndWorld;

			glm::vec3 rayStart = glm::vec3(rayStartWorld.x, rayStartWorld.y, rayStartWorld.z);
			glm::vec3 rayEnd = glm::vec3(rayEndWorld.x, rayEndWorld.y, rayEndWorld.z);
			glm::vec3 rayDirection = glm::normalize(rayEnd - rayStart);

			return Raycast(root, rayStart, rayDirection);
		}

		return RaycastHit(false, nullptr, glm::vec3(0), -1);
	}

	RaycastHit Raycast(GameObject* root, glm::vec3 origin, glm::vec3 dir) {
		std::vector<RaycastObject*> ro = root->getComponentsByTypeRecursive<RaycastObject>();

		RaycastHit res(false, nullptr, glm::vec3(0), -1);
		for (size_t i = 0, max = ro.size(); i < max; i++) {
			BoundingBoxCollider* bbc = ro[i]->attachment->getFirstComponentByType<BoundingBoxCollider>();
			double val = -1;
			int nearestVertex = -1;
			if (bbc != nullptr) {
				val = CollisionDetection::Ray_AABB(origin, dir, bbc);
			}

			if (val >= 0 && val < res.distance) {
				res.distance = val;
				res.hit = true;
				res.obj = ro[i]->GetGameObject();
				res.hitPosition = origin + (dir * (float)res.distance);
			}
		}

		if (res.hit) {
			Model* m = res.obj->getFirstComponentByType<Model>();
			if (m != nullptr) {
				std::vector<glm::vec3> pts = m->GetPoints();
				unsigned int minPts = -1;
				double minDist = std::numeric_limits<double>::max();
				for (int i = 0, max = pts.size(); i < max; i++) {
					double d = glm::distance(pts[i], res.hitPosition);
					if (d < minDist) {
						minPts = i;
						minDist = d;
					}
				}
				res.nearestVertex = minPts;
			}
		}
		return res;
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

	void DoRaycastAction(RaycastHit hit) {
		if (hit.hit) {
			WaterPhysics* wp = hit.obj->getFirstComponentByType<WaterPhysics>();
			if (wp != nullptr)
			{
				if (addDropCooldown < 0) {
					//Trouver les coordonnée X et Y selon la position du vertex; (entre 0 et 1)
					glm::vec2 size = wp->GetContainerSize();
					glm::vec2 min = -size / 2.0f;
					glm::vec2 max = size/ 2.0f;

					glm::vec3 hitPos = hit.hitPosition - hit.obj->GetPositionWithRecursiveMatrix();

					glm::vec2 uv = (glm::vec2(hitPos.x, hitPos.z) - min) / (max - min);
					uv.x = uv.x > 0.999 ? 0.999 : uv.x < 0.001 ? 0.001 : uv.x;
					uv.y = uv.y > 0.999 ? 0.999 : uv.y < 0.001 ? 0.001 : uv.y;
					//glm::vec2 uv((hit.hitPosition.x + (size.x / 2.0)) / size.x, 1.0 -(hit.hitPosition.z + (size.y / 2.0)) / size.y);

					wp->AddDrop(uv, 0.02f, 1.0f);
					addDropCooldown = 0.5;
				}
			}
		}
	}


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