#ifndef __I_COLLIDER_HPP__
#define __I_COLLIDER_HPP__

#include <vector>
#include <string>
#include <glm/glm.hpp>

#include <Engine/Component/Component.hpp>

/// <summary>
/// Collider abstract class for BoundingBoxCollider and SphereCollider.
/// </summary>
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
	/// <summary>
	/// Create an ICollider object
	/// </summary>
	/// <param name="type">The type of collider</param>
	/// <param name="trigger">Is the collider a trigger ?</param>
	ICollider(Type type = Type::None, bool trigger = false) {
		this->type = type;
	}

	/// <summary>
	/// Set custom collider to the Gameobject after attachement
	/// </summary>
	virtual void PostAttachment() {
		this->attachment->SetHasCustomCollider(true); // For Octree
	}

	/// <summary>
	/// Check if colliding, without override, return false.
	/// </summary>
	/// <param name="other">The other collider</param>
	/// <returns>Is colliding ?</returns>
	virtual bool IsColliding(ICollider* other) {
		return false;
	}

	/// <summary>
	/// Return if is collision
	/// </summary>
	/// <returns></returns>
	virtual bool IsCollision(){
		return isCollision;
	}

	/// <summary>
	/// Return the collider type.
	/// </summary>
	/// <returns>The collider type</returns>
	Type ColliderType(){
		return this->type;
	}
};

#endif