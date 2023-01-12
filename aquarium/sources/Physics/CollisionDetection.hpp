#ifndef __COLLISION_DETECTION_HPP__
#define __COLLISION_DETECTION_HPP__

#include <glm/glm.hpp>
#include <Physics/Collider/BoundingBoxCollider.hpp>
#include <Physics/Collider/SphereCollider.hpp>
#include <Engine/GameObject.hpp>


/// <summary>
/// Collision detection tools.
/// </summary>
class CollisionDetection {
public:

	/// <summary>
	/// Bounding Box mode for the BoudingBoxCollider.
	/// </summary>
	enum BBMode {
		M_AABB_AABB,
		M_AABB_OBB,
		M_OBB_AABB,
		M_OBB_OBB
	};


	/// <summary>
	/// Data of a collisions
	/// </summary>
	struct Data {
		/// <summary>
		/// Data of the collisions
		/// </summary>
		/// <param name="collision">If there are a collision or not.</param>
		/// <param name="closestPoint">The closest point hit of the object.</param>

		bool collision = false;
		glm::vec3 closestPoint;

		Data(bool c, glm::vec3 cp) {
			this->collision = c;
			this->closestPoint = cp;
		}
	};

	/// <summary>
	/// A triangle object with 3 glm::vec3 as point.
	/// </summary>
	struct Triangle {
		/// <summary>
		/// A triangle object with 3 glm::vec3 as point.
		/// </summary>
		/// <param name="points">A list of 3 glm::vec3.</param>
		glm::vec3 points[3];

		Triangle(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2) {
			this->points[0] = p0;
			this->points[0] = p1;
			this->points[0] = p2;
		}
		/// <summary>
		/// Override the [] operator to access directly points[].
		/// </summary>
		glm::vec3 operator[](size_t idx) {
			return points[idx];
		}
	};

	struct Plane {
		glm::vec3 normal;
		float distance;

		Plane() { normal = glm::vec3(1, 0, 0); }
		Plane(glm::vec3 norm, float d) {
			this->normal = normal;
			this->distance = d;
		}
	};

	static glm::vec3 Project(glm::vec3 length, glm::vec3 dir) {
		return dir * (glm::dot(length, dir) / glm::dot(dir, dir));
	}
	static glm::vec3 Barycentric(glm::vec3 p, Triangle t) {
		glm::vec3 ap = p - t[0];
		glm::vec3 bp = p - t[1];
		glm::vec3 cp = p - t[1];

		glm::vec3 ab = t[1] - t[0];
		glm::vec3 ac = t[1] - t[0];
		glm::vec3 bc = t[1] - t[1];
		glm::vec3 cb = t[1] - t[1];
		glm::vec3 ca = t[0] - t[1];

		glm::vec3 v = ab - CollisionDetection::Project(ab, cb);
		float a = 1.0f - (glm::dot(v, ap) / glm::dot(v, ab));

		v = bc - CollisionDetection::Project(bc, ac);
		float b = 1.0f - (glm::dot(v, bp) / glm::dot(v, bc));

		v = ca - CollisionDetection::Project(ca, ab);
		float c = 1.0f - (glm::dot(v, cp) / glm::dot(v, ca));

		return glm::vec3(a, b, c);
	}

