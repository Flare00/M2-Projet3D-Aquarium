#ifndef __COLLISION_DETECTION_HPP__
#define __COLLISION_DETECTION_HPP__

#include <glm/glm.hpp>
#include <Physics/Collider/BoundingBoxCollider.hpp>
#include <Physics/Collider/SphereCollider.hpp>
#include <Engine/GameObject.hpp>


class CollisionDetection {
public:


	enum BBMode {
		AABB_AABB,
		AABB_OBB,
		OBB_AABB,
		OBB_OBB
	};
	struct Data {
		bool collision = false;
		glm::vec3 closestPoint;

		Data(bool c, glm::vec3 cp) {
			this->collision = c;
			this->closestPoint = cp;
		}
	};

	struct Triangle {
		glm::vec3 points[3];
		Triangle(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2) {
			this->points[0] = p0;
			this->points[0] = p1;
			this->points[0] = p2;
		}
		glm::vec3 operator[](size_t idx) {
			return points[idx];
		}
	};

	Data Detection(GameObject* one, GameObject* two) {
		ICollider* collider1 = nullptr;
		ICollider* collider2 = nullptr;

		if (one->HasCustomCollider()) {
			collider1 = one->getFirstComponentByType<ICollider>();
		}
		else {
			return Data(false, glm::vec3());
		}

		if (two->HasCustomCollider()) {
			collider2 = two->getFirstComponentByType<ICollider>();
		}
		else {
			return Data(false, glm::vec3());
		}


		if (collider1->ColliderType() == ICollider::BoundingBox)
		{
			BoundingBoxCollider* bb = dynamic_cast<BoundingBoxCollider*>(collider1);
			if (bb != nullptr) {
				return Detection(bb, collider2);
			}
		}
		else if (collider1->ColliderType() == ICollider::Sphere) {
			SphereCollider* s = dynamic_cast<SphereCollider*>(collider1);
			if (s != nullptr) {
				return Detection(s, collider2, true);
			}
		}
	}

	Data Detection(BoundingBoxCollider* one, ICollider* two) {
		if (two->ColliderType() == ICollider::BoundingBox)
		{
			BoundingBoxCollider* bb = dynamic_cast<BoundingBoxCollider*>(two);
			if (bb != nullptr) {
				return Detection(one, bb, OBB_OBB);
			}
		}
		else if (two->ColliderType() == ICollider::Sphere) {
			SphereCollider* s = dynamic_cast<SphereCollider*>(two);
			if (s != nullptr) {
				return Detection(s, one, true);

			}
		}
	}

	Data Detection(SphereCollider* one, ICollider* two) {
		if (two->ColliderType() == ICollider::BoundingBox)
		{
			BoundingBoxCollider* bb = dynamic_cast<BoundingBoxCollider*>(two);
			if (bb != nullptr) {
				return Detection(one, bb, true);
			}
		}
		else if (two->ColliderType() == ICollider::Sphere) {
			SphereCollider* s = dynamic_cast<SphereCollider*>(two);
			if (s != nullptr) {
				return Detection(one, s);
			}
		}
	}

	// --- FUNCTIONS ---
	Data Detection(BoundingBoxCollider* one, BoundingBoxCollider* two, BBMode mode) {
		if (mode == AABB_AABB)
			return AABB_AABB(one, two);
		else if (mode == OBB_OBB)
			return OBB_OBB(one, two);
		else if (mode == AABB_OBB)
			return OBB_AABB(two, one);
		else
			return OBB_AABB(one, two);
	}

	Data Detection(SphereCollider* one, BoundingBoxCollider* two, bool oriented) {
		if (oriented) {
			return Sphere_OBB(one, two);
		}
		else {
			return Sphere_AABB(one, two);
		}
	}

	Data Detection(glm::vec3 one, BoundingBoxCollider* two, bool oriented) {
		if (oriented) {
			return Point_OBB(one, two);
		}
		else {
			return Point_AABB(one, two);
		}
	}

	Data Detection(glm::vec3 one, SphereCollider* two) {
		return Point_Sphere(one, two);
	}

	Data Detection(SphereCollider* one, SphereCollider* two) {
		return Sphere_Sphere(one, two);
	}

	// --- SPECIFIC ---

	Data AABB_AABB(BoundingBoxCollider* one, BoundingBoxCollider* two) {
		glm::vec3 closestPoint;
		glm::vec3 c = one->GetCenter();
		glm::vec3 aMin = one->GetMin(), aMax = one->GetMax();
		glm::vec3 bMin = one->GetMin(), bMax = one->GetMax();

		bool collision = (aMin.x <= bMax.x && aMax.x >= bMin.x) &&
			(aMin.y <= bMax.y && aMax.y >= bMin.y) &&
			(aMin.z <= bMax.z && aMax.z >= bMin.z);

		if (glm::distance(c, bMin) <= glm::distance(c, bMax)) {
			closestPoint = Point_AABB(bMin, one);
		}
		else {
			closestPoint = Point_AABB(bMax, one);
		}

		return Data(collision, closestPoint);
	}

