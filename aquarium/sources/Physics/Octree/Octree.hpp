#ifndef __OCTREE_HPP__
#define __OCTREE_HPP__

#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <Physics/Collider/ICollider.hpp>
#include <Engine/GameObject.hpp>


int OCTREE_MAX_PROFONDEUR = 64;

/// <summary>
/// An Octree Structure, Not Working for now.
/// </summary>
class Octree
{
public : 
	/// <summary>
	/// List of Gameobject that are possibly in collision.
	/// </summary>
	struct OctreeCollisionPossibility {
		std::vector<GameObject*> possibleCollisions;
	};
private:
	int currentProfondeur;

	glm::vec3 pos;
	double radius;
	std::vector<GameObject*> objects;

	std::vector<Octree*> childs;

public:
	
	/// <summary>
	/// Generate an octree
	/// </summary>
	/// <param name="pos">Position of the octree</param>
	/// <param name="radius">Radius of the octree</param>
	/// <param name="profondeur">Profondeur value of this octree level (root = 0)</param>
	Octree(glm::vec3 pos, double radius, int profondeur = 0) {
		this->pos = pos;
		this->radius = radius;
		this->currentProfondeur = profondeur;
	}

	/// <summary>
	/// Destroy octree and childs.
	/// </summary>
	~Octree() {
		RemoveChilds();
	}

	/// <summary>
	/// Refresh the octree values.
	/// </summary>
	/// <param name="root">The root Octree.</param>
	/// <returns>the number of object in this branch of octree.</returns>
	int Refresh(Octree* root) {
		int nb = 0;
		if (childs.size() > 0) {
			for (int i = 0; i < 8; i++) {
				nb += childs[i]->Refresh(root);
			}
			if (nb == 0) {
				RemoveChilds();
			}
		}

		for (int i = current.objects.size() -1 ; i >= 0; i--) {
			if (!Fit(current.objects[i])) {
				root->Insert(current.objects[i]);
				current.objects.erase(std::next(current.objects.begin(), i));
			}
			nb += current.objects.size();
		}
		return nb;
	}

	/// <summary>
	/// Remove all childs of this octree.
	/// </summary>
	void RemoveChilds() {
		for (int i = childs.size() - 1; i >= 0; i--) {
			delete childs[i];
		}
		childs.clear();
	}

	/// <summary>
	/// Remove and Insert an element to the octree
	/// </summary>
	/// <param name="elem">The element to reinsert.</param>
	void ReInsert(GameObject* elem) {
		Remove(elem);
		Insert(elem);
	}

	/// <summary>
	/// Insert an element in the octree
	/// </summary>
	/// <param name="elem">The element to insert</param>
	void Insert(GameObject* elem) {
		if (!Fit(elem)) {
			printf("Octree Error : Gameobject does not fit in Octree");
			return;
		}
		current.objects.push_back(elem);

		if (currentProfondeur < OCTREE_MAX_PROFONDEUR) {
			if (current.objects.size() > 0) {
				if (childs.size() == 0) {
					MakeChilds();
				}
				if (!ReallocateToChild()) {
					RemoveChilds();
				}
			}
		}
	}


	/// <summary>
	/// Remove an element from the octreez
	/// </summary>
	/// <param name="elem">The element to remove</param>
	/// <returns>is remove succeed ?</returns>
	bool Remove(GameObject* elem) {
		bool erased = false;
		for (int i = current.objects.size() - 1; i >= 0; i++) {
			if (current.objects[i] == elem) {
				current.objects.erase(std::next(current.objects.begin(), i));
				erased = true;
			}
		}

		if (erased) {
			return true;
		}

		if (childs.size() > 0) {
			for (int i = 0; i < 8 && !erased; i++) {
				erased = childs[i]->Remove(elem);
			}
		}

		return erased;
	}

	/// <summary>
	/// is an element fitting inside of this Octree ?
	/// </summary>
	/// <param name="elem">The element to check</param>
	/// <returns>The element fit or not</returns>
	bool Fit(GameObject* elem) {
		if (elem->HasCustomCollider()) {
			ICollider* collider = elem->getFirstComponentByType<ICollider>();
			if (collider != nullptr) {
				return Fit(collider);
			}
		}
		return Fit(elem->GetPositionWithRecursiveMatrix());
	}


	/// <summary>
	/// Return collision possibility find in the octree.
	/// </summary>
	/// <returns>List of Octree Collision Possibility</returns>
	std::vector<OctreeCollisionPossibility> GetAllCollisionPossibility() {
		std::vector<OctreeCollisionPossibility> ocps = GetAllCollisionPossibility_Full();
		for (int i = ocps.size() - 1; i >= 0; i--) {
			if (ocps[i].possibleCollisions.size() <= 1) {
				ocps.erase(std::next(ocps.begin(), i));
			}
		}
		return ocps;
	}

