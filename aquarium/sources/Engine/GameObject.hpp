#ifndef __GAMEOBJECT_HPP__
#define __GAMEOBJECT_HPP__

#include <type_traits>
#include <string>
#include <any>
#include <vector>

#include <Engine/Component/Component.hpp>
#include <Engine/Component/Transformation.hpp>


class GameObject {

protected:
	std::string identifier;
	GameObject* parent;
	Transformation transform;

	std::vector<GameObject*> childs;
	std::vector<Component*> components;

	bool customCollider = false; //for octree
	bool active = true;

	uint_fast8_t layer;
	uint_fast8_t tag;

public:
	GameObject(std::string id, GameObject* parent = NULL, bool addToParent = true) {

		this->identifier = id;
		this->parent = parent;
		if (addToParent && parent != NULL) {
			this->parent->addChild(this);
		}
		this->components.push_back(&transform);
	}

	GameObject(std::string id, GameObject* parent, std::vector<GameObject*> childs, bool addToParent = false) : GameObject(id, parent, addToParent)
	{
		this->identifier = id;
		this->parent = parent;
		if (addToParent && parent != NULL) {
			this->parent->addChild(this);
		}
		this->childs = childs;
		this->components.push_back(&transform);
	}

	~GameObject()
	{
		for (size_t i = 0, max = this->childs.size(); i < max; i++)
		{
			delete this->childs[i];
		}
	}

	std::string getIdentifier()
	{
		return this->identifier;
	}

	bool isId(std::string id) {
		return this->identifier.compare(id) == 0;
	}

	GameObject* findDirectChild(std::string identifier)
	{
		GameObject* res = NULL;
		for (size_t i = 0, max = this->childs.size(); i < max && res == NULL; i++)
		{
			if (this->childs[i]->isId(identifier))
			{
				res = this->childs[i];
			}
		}
		return res;
	}

	GameObject* findChild(std::string identifier)
	{
		GameObject* res = NULL;
		for (size_t i = 0, max = this->childs.size(); i < max && res == NULL; i++)
		{
			if (this->childs[i]->isId(identifier))
			{
				res = this->childs[i];
			}
			else
			{
				res = this->childs[i]->findChild(identifier);
			}
		}
		return res;
	}

	GameObject* getChild(int i)
	{
		return this->childs[i];
	}

	std::vector<GameObject*> getChilds()
	{
		return this->childs;
	}

	void addChild(GameObject* child)
	{
		child->setParent(this);
		this->childs.push_back(child);
	}

	void removeChild(int index)
	{
		this->childs[index]->setParent(NULL);
		this->childs.erase(this->childs.begin() + index);
	}

	void removeChild(GameObject* obj)
	{
		size_t found = -1;
		for (size_t i = 0, max = this->childs.size(); i < max && found == -1; i++)
		{
			if (this->childs[i] == obj)
			{
				found = i;
			}
		}
		if (found != -1)
		{
			this->childs[found]->setParent(NULL);
			this->childs.erase(this->childs.begin() + found);
		}
	}

	void setParent(GameObject* parent) {
		this->parent = parent;
	}
	GameObject* getParent()
	{
		return this->parent;
	}

	void addComponent(Component* component)
	{
		if (component != nullptr) {
			component->Attach(this);
			this->components.push_back(component);
		}
	}


	template <typename T>
	std::vector<T*> getComponentsByType(bool activeOnly = false)
	{
		std::vector<T*> res;
		for (size_t i = 0, max = this->components.size(); i < max; i++) {
			if (!activeOnly || (activeOnly && this->components[i]->active)) {
				T* comp = dynamic_cast<T*>(this->components[i]);
				if (comp != nullptr) {
					res.push_back(comp);
				}
			}
		}
		return res;
	}

	template<class T>
	std::vector<T*> getComponentsByTypeRecursive(bool activeOnly = false)
	{
		if (this->active) {
			std::vector<T*> res = getComponentsByType<T>(activeOnly);

			for (size_t i = 0, max = this->childs.size(); i < max; i++) {
				std::vector<T*> tmp = this->childs[i]->getComponentsByTypeRecursive<T>(activeOnly);
				res.insert(res.end(), tmp.begin(), tmp.end());
			}
			return res;
		}
		return std::vector<T*>();
	}



	template<typename T>
	T* getFirstComponentByType() {
		T* res = NULL;
		for (int i = 0, max = this->components.size(); i < max && res == NULL; i++) {
			T* comp = dynamic_cast<T*>(this->components[i]);
			if (comp != nullptr) {
				res = comp;
			}
		}
		return res;
	}

	template<typename T>
	std::vector<GameObject*> GetChildsWithoutComponent()
	{
		std::vector<GameObject*> res;
		for (size_t i = 0, maxI = this->childs.size(); i < maxI; i++) {
			if (this->childs[i]->active) {
				if (this->childs[i]->getFirstComponentByType<T>() == nullptr) {
					res.push_back(this->childs[i]);
				}
			}
		}
		return res;
	}

	template<typename T>
	std::vector<GameObject*> GetChildsWithoutComponentRecursive()
	{
		std::vector<GameObject*> res;
		for (size_t i = 0, maxI = this->childs.size(); i < maxI; i++) {
			if (this->childs[i]->active) {
				std::vector<GameObject*> tmp = this->childs[i]->GetChildsWithoutComponentRecursive<T>();
				if (tmp.size() > 0) {
					res.insert(res.end(), tmp.begin(), tmp.end());
				}
				if (this->childs[i]->getFirstComponentByType<T>() == nullptr) {
					res.push_back(this->childs[i]);
				}
			}
		}
		return res;
	}

	void removeComponent(int id) {
		if (id >= 0 && id < this->components.size()) {
			this->components.erase(this->components.begin() + id);
		}
	}



	template<typename T>
	void removeComponentsByType() {
		for (size_t i = 0, max = this->components.size(); i < max == -1; i++)
		{
			T* comp = dynamic_cast<T*>(this->components[i]);
			if (comp != nullptr)
			{
				this->components.erase(this->components.begin() + i);
				max--;
				i--;
			}
		}
	}

	void SetActive(bool active) {
		this->active = active;
	}

	Transformation* GetTransform() {
		return &this->transform;
	}

	glm::mat4 GetMatrixRecursive() {
		if (this->parent != nullptr) {
			return this->parent->GetMatrixRecursive() * this->transform.getMatrix();
		}
		return this->transform.getMatrix();
	}
	glm::mat4 GetRotationMatrixRecursive() {
		if (this->parent != NULL) {
			return this->parent->GetRotationMatrixRecursive() * this->transform.getRotationMatrix();
		}
		return this->transform.getRotationMatrix();
	}

	glm::vec3 GetPositionWithRecursiveMatrix() {
		glm::vec4 pos = glm::vec4(this->transform.getPosition(), 1.0) * GetMatrixRecursive();
		return glm::vec3(pos.x, pos.y, pos.z);
	}

	bool HasCustomCollider() {
		return this->customCollider;
	}

	void SetHasCustomCollider(bool customCollider) {
		this->customCollider = customCollider;
	}

	int_fast8_t GetLayer() {
		return this->layer;
	}

	int_fast8_t GetTag() {
		return this->tag;
	}
};

#endif