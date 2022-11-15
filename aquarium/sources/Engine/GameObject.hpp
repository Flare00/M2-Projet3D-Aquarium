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
	std::vector<GameObject*> childs;
	std::vector<Component *> components;
	bool active = true;

public:
	GameObject(std::string id, GameObject* parent = NULL, bool addToParent = true) {
		this->identifier = id;
		this->parent = parent;
		if (addToParent && parent != NULL) {
			this->parent->addChild(this);
		}
		this->components.push_back(new Transformation());
	}

	GameObject(std::string id, GameObject* parent, std::vector<GameObject*> childs, bool addToParent = false) : GameObject(id, parent, addToParent)
	{
		this->identifier = id;
		this->parent = parent;
		if (addToParent && parent != NULL) {
			this->parent->addChild(this);
		}
		this->childs = childs;
		this->components.push_back(new Transformation());
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
		this->components.push_back(component);
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
};

#endif