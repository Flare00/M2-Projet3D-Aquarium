#ifndef __BOUNDING_BOX_COLLIDER_HPP__
#define __BOUNDING_BOX_COLLIDER_HPP__

#include <glm/glm.hpp>
#include <vector>
#include <float.h>
#include <Physics/Collider/ICollider.hpp>

class BoundingBoxCollider : public ICollider {
protected:
	glm::vec3 center;
	glm::vec3 halfSize;

	
public:
	BoundingBoxCollider(glm::vec3 center = glm::vec3(0), glm::vec3 halfSize = glm::vec3(1)) : ICollider(Type::BoundingBox) {
		this->center = center;
		this->halfSize = halfSize;
	}

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

	glm::vec3 GetMin() {
		return (this->center - halfSize) + this->attachment->GetTransform()->getPosition();
	}

	glm::vec3 GetMax() {
		return (this->center + halfSize) + this->attachment->GetTransform()->getPosition();
	}

	glm::vec3 GetMinOriented() {
		glm::vec4 tmp = glm::vec4(this->center - halfSize, 1) * this->attachment->GetRotationMatrixRecursive();
		return glm::vec3(tmp.x, tmp.y, tmp.z) + this->attachment->GetPositionWithRecursiveMatrix();
	}

	glm::vec3 GetMaxOriented() {
		glm::vec4 tmp = glm::vec4(this->center + halfSize, 1) * this->attachment->GetRotationMatrixRecursive();
		return glm::vec3(tmp.x, tmp.y, tmp.z) + this->attachment->GetPositionWithRecursiveMatrix();
	}

	glm::vec3 GetCenterOriented() {
		glm::vec4 tmp = glm::vec4(this->center,1) * this->attachment->GetTransform()->getRotationMatrix();
		return glm::vec3(tmp.x, tmp.y, tmp.z);
	}

	glm::vec3 GetCenter() {
		return this->center + this->attachment->GetPositionWithRecursiveMatrix();
	}

	glm::vec3 GetHalfSize() {
		return halfSize;
	}

	

	bool IsColliding(ICollider * other) override {
		printf("Self : %s | Other : %s\n", this->ColliderType().c_str(), other->ColliderType().c_str());
		return false;
	}

	

};

#endif // !__BOUNDING_BOX_COLLIDER_HPP__
