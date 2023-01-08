#ifndef __COMPONENT_HPP__
#define __COMPONENT_HPP__

class GameObject;

/// <summary>
/// A component that is attached to an Gameobject.
/// </summary>
class Component {
public:
	GameObject* attachment;
	bool active = true;

	virtual ~Component(){}

	/// <summary>
	/// Change the active status of the component.
	/// </summary>
	/// <param name="active">Is the component is active or not</param>
	void SetActive(bool active) {
		this->active = active;
	}

	/// <summary>
	/// Attach the Gameobject linked to this component.
	/// </summary>
	/// <param name="attachment"></param>
	void Attach(GameObject* attachment) {
		this->attachment = attachment;
		PostAttachment();
	}

	/// <summary>
	/// Function called after the gameobject is attached to the component.
	/// </summary>
	virtual void PostAttachment() {
	}

	/// <summary>
	/// Return the attachment gameobject.
	/// </summary>
	/// <returns>The attachment Gameobject.</returns>
	GameObject* GetGameObject() {
		return attachment;
	}

};

#endif // !__COMPONENT_HPP__
