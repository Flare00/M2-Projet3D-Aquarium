#ifndef __BOUNDING_BOX_COLLIDER_HPP__
#define __BOUNDING_BOX_COLLIDER_HPP__

#include <glm/glm.hpp>
#include <vector>
#include <float.h>
#include <Physics/Collider/ICollider.hpp>

/// <summary>
/// A Bounding Box collider. (AABB and OBB)
/// </summary>
class BoundingBoxCollider : public ICollider {
protected:
	glm::vec3 center;
	glm::vec3 halfSize;

	
public:
	/// <summary>
	/// Create a Bounding box collider.
	/// </summary>
	/// <param name="center">The center of the Bounding box</param>
	/// <param name="halfSize">The half size of each axis of the bounding box</param>
	BoundingBoxCollider(glm::vec3 center = glm::vec3(0), glm::vec3 halfSize = glm::vec3(1)) : ICollider(Type::BoundingBox) {
		this->center = center;
		this->halfSize = halfSize;
	}

	/// <summary>
	/// Create a bounding box, with a list of points.
	/// </summary>
	/// <param name="points">Points in 3D space</param>
	BoundingBoxCollider(std::vector<glm::vec3> points) {
		glm::vec3 min = glm::vec3(DBL_MAX);
		glm::vec3 max = glm::vec3(-DBL_MAX);
		for (glm::vec3 p : points) {
			for (int i = 0; i < 3; i++) {
				if (p[i] < min[i]) {
					min[i] = p[i];
				}
				if (p[i] > max[i]) {
					max[i] = p[i];
				}
			}
		}
		this->center = (max + min) / 2.0f;
		this->halfSize = (max - min) / 2.0f;
	}

	/// <summary>
	/// Return the Min value for AABB
	/// </summary>
	/// <returns>The min AABB value</returns>
	glm::vec3 GetMin() {
		return (this->center - halfSize) + this->attachment->GetPositionWithRecursiveMatrix();
	}

	/// <summary>
	/// Return the Max value for AABB
	/// </summary>
	/// <returns>The Max AABB value</returns>
	glm::vec3 GetMax() {
		return (this->center + halfSize) + this->attachment->GetPositionWithRecursiveMatrix();
	}

	/// <summary>
	/// Compute Min and Max of the bounding box (faster than calling GetMin then GetMax)
	/// </summary>
	/// <param name="min">the Min result</param>
	/// <param name="max">the Max result</param>
	void GetMinMax(glm::vec3& min, glm::vec3& max) {
		glm::vec3 pos = this->attachment->GetPositionWithRecursiveMatrix();
		min = (this->center - halfSize) + pos;
		max = (this->center + halfSize) + pos;
	}

	/// <summary>
	/// Return the Min value for OBB
	/// </summary>
	/// <returns>the Min value for OBB</returns>
	glm::vec3 GetMinOriented() {
		glm::vec4 tmp = glm::vec4(this->center - halfSize, 1) * this->attachment->GetRotationMatrixRecursive();
		return glm::vec3(tmp.x, tmp.y, tmp.z) + this->attachment->GetPositionWithRecursiveMatrix();
	}

	/// <summary>
	/// Return the Max value for OBB
	/// </summary>
	/// <returns>the Max value for OBB</returns>
	glm::vec3 GetMaxOriented() {
		glm::vec4 tmp = glm::vec4(this->center + halfSize, 1) * this->attachment->GetRotationMatrixRecursive();
		return glm::vec3(tmp.x, tmp.y, tmp.z) + this->attachment->GetPositionWithRecursiveMatrix();
	}



	/// <summary>
	/// Return the Center value for OBB
	/// </summary>
	/// <returns>the Center value for OBB</returns>
	glm::vec3 GetCenterOriented() {
		glm::vec4 tmp = glm::vec4(this->center,1) * this->attachment->GetTransform()->getRotationMatrix();
		return glm::vec3(tmp.x, tmp.y, tmp.z);
	}

	/// <summary>
	/// Return the Center value for AABB
	/// </summary>
	/// <returns>the Center value for AABB</returns>
	glm::vec3 GetCenter() {
		return this->center + this->attachment->GetPositionWithRecursiveMatrix();
	}

	/// <summary>
	/// Return the HalfSize of each axis.
	/// </summary>
	/// <returns></returns>
	glm::vec3 GetHalfSize() {
		return halfSize;
	}

};

#endif // !__BOUNDING_BOX_COLLIDER_HPP__