	Data OBB_OBB(BoundingBoxCollider* one, BoundingBoxCollider* two) {
		bool collision = true;
		glm::vec3 closestPoint;

		glm::mat3 o1(one->attachment->GetTransform()->getRotationMatrix());
		glm::mat3 o2(two->attachment->GetTransform()->getRotationMatrix());

		glm::vec3 test[15] = {
			glm::vec3(o1[0][0], o1[0][1], o1[0][2]),
			glm::vec3(o1[1][0], o1[1][1], o1[1][2]),
			glm::vec3(o1[2][0], o1[2][1], o1[2][2]),
			glm::vec3(o2[0][0], o2[0][1], o2[0][2]),
			glm::vec3(o2[1][0], o2[1][1], o2[1][2]),
			glm::vec3(o2[2][0], o2[2][1], o2[2][2])
		};

		for (int i = 0; i < 3; ++i) { // Fill out rest of axis
			test[6 + i * 3 + 0] = glm::cross(test[i], test[0]);
			test[6 + i * 3 + 1] = glm::cross(test[i], test[1]);
			test[6 + i * 3 + 2] = glm::cross(test[i], test[2]);
		}

		for (int i = 0; i < 15; ++i) {
			if (!AxisOverlap(one, two, test[i], OBB_OBB)) {
				return false; // Seperating axis found
			}
		}
		return true;
	}

	Data AABB_OBB(BoundingBoxCollider* one, BoundingBoxCollider* two) {
		return OBB_AABB(two, one);
	}
	Data OBB_AABB(BoundingBoxCollider* one, BoundingBoxCollider* two) {
		bool collision = true;
		glm::vec3 closestPoint;

		glm::mat3 o(one->attachment->GetTransform()->getRotationMatrix());

		glm::vec3 test[15] = {
			glm::vec3(1,0,0),
			glm::vec3(0,1,0),
			glm::vec3(0,0,1),
			glm::vec3(o[0][0], o[0][1], o[0][2]),
			glm::vec3(o[1][0], o[1][1], o[1][2]),
			glm::vec3(o[2][0], o[2][1], o[2][2])
		};

		for (int i = 0; i < 3; ++i) { // Fill out rest of axis
			test[6 + i * 3 + 0] = glm::cross(test[i], test[0]);
			test[6 + i * 3 + 1] = glm::cross(test[i], test[1]);
			test[6 + i * 3 + 2] = glm::cross(test[i], test[2]);
		}

		for (int i = 0; i < 15; ++i) {
			if (!AxisOverlap(one, two, test[i], OBB_AABB)) {
				return false; // Seperating axis found
			}
		}
		return true;
	}

	Data AABB_Sphere(BoundingBoxCollider* one, SphereCollider* two) {
		return Sphere_AABB(two, one);
	}
	Data Sphere_AABB(SphereCollider* one, BoundingBoxCollider* two) {
		glm::vec3 oneCenter = one->GetCenter();
		Data d = Point_AABB(oneCenter, two);
		if (!d.collision) {
			d.collision = glm::distance(oneCenter, d.closestPoint) < one->GetRadius() * one->GetRadius();
		}
		return d;
	}

	Data OBB_Sphere(BoundingBoxCollider* one, SphereCollider* two) {
		return Sphere_AABB(two, one);
	}
	Data Sphere_OBB(SphereCollider* one, BoundingBoxCollider* two) {
		glm::vec3 oneCenter = one->GetCenter();
		Data d = Point_OBB(oneCenter, two);
		if (!d.collision) {
			d.collision = glm::distance(oneCenter, d.closestPoint) < one->GetRadius() * one->GetRadius();
		}
		return d;
	}



	Data Sphere_Sphere(SphereCollider* one, SphereCollider* two) {
		float dist = glm::distance(one->GetCenter(), two->GetCenter());
		float sum = one->GetRadius() + two->GetRadius();
		glm::vec3 n = glm::normalize(two->GetCenter() - one->GetCenter());
		return Data(dist <= sum, n * one->GetRadius());
	}

	// --- Points ---

	Data Point_AABB(glm::vec3 one, BoundingBoxCollider* two) {
		bool collision = true;
		glm::vec3 closestPoint = one;

		glm::vec3 min = two->GetMin(), max = two->GetMax();
		if (one.x < min.x || one.y < min.y || one.z < min.z) {
			collision = false;
		}
		else if (one.x > max.x || one.y > max.y || one.z > max.z) {
			collision = false;
		}

		for (int i = 0; i < 3; i++) {
			if (closestPoint[i] < min[i]) {
				closestPoint[i] = min[i];
			}
			if (closestPoint[i] > max[i]) {
				closestPoint[i] = max[i];
			}
		}

		return Data(collision, closestPoint);
	}

