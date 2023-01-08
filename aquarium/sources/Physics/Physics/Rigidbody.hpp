#ifndef __RIGIDBODY_HPP__
#define __RIGIDBODY_HPP__


#include <Physics/Physics/CPhysic.hpp>
#include <glm/glm.hpp>


/// <summary>
/// A Rigidbody for Movement Physics (as Gravity, velocity, and acceleration).
/// </summary>
class Rigidbody : public CPhysic
{
protected:
	float mass = 1.0f;
	float drag = 0.0f;
	glm::vec3 velocity = glm::vec3(0);
	glm::vec3 acceleration = glm::vec3(0);
	bool gravity = false;

public:

	/// <summary>
	/// Create a rigidbody
	/// </summary>
	/// <param name="mass">The mass of the object</param>
	/// <param name="drag">The drag of the object</param>
	/// <param name="gravity">Is gravity applied to the object ?</param>
	Rigidbody(float mass = 1.0f, float drag = 0.0f, bool gravity = false) : CPhysic(true) {
		this->mass = mass;
		this->drag = drag;
		this->gravity = gravity;
	}

	/// <summary>
	/// Set the velocity of the object.
	/// </summary>
	/// <param name="velocity">The new velocity</param>
	void SetVelocity(glm::vec3 velocity) {
		this->velocity = velocity;
	}

	/// <summary>
	/// Return the velocity of the object.
	/// </summary>
	/// <returns>The velocity</returns>
	glm::vec3 GetVelocity() {
		return this->velocity;
	}

	/// <summary>
	/// Set the acceleration of the object.
	/// </summary>
	/// <param name="acceleration">The new acceleration.</param>
	void SetAcceleration(glm::vec3 acceleration) {
		this->acceleration = acceleration;
	}

	/// <summary>
	/// Get the acceleration the object
	/// </summary>
	/// <returns>The acceleration</returns>
	glm::vec3 GetAcceleration() {
		return this->acceleration;
	}

	/// <summary>
	/// Return if is gravity
	/// </summary>
	/// <returns>Is gravity ?</returns>
	bool IsGravity() {
		return gravity;
	}

	/// <summary>
	/// Create an impulse on the object (not implemented yet)
	/// </summary>
	/// <param name="impulse">The impulse</param>
	void Impulse(glm::vec3 impulse) {

	}


	/// <summary>
	/// Create an force on the object (not implemented yet)
	/// </summary>
	/// <param name="force">The force</param>
	void AddForce(glm::vec3 force) {

	}

	/// <summary>
	/// Compute the physic.
	/// </summary>
	/// <param name="delta">Time since last frame.</param>
	void Compute(double delta) override {
		this->attachment->GetTransform()->Translate(this->velocity * (float)delta);
	}
};
#endif