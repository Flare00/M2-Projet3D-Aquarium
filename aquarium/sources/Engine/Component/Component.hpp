#ifndef __COMPONENT_HPP__
#define __COMPONENT_HPP__

class Component {
public:
	bool active = true;
	virtual ~Component(){}
	void SetActive(bool active) {
		this->active = active;
	}
};

#endif // !__COMPONENT_HPP__