	Data Point_OBB(glm::vec3 one, BoundingBoxCollider* two) {
		bool collision = true;
		glm::vec3 closestPoint = one;

		glm::mat3 orientation(two->attachment->GetTransform()->getRotationMatrix());
		closestPoint = two->GetCenter();
		glm::vec3 halfSize = two->GetHalfSize();
		glm::vec3 dir = one - closestPoint;

		for (int i = 0; i < 3; ++i) {
			glm::vec3 axis(orientation[i][0], orientation[i][1], orientation[i][2]);
			float dist = glm::dot(dir, axis);
			if (dist > halfSize[i]) {
				dist = halfSize[i];
				collision = false;
			}
			if (dist < -halfSize[i]) {
				dist = -halfSize[i];
				collision = false;
			}
			closestPoint += axis * dist;
		}

		return Data(collision, closestPoint);
	}

	Data Point_Sphere(glm::vec3 one, SphereCollider* two) {
		bool collision = glm::distance(one, two->GetCenter()) <= two->GetRadius();
		glm::vec3 closestPoint = glm::normalize(one - two->GetCenter()); *two->GetRadius();
		return Data(collision, closestPoint)
	}

	// --- Intervales ---
	glm::vec2 GetInterval(Triangle t, glm::vec3 axis) {
		glm::vec2 res(glm::dot(axis, t[0]));
		for (int i = 1; i < 3; i++) {
			float tmp = glm::dot(axis, t[i]);
			res.x = (res.x < tmp) ? res.x : tmp;
			res.y = (res.y > tmp) ? res.y : tmp;
		}
		return res;
	}

	glm::vec2 GetInterval(const BoundingBoxCollider* one, glm::vec3 axis, bool oriented) {
		glm::vec3 v[8];
		if (!oriented) {
			glm::vec3 i = one->GetMin();
			glm::vec3 a = one->GetMax();

			v[0] = glm::vec3(i.x, a.y, a.z);
			v[1] = glm::vec3(i.x, a.y, i.z);
			v[2] = glm::vec3(i.x, i.y, a.z);
			v[3] = glm::vec3(i.x, i.y, i.z);
			v[4] = glm::vec3(a.x, a.y, a.z);
			v[5] = glm::vec3(a.x, a.y, i.z);
			v[6] = glm::vec3(a.x, i.y, a.z);
			v[7] = glm::vec3(a.x, i.y, i.z);
		}
		else {
			glm::vec3 center = one->GetCenterOriented();
			glm::vec3 size = one->GetHalfSize();
			glm::mat4 orientation = one->attachment->GetTransform()->getRotationMatrix();

			glm::vec3 a[] = {
				glm::vec3(orientation[0][0], orientation[0][1], orientation[0][2]),
				glm::vec3(orientation[1][0], orientation[1][1], orientation[1][2]),
				glm::vec3(orientation[2][0], orientation[2][1], orientation[2][2]),
			};

			v[0] = center + a[0] * size[0] + a[1] * size[1] + a[2] * size[2];
			v[1] = center - a[0] * size[0] + a[1] * size[1] + a[2] * size[2];
			v[2] = center + a[0] * size[0] - a[1] * size[1] + a[2] * size[2];
			v[3] = center + a[0] * size[0] + a[1] * size[1] - a[2] * size[2];
			v[4] = center - a[0] * size[0] - a[1] * size[1] - a[2] * size[2];
			v[5] = center + a[0] * size[0] - a[1] * size[1] - a[2] * size[2];
			v[6] = center - a[0] * size[0] + a[1] * size[1] - a[2] * size[2];
			v[7] = center - a[0] * size[0] - a[1] * size[1] + a[2] * size[2];
		}

		glm::vec2 res(glm::dot(axis, v[0]));

		for (int i = 1; i < 8; ++i) {
			float p = glm::dot(axis, v[i]);
			res.x = (p < res.x) ? p : res.x;
			res.y = (p > res.y) ? p : res.y;
		}
		return res;
	}

	// --- Overlap on axis ---



	bool AxisOverlap(const BoundingBoxCollider* one, const BoundingBoxCollider* two, glm::vec3 axis, BBMode mode) {
		glm::vec2 a, b;
		if (mode == AABB_AABB) {
			a = GetInterval(one, axis, false);
			b = GetInterval(two, axis, false);
		}
		else if (mode == OBB_OBB) {
			a = GetInterval(one, axis, true);
			b = GetInterval(two, axis, true);
		}
		else if (mode == AABB_OBB) {
			a = GetInterval(one, axis, false);
			b = GetInterval(two, axis, true);
		}
		else {
			a = GetInterval(one, axis, true);
			b = GetInterval(two, axis, false);
		}

		return ((b.x <= a.y) && (a.x <= b.y));
	}

	bool AxisOverlap(const BoundingBoxCollider* bb, Triangle t, glm::vec3 axis, bool oriented) {
		glm::vec2 a = GetInterval(bb, axis, oriented);
		glm::vec2 b = GetInterval(t, axis);
		return ((b.x <= a.y) && (a.x <= b.y));
	}

	bool AxisOverlap(Triangle t1, Triangle t2, glm::vec3 axis, bool oriented) {
		glm::vec2 a = GetInterval(t1, axis);
		glm::vec2 b = GetInterval(t2, axis);
		return ((b.x <= a.y) && (a.x <= b.y));
	}
};

#endif // !__COLLISION_DETECTION_HPP__
