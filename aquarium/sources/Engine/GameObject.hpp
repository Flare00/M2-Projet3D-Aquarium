#ifndef __GAMEOBJECT_HPP__
#define __GAMEOBJECT_HPP__

#include <string>
#include <Engine/Component/Component.hpp>
class GameObject{
 
protected:
    std::string identifier;
	GameObject *parent;
	std::vector<GameObject *> childs;
	std::vector<IComponent *> components;

public :
	GameObject(std::string id, GameObject *parent = NULL, bool addToParent = false){
		this->identifier = id;
		this->parent = parent;
		if(addToParent && parent != NULL){
			this->parent->addChild(this);
		}
		this->components.push_back(new Transformation());
	}

	GameObject(std::string id, GameObject *parent, std::vector<GameObject *> childs, bool addToParent = false)
	{
		this->GameObject(id, parent, addToParent);
		this->childs = childs;
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

	bool isId(std::string id){
		return this->identifier.compare(id) == 0;
	}

	GameObject *findDirectChild(std::string identifier)
	{
		GameObject *res = NULL;
		for (size_t i = 0, max = this->childs.size(); i < max && res == NULL; i++)
		{
			if (this->childs[i]->isId(identifier))
			{
				res = this->childs[i];
			}
		}
		return res;
	}

	GameObject *findChild(std::string identifier)
	{
		GameObject *res = NULL;
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

	GameObject *getChild(int i)
	{
		return this->childs[i];
	}

	std::vector<GameObject *> getChilds()
	{
		return this->childs;
	}

	void addChild(GameObject *child)
	{
		child->setParent(this);
		this->childs.push_back(child);
	}

	void removeChild(int index)
	{
		this->childs[index]->setParent(NULL);
		this->childs.erase(this->childs.begin() + index);
	}

	void removeChild(GameObject *obj)
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

	GameObject *getParent()
	{
		return this->parent;
	}

	void addComponent(Component *component)
	{
		this->components.push_back(component);
	}

	std::vector<Component *> getAllComponents()
	{
		return this->components;
	}

	template<typename T>
	std::vector<Component<T> *> getComponentsByType<T>()
	{
		size_t i = 0, max = this->components.size();
		std::vector<Component<T> *> res;
		while (i < max)
		{
			if (this->components[i]->isType<T>())
			{
				res.push_back(this->components[i]);
			}
			i++;
		}
		return res;
	}

	template<typename T>
	std::vector<Component<T> *> getComponentsByTypeRecursive<T>()
	{
		std::vector<Component<T> *> res;
		for(size_t i = 0, max = this->componentss.size(); i < max; i++){
			if (this->componentss[i]->isType<T>())
			{
				res.push_back(this->componentss[i]);
			}
		}
		for(size_t i = 0, max = this->childs.size(); i < max; i++){
			std::vector<Component<T> *> tmp = this->childs[i]->getComponentsByTypeRecursive<T>();
			res.insert(res.end(), tmp.begin(), tmp.end());
		}

		return res;
	}

	template<typename T>
	Component<T>* getFirstComponentByType<T>(){
		Component<T>* res = NULL;
		for(int i = 0, max = this->componentss.size(); i < max && res == NULL; i++){
			if(this->componentss[i]->isType<T>()){
				res = this->componentss[i];
			}
		}
		return res;
	}

	void removeComponent(int id){
		if(id >= 0 && id < this->componentss.size()){
			delete this->componentss[i];
		}
	}

	template<typename T>
	void removeComponentsByType<T>(){
		for (size_t i = 0, max = this->componentss.size(); i < max == -1; i++)
		{
			if (this->componentss[i]->isType<T>())
			{
				this->componentss.erase(this->componentss.begin() + i);
				max--;
				i--;
			}
		}
	}


};

#endif