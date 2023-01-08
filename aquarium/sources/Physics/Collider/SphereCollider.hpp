#ifndef __SPHERE_COLLIDER_HPP__
#define __SPHERE_COLLIDER_HPP__

#include <glm/glm.hpp>
#include <vector>
#include <Physics/Collider/ICollider.hpp>

/// <summary>
/// A Sphere collider.
/// </summary>
class SphereCollider : public ICollider {
protected:
	glm::vec3 center;
	double radius;
public :
	/// <summary>
	/// Create a Sphere collider
	/// </summary>
	/// <param name="radius">The radius of the sphere</param>
	/// <param name="center">The center of the sphere</param>
	SphereCollider(double radius = 1.0, glm::vec3 center = glm::vec3(0)) : ICollider(Type::Sphere) {
		this->center = center;
		this->radius = radius;
	}

	//SphereCollider(std::vector<glm::vec3> points) {
		/*glm::vec3 min = glm::vec3(DBL_MAX);
		glm::vec3 max = glm::vec3(-DBL_MAX);
		for (glm::vec3 p : points) {
			for (int i = 0; i < 3; i++) {
				if (p[i] < this->min[i]) {
					this->min[i] = p[i];
				}
				if (p[i] > this->max[i]) {
					this->max[i] = p[i];
				}
			}
		}*/
	//}


	/// <summary>
	/// Return the center of the sphere.
	/// </summary>
	/// <returns>The center of the sphere</returns>
	glm::vec3 GetCenter() {
		return this->center + this->attachment->GetTransform()->getPosition();
	}

	/// <summary>
	/// Return the radius of the sphere
	/// </summary>
	/// <returns>The radius of the sphere</returns>
	double GetRadius() {
		return this->radius;
	}
};

#endif // !__SPHERE_COLLIDER_HPP__
