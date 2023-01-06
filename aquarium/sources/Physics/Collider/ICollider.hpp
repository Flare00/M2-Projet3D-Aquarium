#ifndef __I_COLLIDER_HPP__
#define __I_COLLIDER_HPP__

#include <vector>
#include <string>
#include <glm/glm.hpp>

#include <Engine/Component/Component.hpp>

class ICollider : public Component {
public:
	enum Type {
		None,
		BoundingBox,
		Sphere
	};
protected :
	Type type;
	bool trigger;
	bool isCollision = false;
public:
	ICollider(Type type = Type::None, bool trigger = false) {
		this->type = type;
		this->attachment->SetHasCustomCollider(true); // For Octree
	}

	virtual bool IsColliding(ICollider* other) {
		return false;
	}

	virtual bool IsCollision(){
		return isCollision;
	}

	Type ColliderType(){
		return this->type;
	}
};

#endif