#ifndef __I_COLLIDER_HPP__
#define __I_COLLIDER_HPP__

#include <vector>
#include <string>
#include <glm/glm.hpp>

#include <Engine/Component/Component.hpp>

class ICollider : public Component {
protected :
	std::string type;
	bool trigger;
public:
	ICollider(std::string type = "none", bool trigger = false) {
		this->type = type;
	}
	virtual bool IsColliding(ICollider* other) {
		return false;
	}

	std::string ColliderType(){
		return this->type;
	}
};

#endif