	/// <summary>
	/// Return all Information of possibility from each childs, recursivly, including element with no collision possibility detected.
	/// </summary>
	/// <returns></returns>
	std::vector<OctreeCollisionPossibility> GetAllCollisionPossibility_Full() {
		std::vector<OctreeCollisionPossibility> res;
		if (this->childs.size() > 0) {
			for (int i = 0; i < 8; i++) {
				std::vector<OctreeCollisionPossibility> possibility = this->childs[i]->GetAllCollisionPossibility_Full();
				if (possibility.size() > 0) {
					if (current.objects.size() > 0) {
						for (int j = 0, max = possibility.size(); j < max; j++) {
							possibility[j].possibleCollisions.insert(possibility[j].possibleCollisions.end(), current.objects.begin(), current.objects.end());
						}
					}
					res.insert(res.end(), possibility.begin(), possibility.end());
				}
			}

		}
		else {
			if (current.objects.size() > 0) {
				OctreeCollisionPossibility ocp;
				ocp.possibleCollisions.insert(ocp.possibleCollisions.end(), current.objects.begin(), current.objects.end());
				res.push_back(ocp);
			}
		}

		return res;
	}

	/// <summary>
	/// Check if a collider fit in the octree.
	/// </summary>
	/// <param name="collider">The collider to test</param>
	/// <returns>Is fitting ?</returns>
	bool Fit(ICollider* collider) {
		bool fit = false;
		ICollider::Type type = collider->ColliderType();
		if (type == ICollider::Type::BoundingBox) {
			BoundingBoxCollider* bb = dynamic_cast<BoundingBoxCollider*>(collider);
			if (bb != nullptr) {

				if (Fit(bb->GetCenterOriented())) {
					glm::vec3 pMin = bb->GetMinOriented();
					glm::vec3 pMax = bb->GetMaxOriented();
					glm::vec3 cMin = current.pos - glm::vec3(current.radius);
					glm::vec3 cMax = current.pos + glm::vec3(current.radius);

					fit = pMin.x >= cMin.x && pMax.x <= cMax.x &&
						pMin.y >= cMin.y && pMax.y <= cMax.y &&
						pMin.z >= cMin.z && pMax.z <= cMax.z;
				}
			}
		}
		else if (type == ICollider::Type::Sphere) {
			SphereCollider* s = dynamic_cast<SphereCollider*>(collider);
			if (s != nullptr) {
				if (Fit(s->GetCenter())) {
					glm::vec3 pMin = s->GetCenter() - glm::vec3(s->GetRadius());
					glm::vec3 pMax = s->GetCenter() + glm::vec3(s->GetRadius());
					glm::vec3 cMin = current.pos - glm::vec3(current.radius);
					glm::vec3 cMax = current.pos + glm::vec3(current.radius);

					fit = pMin.x >= cMin.x && pMax.x <= cMax.x &&
						pMin.y >= cMin.y && pMax.y <= cMax.y &&
						pMin.z >= cMin.z && pMax.z <= cMax.z;
				}
			}
		}
		return fit;
	}

	/// <summary>
	/// Check if a point fit in this octree.
	/// </summary>
	/// <param name="pos">The point to test</param>
	/// <returns>Is fitting ?</returns>
	bool Fit(glm::vec3 pos) {
		bool fit = true;
		for (int i = 0; i < 3 && fit; i++) {
			if (pos[i] < current.pos[i] - current.radius || pos[i] > current.pos[i] + current.radius) {
				fit = false;
			}
		}
		return fit;
	}

	/// <summary>
	/// Generate childs for this octree.
	/// </summary>
	void MakeChilds() {
		if (childs.size() == 0) {
			double r = current.radius / 2.0;

			childs.push_back(new Octree(current.pos + glm::vec3(r, r, r), r / 2.0, this->currentProfondeur + 1));
			childs.push_back(new Octree(current.pos + glm::vec3(-r, r, r), r / 2.0, this->currentProfondeur + 1));
			childs.push_back(new Octree(current.pos + glm::vec3(-r, -r, r), r / 2.0, this->currentProfondeur + 1));
			childs.push_back(new Octree(current.pos + glm::vec3(r, -r, r), r / 2.0, this->currentProfondeur + 1));
			childs.push_back(new Octree(current.pos + glm::vec3(r, r, -r), r / 2.0, this->currentProfondeur + 1));
			childs.push_back(new Octree(current.pos + glm::vec3(-r, r, -r), r / 2.0, this->currentProfondeur + 1));
			childs.push_back(new Octree(current.pos + glm::vec3(-r, -r, -r), r / 2.0, this->currentProfondeur + 1));
			childs.push_back(new Octree(current.pos + glm::vec3(r, -r, -r), r / 2.0, this->currentProfondeur + 1));
		}
	}

	/// <summary>
	/// Realocate all object in this octree to children if possible.
	/// </summary>
	/// <returns>Is any object can go to children ?</returns>
	bool ReallocateToChild() {
		bool foundAny = false;
		for (size_t j = current.objects.size() - 1; j >= 0; j--) {
			bool found = false;
			for (int i = 0; i < 8 && !found; i++) {
				if (childs[i]->Fit(current.objects[j])) {
					childs[i]->Insert(current.objects[j]);
					current.objects.erase(std::next(current.objects.begin(), j));
					found = true;
					foundAny = true;
				}
			}
		}
		return foundAny;
	}





};

#endif // !__OCTREE_HPP__
