#ifndef __GAMEOBJECT_HPP__
#define __GAMEOBJECT_HPP__

#include <type_traits>
#include <string>
#include <any>
#include <vector>

#include <Engine/Component/Component.hpp>
#include <Engine/Component/Transformation.hpp>


/// <summary>
/// The Gameobject class, used by the engine as key element.
/// </summary>
class GameObject {

protected:
	// The identifier of this gameObject.
	std::string identifier;
	//The parent of the gameobject.
	GameObject* parent;
	//The Transformation of this Gameobject.
	Transformation transform;

	//The list of childs, and components.
	std::vector<GameObject*> childs;
	std::vector<Component*> components;

	bool customCollider = false; //for octree, not implemented yet.
	bool active = true;

	//The layer as 8bit value.
	uint_fast8_t layer;
	//The tag as 8bit value.
	uint_fast8_t tag;

public:
	/// <summary>
	/// Create an gameobject.
	/// </summary>
	/// <param name="id">The id of the gameobject.</param>
	/// <param name="parent">The parent of the gameobject.</param>
	/// <param name="addToParent">Add to parent Gameobject?</param>
	GameObject(std::string id, GameObject* parent = NULL, bool addToParent = true) {
		this->identifier = id;
		this->parent = parent;
		if (addToParent && parent != NULL) {
			this->parent->addChild(this);
		}
		this->components.push_back(&transform);
	}

	/// <summary>
	/// Create an gameobject.
	/// </summary>
	/// <param name="id">The id of the gameobject.</param>
	/// <param name="parent">The parent of the gameobject.</param>
	/// <param name="childs">The list of childs</param>
	/// <param name="addToParent">Add to parent Gameobject?</param>
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

	/// <summary>
	/// Destroy gameobject and childs Gameobject.
	/// </summary>
	~GameObject()
	{
		for (size_t i = 0, max = this->childs.size(); i < max; i++)
		{
			delete this->childs[i];
		}
	}

	/// <summary>
	/// Return the identifier.
	/// </summary>
	/// <returns>The identifier.</returns>
	std::string getIdentifier()
	{
		return this->identifier;
	}

	/// <summary>
	/// Check if the identifier is a certain value.
	/// </summary>
	/// <param name="id">The value to test.</param>
	/// <returns>Is the same ?</returns>
	bool isId(std::string id) {
		return this->identifier.compare(id) == 0;
	}

	/// <summary>
	/// Get a child by is identifier, no recursion.
	/// </summary>
	/// <param name="identifier">The identifier of the child</param>
	/// <returns>The child (if exist, else NULL)</returns>
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

	/// <summary>
	/// Get a child by is identifier, recursivly.
	/// </summary>
	/// <param name="identifier">The identifier of the child</param>
	/// <returns>The child (if exist, else NULL)</returns>
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

	/// <summary>
	/// Get the child by its index position.
	/// </summary>
	/// <param name="i">The index position.</param>
	/// <returns>The Child gameobject.</returns>
	GameObject* getChild(int i)
	{
		return this->childs[i];
	}

	/// <summary>
	/// Return all childs.
	/// </summary>
	/// <returns>All the childs.</returns>
	std::vector<GameObject*> getChilds()
	{
		return this->childs;
	}

	/// <summary>
	/// Add a child to the Gameobject
	/// </summary>
	/// <param name="child">The child to add.</param>
	void addChild(GameObject* child)
	{
		child->setParent(this);
		this->childs.push_back(child);
	}

	/// <summary>
	/// Remove a child by its index position.
	/// </summary>
	/// <param name="index">The index position.</param>
	void removeChild(int index)
	{
		this->childs[index]->setParent(NULL);
		this->childs.erase(this->childs.begin() + index);
	}

	/// <summary>
	/// Remove a child, by it's reference.
	/// </summary>
	/// <param name="obj">The Gameobject to remove.</param>
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

	/// <summary>
	/// Set the parent of the gameobject.
	/// </summary>
	/// <param name="parent">The new parent.</param>
	void setParent(GameObject* parent) {
		this->parent = parent;
	}

	/// <summary>
	/// Return the parent of the gameobject.
	/// </summary>
	/// <returns></returns>
	GameObject* getParent()
	{
		return this->parent;
	}

	/// <summary>
	/// Add a component to the gameobject, auto attach gameobject to this component.
	/// </summary>
	/// <param name="component">The component to add.</param>
	void addComponent(Component* component)
	{
		if (component != nullptr) {
			component->Attach(this);
			this->components.push_back(component);
		}
	}