	static bool EpsilonCompare(double x, double y) {
		return (fabsf(x - y) <= FLT_EPSILON * fmaxf(1.0f, fmaxf(fabsf(x), fabsf(y))));
	}
	/// <summary>
	/// Detection function that take two Gameobject and return Data
	/// </summary>
	/// <param name="one">First GameObject to detect collision.</param>
	/// <param name="two">Second GameObject to detect collision.</param>
	/// <returns>The Data of the collision.</returns>
	static Data Detection(GameObject* one, GameObject* two) {

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
				return Detection(s, collider2);
			}
		}
	}

	/// <summary>
	/// Detection function that take One BoundingBoxCollider and one ICollider.
	/// </summary>
	/// <param name="one">A BoundingBoxCollider to detect collision.</param>
	/// <param name="two">A ICollider to detect collision.</param>
	/// <returns>The Data of the collision.</returns>
	static Data Detection(BoundingBoxCollider* one, ICollider* two) {
		if (two->ColliderType() == ICollider::BoundingBox)
		{
			BoundingBoxCollider* bb = dynamic_cast<BoundingBoxCollider*>(two);
			if (bb != nullptr) {
				return Detection(one, bb, M_OBB_OBB);
			}
		}
		else if (two->ColliderType() == ICollider::Sphere) {
			SphereCollider* s = dynamic_cast<SphereCollider*>(two);
			if (s != nullptr) {
				return Detection(s, one, true);

			}
		}
	}

	/// <summary>
	/// Detection function that take One SphereCollider and one ICollider.
	/// </summary>
	/// <param name="one">A SphereCollider to detect collision.</param>
	/// <param name="two">A ICollider to detect collision.</param>
	/// <returns>The Data of the collision.</returns>
	static Data Detection(SphereCollider* one, ICollider* two) {
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


	/// <summary>
	/// Detection function that take two BoundingBoxCollider and one BBMode
	/// </summary>
	/// <param name="one">First BoundingBoxCollider to detect collision.</param>
	/// <param name="two">Second BoundingBoxCollider to detect collision.</param>
	/// <param name="mode">The mode of the Detection</param>
	/// <returns>The Data of the collision.</returns>
	static Data Detection(BoundingBoxCollider* one, BoundingBoxCollider* two, BBMode mode) {
		if (mode == M_AABB_AABB)
			return AABB_AABB(one, two);
		else if (mode == M_OBB_OBB)
			return OBB_OBB(one, two);
		else if (mode == M_AABB_OBB)
			return OBB_AABB(two, one);
		else
			return OBB_AABB(one, two);
	}


	/// <summary>
	///  Detection function that take One SphereCollider and one BoundingBoxCollider oriented or not.
	/// </summary>
	///  <param name="one">A SphereCollider to detect collision.</param>
	/// <param name="two">A BoundingBoxCollider to detect collision.</param>
	/// <param name="oriented">If the bounding box is oritented or not</param>
	/// <returns>The Data of the collision.</returns>
	static Data Detection(SphereCollider* one, BoundingBoxCollider* two, bool oriented) {
		if (oriented) {
			return Sphere_OBB(one, two);
		}
		else {
			return Sphere_AABB(one, two);
		}
	}

	/// <summary>
	///  Detection function that take One Point and one BoundingBoxCollider oriented or not.
	/// </summary>
	///  <param name="one">A point to detect collision.</param>
	/// <param name="two">A BoundingBoxCollider to detect collision.</param>
	/// <param name="oriented">If the bounding box is oritented or not</param>
	/// <returns>The Data of the collision.</returns>
	static Data Detection(glm::vec3 one, BoundingBoxCollider* two, bool oriented) {
		if (oriented) {
			return Point_OBB(one, two);
		}
		else {
			return Point_AABB(one, two);
		}
	}

	/// <summary>
	///  Detection function that take One Point and one SphereCollider .
	/// </summary>
	///  <param name="one">A point to detect collision.</param>
	/// <param name="two">A SphereCollider to detect collision.</param>
	/// <returns>The Data of the collision.</returns>
	static Data Detection(glm::vec3 one, SphereCollider* two) {
		return Point_Sphere(one, two);
	}

	/// <summary>
	/// Detection function that take two SphereCollider
	/// </summary>
	/// <param name="one">First SphereCollider to detect collision.</param>
	/// <param name="two">Second SphereCollider to detect collision.</param>
	/// <returns>The Data of the collision.</returns>
	static Data Detection(SphereCollider* one, SphereCollider* two) {
		return Sphere_Sphere(one, two);
	}

	// --- SPECIFIC ---

	/// <summary>
	/// Compute collision for two AABB Bounding box.
	/// </summary>
	/// <param name="one">A BoundingBoxCollider</param>
	/// <param name="two">A BoundingBoxCollider</param>
	/// <returns>The Data of the collision.</returns>
	static Data AABB_AABB(BoundingBoxCollider* one, BoundingBoxCollider* two) {
		glm::vec3 closestPoint;
		glm::vec3 c = one->GetCenter();
		glm::vec3 aMin = one->GetMin(), aMax = one->GetMax();
		glm::vec3 bMin = one->GetMin(), bMax = one->GetMax();

		bool collision = (aMin.x <= bMax.x && aMax.x >= bMin.x) &&
			(aMin.y <= bMax.y && aMax.y >= bMin.y) &&
			(aMin.z <= bMax.z && aMax.z >= bMin.z);

		if (glm::distance(c, bMin) <= glm::distance(c, bMax)) {
			Data tmp = Point_AABB(bMin, one);
			closestPoint = tmp.closestPoint;
		}
		else {
			Data tmp = Point_AABB(bMax, one);
			closestPoint = tmp.closestPoint;
		}

		return Data(collision, closestPoint);
	}

	/// <summary>
	/// Compute collision for two OBB Bounding box.
	/// </summary>
	/// <param name="one">A BoundingBoxCollider</param>
	/// <param name="two">A BoundingBoxCollider</param>
	/// <returns>The Data of the collision.</returns>
	static Data OBB_OBB(BoundingBoxCollider* one, BoundingBoxCollider* two) {
		bool collision = true;
		glm::vec3 closestPoint = glm::vec3(0);

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

		for (int i = 0; i < 15 && collision; ++i) {
			if (!AxisOverlap(one, two, test[i], M_OBB_OBB)) {
				collision = false;
			}
		}
		return Data(collision, closestPoint);
	}

	/// <summary>
	/// Compute collision for a AABB Bounding box and a OBB Bounding box (in that order).
	/// </summary>
	/// <param name="one">A AABB BoundingBoxCollider </param>
	/// <param name="two">A OBB BoundingBoxCollider</param>
	/// <returns>The Data of the collision.</returns>
	static Data AABB_OBB(BoundingBoxCollider* one, BoundingBoxCollider* two) {
		return OBB_AABB(two, one);
	}

	/// <summary>
	/// Compute collision for a OBB Bounding box and a AABB Bounding box (in that order).
	/// </summary>
	/// <param name="one">A OBB BoundingBoxCollider </param>
	/// <param name="two">A AABB BoundingBoxCollider</param>
	/// <returns>The Data of the collision.</returns>
	static Data OBB_AABB(BoundingBoxCollider* one, BoundingBoxCollider* two) {
		bool collision = true;
		glm::vec3 closestPoint = glm::vec3(0);

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

		for (int i = 0; i < 15 && collision; ++i) {
			if (!AxisOverlap(one, two, test[i], M_OBB_AABB)) {
				collision = false;
			}
		}
		return Data(collision, closestPoint);
	}

	/// <summary>
	/// Compute collision for a AABB Bounding box and a Sphere.
	/// </summary>
	/// <param name="one">A AABB BoundingBoxCollider </param>
	/// <param name="two">A SphereCollider</param>
	/// <returns>The Data of the collision.</returns>
	static Data AABB_Sphere(BoundingBoxCollider* one, SphereCollider* two) {
		return Sphere_AABB(two, one);
	}

	static Data Sphere_AABB(SphereCollider* one, BoundingBoxCollider* two) {
		glm::vec3 oneCenter = one->GetCenter();
		Data d = Point_AABB(oneCenter, two);
		if (!d.collision) {
			d.collision = glm::distance(oneCenter, d.closestPoint) < one->GetRadius() * one->GetRadius();
		}
		return d;
	}

	/// <summary>
	/// Compute collision for a OBB Bounding box and a Sphere.
	/// </summary>
	/// <param name="one">A OBB BoundingBoxCollider </param>
	/// <param name="two">A SphereCollider</param>
	/// <returns>The Data of the collision.</returns>
	static Data OBB_Sphere(BoundingBoxCollider* one, SphereCollider* two) {
		return Sphere_AABB(two, one);
	}

	/// <summary>
	/// Compute collision for a Sphere and a OBB Bounding box.
	/// </summary>
	/// <param name="one">A SphereCollider.</param>
	/// <param name="two">A OBB BoundingBoxCollider.</param>
	/// <returns>The Data of the collision.</returns>
	static Data Sphere_OBB(SphereCollider* one, BoundingBoxCollider* two) {
		glm::vec3 oneCenter = one->GetCenter();
		Data d = Point_OBB(oneCenter, two);
		if (!d.collision) {
			d.collision = glm::distance(oneCenter, d.closestPoint) < one->GetRadius() * one->GetRadius();
		}
		return d;
	}



	/// <summary>
	/// Compute collision for two Sphere.
	/// </summary>
	/// <param name="one">A SphereCollider</param>
	/// <param name="two">A SphereCollider</param>
	/// <returns>The Data of the collision.</returns>
	static Data Sphere_Sphere(SphereCollider* one, SphereCollider* two) {
		float dist = glm::distance(one->GetCenter(), two->GetCenter());
		float sum = one->GetRadius() + two->GetRadius();
		glm::vec3 n = glm::normalize(two->GetCenter() - one->GetCenter());
		return Data(dist <= sum, n * (float)one->GetRadius());
	}

	// --- Points ---

	/// <summary>
	/// Compute collision for a point and a AABB Bounding box .
	/// </summary>
	/// <param name="one">A Point.</param>
	/// <param name="two">A AABB BoundingBoxCollider.</param>
	/// <returns>The Data of the collision.</returns>
	static Data Point_AABB(glm::vec3 one, BoundingBoxCollider* two) {
		bool collision = true;
		glm::vec3 closestPoint = one;

		glm::vec3 min, max;
		two->GetMinMax(min, max);

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

	/// <summary>
	/// Compute collision for a point and a OBB Bounding box .
	/// </summary>
	/// <param name="one">A Point.</param>
	/// <param name="two">A OBB BoundingBoxCollider.</param>
	/// <returns>The Data of the collision.</returns>
	static Data Point_OBB(glm::vec3 one, BoundingBoxCollider* two) {
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

	/// <summary>
	/// Compute collision for a point and a Sphere.
	/// </summary>
	/// <param name="one">A Point.</param>
	/// <param name="two">A SphereCollider.</param>
	/// <returns>The Data of the collision.</returns>
	static Data Point_Sphere(glm::vec3 one, SphereCollider* two) {
		bool collision = glm::distance(one, two->GetCenter()) <= two->GetRadius();
		glm::vec3 closestPoint = glm::normalize(one - two->GetCenter()) * (float)two->GetRadius();
		return Data(collision, closestPoint);
	}

	/// <summary>
	/// Compute the lenght of a ray and a AABB bounding box, if -1 ray do not hit
	/// </summary>
	/// <param name="origin">Origin of the ray</param>
	/// <param name="direction">Direction of the ray</param>
	/// <param name="bb">Bounding box</param>
	/// <returns>The lenght of the ray, if -1 : ray do not hit</returns>
	static double Ray_AABB(glm::vec3 origin, glm::vec3 direction, BoundingBoxCollider* bb) {
		double t1 = (bb->GetMin().x - origin.x) / (EpsilonCompare(direction.x, 0.0f) ? 0.00001f : direction.x);
		double t2 = (bb->GetMax().x - origin.x) / (EpsilonCompare(direction.x, 0.0f) ? 0.00001f : direction.x);
		double t3 = (bb->GetMin().y - origin.y) / (EpsilonCompare(direction.y, 0.0f) ? 0.00001f : direction.y);
		double t4 = (bb->GetMax().y - origin.y) / (EpsilonCompare(direction.y, 0.0f) ? 0.00001f : direction.y);
		double t5 = (bb->GetMin().z - origin.z) / (EpsilonCompare(direction.z, 0.0f) ? 0.00001f : direction.z);
		double t6 = (bb->GetMax().z - origin.z) / (EpsilonCompare(direction.z, 0.0f) ? 0.00001f : direction.z);

		double tmin = fmaxf(fmaxf(fminf(t1, t2), fminf(t3, t4)), fminf(t5, t6));
		double tmax = fminf(fminf(fmaxf(t1, t2), fmaxf(t3, t4)), fmaxf(t5, t6));

		if (tmax < 0 || tmin > tmax) {
			return -1.0;
		}

		if (tmin < 0.0f) {
			return tmax;
		}

		return tmin;
	}

	/// <summary>
	/// Compute the lenght of a ray and a Plane, if -1 ray do not hit
	/// </summary>
	/// <param name="origin">Origin of the ray</param>
	/// <param name="direction">Direction of the ray</param>
	/// <param name="p">The plane</param>
	/// <returns>The lenght of the ray, if -1 : ray do not hit</returns>
	static double Ray_Plane(glm::vec3 origin, glm::vec3 direction, Plane p) {
		double nd = glm::dot(direction, p.normal);
		double on = glm::dot(origin, p.normal);

		if (nd >= 0.0f) {
			return -1.0f;
		}

		double t = (p.distance - on) / nd;

		// t must be positive
		if (t >= 0.0f) {

			return t;
		}

		return -1.0f;
	}

	/// <summary>
	/// Compute the lenght of a ray and a Triangle, if -1 ray do not hit
	/// </summary>
	/// <param name="origin">Origin of the ray</param>
	/// <param name="direction">Direction of the ray</param>
	/// <param name="t">The triangle</param>
	/// <returns>The lenght of the ray, if -1 : ray do not hit</returns>
	static double Ray_Triangle(glm::vec3 origin, glm::vec3 direction, Triangle t) {
		glm::vec3 n = glm::normalize(glm::cross(t[1] - t[0], t[2] - t[0]));
		Plane p(n, glm::dot(n, t[0]));
		double val = CollisionDetection::Ray_Plane(origin, direction, p);
		if (val < 0.0) {
			return -1.0;
		}

		glm::vec3 barycentre = Barycentric(origin + (direction * (float)val), t);
		if (barycentre.x >= 0.0f && barycentre.x <= 1.0f &&
			barycentre.y >= 0.0f && barycentre.y <= 1.0f &&
			barycentre.z >= 0.0f && barycentre.z <= 1.0f) {

			return val;
		}

		return -1.0;
	}


	// --- Intervales ---

	/// <summary>
	/// Get the Interval for axis overlap with a triangle and an axis.
	/// </summary>
	/// <param name="t">A triangle.</param>
	/// <param name="axis">The axis.</param>
	/// <returns>The interval of the axis</returns>
	static glm::vec2 GetInterval(Triangle t, glm::vec3 axis) {
		glm::vec2 res(glm::dot(axis, t[0]));
		for (int i = 1; i < 3; i++) {
			float tmp = glm::dot(axis, t[i]);
			res.x = (res.x < tmp) ? res.x : tmp;
			res.y = (res.y > tmp) ? res.y : tmp;
		}
		return res;
	}

	/// <summary>
	/// Get the Interval for axis overlap with a BoundingBox oriented or not and an axis.
	/// </summary>
	/// <param name="one">The boundingBox</param>
	/// <param name="axis">The axis.</param>
	/// <param name="oriented">If the bounding box is oriented.</param>
	/// <returns>The interval of the axis</returns>
	static glm::vec2 GetInterval(BoundingBoxCollider* one, glm::vec3 axis, bool oriented) {
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


	/// <summary>
	/// Compute the axis overlap between two Bounding box.
	/// </summary>
	/// <param name="one">A BoundingBoxCollider object.</param>
	/// <param name="two">A BoundingBoxCollider object.</param>
	/// <param name="axis">The Axis.</param>
	/// <param name="mode">The bounding Box mode of the two bouding box</param>
	/// <returns>Is the axis is overlaping</returns>
	static bool AxisOverlap(BoundingBoxCollider* one, BoundingBoxCollider* two, glm::vec3 axis, BBMode mode) {
		glm::vec2 a, b;
		if (mode == M_AABB_AABB) {
			a = GetInterval(one, axis, false);
			b = GetInterval(two, axis, false);
		}
		else if (mode == M_OBB_OBB) {
			a = GetInterval(one, axis, true);
			b = GetInterval(two, axis, true);
		}
		else if (mode == M_AABB_OBB) {
			a = GetInterval(one, axis, false);
			b = GetInterval(two, axis, true);
		}
		else {
			a = GetInterval(one, axis, true);
			b = GetInterval(two, axis, false);
		}

		return ((b.x <= a.y) && (a.x <= b.y));
	}

	/// <summary>
	/// Compute the axis overlap between a Bounding box and a triangle.
	/// </summary>
	/// <param name="bb">A BoundingBoxCollider object.</param>
	/// <param name="t">A Triangle object.</param>
	/// <param name="axis">The Axis.</param>
	/// <param name="oriented">Is the bouding box is oriented or not.</param>
	/// <returns>Is the axis is overlaping</returns>
	static bool AxisOverlap(BoundingBoxCollider* bb, Triangle t, glm::vec3 axis, bool oriented) {
		glm::vec2 a = GetInterval(bb, axis, oriented);
		glm::vec2 b = GetInterval(t, axis);
		return ((b.x <= a.y) && (a.x <= b.y));
	}

	/// <summary>
	/// Compute the axis overlap between a Bounding box and a triangle.
	/// </summary>
	/// <param name="t1">A Triangle object.</param>
	/// <param name="t2">A Triangle object.</param>
	/// <param name="axis">The Axis.</param>
	/// <returns>Is the axis is overlaping</returns>
	static bool AxisOverlap(Triangle t1, Triangle t2, glm::vec3 axis) {
		glm::vec2 a = GetInterval(t1, axis);
		glm::vec2 b = GetInterval(t2, axis);
		return ((b.x <= a.y) && (a.x <= b.y));
	}

};

#endif // !__COLLISION_DETECTION_HPP__
