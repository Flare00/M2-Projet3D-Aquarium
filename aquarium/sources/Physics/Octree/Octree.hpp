#ifndef __OCTREE_HPP__
#define __OCTREE_HPP__

#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <Physics/Collider/ICollider.hpp>
#include <Engine/GameObject.hpp>

//A cube of the octree with is position, radius (each side is the same size) that contain a Gameobject
struct OctreeNode {
	glm::vec3 pos;
	double radius;
	std::vector<GameObject*> objects;
};

struct OctreeCollisionPossibility {
	std::vector<GameObject*> possibleCollisions;
};

int OCTREE_MAX_PROFONDEUR = 64;
class Octree
{
private:
	int currentProfondeur;

	OctreeNode current;

	std::vector<Octree*> childs;

public:

	Octree(glm::vec3 pos, double radius, int profondeur = 0) {
		current.pos = pos;
		current.radius = radius;
		currentProfondeur = profondeur;
	}

	~Octree() {
		RemoveChilds();
	}

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

		for (int i = current.objects.size(); i >= 0; i--) {
			if (!Fit(current.objects[i])) {
				root->Insert(current.objects[i]);
				current.objects.erase(std::next(current.objects.begin(), i));
			}
			nb += current.objects.size();
		}
		return nb;
	}

	void RemoveChilds() {
		for (int i = childs.size() - 1; i >= 0; i--) {
			delete childs[i];
		}
		childs.clear();
	}

	void ReInsert(GameObject* elem) {
		Remove(elem);
		Insert(elem);
	}

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

	bool Fit(GameObject* elem) {
		if (elem->HasCustomCollider()) {
			ICollider* collider = elem->getFirstComponentByType<ICollider>();
			if (collider != nullptr) {
				return Fit(collider);
			}
		}
		return Fit(elem->GetPositionWithRecursiveMatrix());
	}


	std::vector<OctreeCollisionPossibility> GetAllCollisionPossibility() {
		std::vector<OctreeCollisionPossibility> ocps = GetAllCollisionPossibility_Full();
		for (int i = ocps.size() - 1; i >= 0; i--) {
			if (ocps[i].possibleCollisions.size() <= 1) {
				ocps.erase(std::next(ocps.begin(), i));
			}
		}
	}

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

private:

	void MakeChilds() {
		if (childs.size() == 0) {
			double r = current.radius;

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

	bool Fit(ICollider* collider) {
		bool fit = false;
		std::string type = collider->ColliderType();
		if (type == "boundingbox") {
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
		else if (type == "sphere") {
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

	bool Fit(glm::vec3 pos) {
		bool fit = true;
		for (int i = 0; i < 3 && fit; i++) {
			if (pos[i] < current.pos[i] - current.radius || pos[i] > current.pos[i] + current.radius) {
				fit = false;
			}
		}
		return fit;
	}



};

#endif // !__OCTREE_HPP__