	/// <summary>
	/// Return all components by their type, this gameobject only.
	/// </summary>
	/// <typeparam name="T">The type of the components</typeparam>
	/// <param name="activeOnly">Is components need to be active ?</param>
	/// <returns>The list of components (may be empty)</returns>
	template <typename T>
	std::vector<T*> getComponentsByType(bool activeOnly = false)
	{
		std::vector<T*> res;
		for (size_t i = 0, max = this->components.size(); i < max; i++) {
			if (!activeOnly || (activeOnly && this->components[i]->active)) {
				//Dynamic cast, if impossible, return nullptr.
				T* comp = dynamic_cast<T*>(this->components[i]);
				if (comp != nullptr) {
					res.push_back(comp);
				}
			}
		}
		return res;
	}

	/// <summary>
	/// Return all components by their type, this gameobject, and all children (recursivly).
	/// </summary>
	/// <typeparam name="T">The type of the components</typeparam>
	/// <param name="activeOnly">Is components need to be active ?</param>
	/// <returns>The list of components (may be empty)</returns>
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


	/// <summary>
	/// Get the first component by his type.
	/// </summary>
	/// <typeparam name="T">The type of the component</typeparam>
	/// <returns>The component (if exist, else NULL)</returns>
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

	/// <summary>
	/// Get childs that does not have a certain component, not recursive.
	/// </summary>
	/// <typeparam name="T">The component that filter.</typeparam>
	/// <returns>The list of childs that does not have the component.</returns>
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

	/// <summary>
	/// Get all childs that does not have a certain component, recursivly.
	/// </summary>
	/// <typeparam name="T">The component that filter.</typeparam>
	/// <returns>The list of childs that does not have the component.</returns>
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

	/// <summary>
	/// Remove a component by it's index position.
	/// </summary>
	/// <param name="id">The index position to remove.</param>
	void removeComponent(int id) {
		if (id >= 0 && id < this->components.size()) {
			this->components.erase(this->components.begin() + id);
		}
	}


	/// <summary>
	/// Remove all components of a certain type, not recursive.
	/// </summary>
	/// <typeparam name="T">The type of the components to remove.</typeparam>
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

	/// <summary>
	/// Set if the Gameobject is active or not.
	/// </summary>
	/// <param name="active">The new active state.</param>
	void SetActive(bool active) {
		this->active = active;
	}

	/// <summary>
	/// Return the transformation object.
	/// </summary>
	/// <returns>The transformation object.</returns>
	Transformation* GetTransform() {
		return &this->transform;
	}

	/// <summary>
	/// Get The Transformation matrix, with all parents Transformation matrix applied to it.
	/// </summary>
	/// <returns>The global Transformation matrix.</returns>
	glm::mat4 GetMatrixRecursive() {
		if (this->parent != nullptr) {
			return this->parent->GetMatrixRecursive() * this->transform.getMatrix();
		}
		return this->transform.getMatrix();
	}

	/// <summary>
	/// Get The Rotation matrix, with all parents Rotation matrix applied to it.
	/// </summary>
	/// <returns>The global Rotation matrix.</returns>
	glm::mat4 GetRotationMatrixRecursive() {
		if (this->parent != NULL) {
			return this->parent->GetRotationMatrixRecursive() * this->transform.getRotationMatrix();
		}
		return this->transform.getRotationMatrix();
	}

	/// <summary>
	/// Get the position of the Gameobject, using the global Transformation Matrix (GetMatrixRecursive()).
	/// </summary>
	/// <returns>The world position.</returns>
	glm::vec3 GetPositionWithRecursiveMatrix() {
		glm::vec4 pos = glm::vec4(this->transform.getPosition(), 1.0) * GetMatrixRecursive();
		return glm::vec3(pos.x, pos.y, pos.z);
	}

	/// <summary>
	/// Return if has a custom collider 
	/// </summary>
	/// <returns>Has a custom collider ?</returns>
	bool HasCustomCollider() {
		return this->customCollider;
	}


	/// <summary>
	/// Set if has a custom collider
	/// </summary>
	/// <param name="customCollider">the new value</param>
	void SetHasCustomCollider(bool customCollider) {
		this->customCollider = customCollider;
	}

	/// <summary>
	/// Return the layer information.
	/// </summary>
	/// <returns>The layer information</returns>
	int_fast8_t GetLayer() {
		return this->layer;
	}

	/// <summary>
	/// Return the tag information.
	/// </summary>
	/// <returns>The tag information</returns>
	int_fast8_t GetTag() {
		return this->tag;
	}
};

#endif