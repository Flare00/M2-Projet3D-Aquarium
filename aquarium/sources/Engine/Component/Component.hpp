#ifndef __COMPONENT_HPP__
#define __COMPONENT_HPP__

class GameObject;

class Component {
public:
	GameObject* attachment;

	bool active = true;
	virtual ~Component(){}
	void SetActive(bool active) {
		this->active = active;
	}

	void Attach(GameObject* attachment) {
		this->attachment = attachment;
		PostAttachment();
	}
	virtual void PostAttachment() {
	}
};

#endif // !__COMPONENT_HPP__
