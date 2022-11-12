#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <glm/glm.hpp>
#include <limits>
#include <Engine/GameObject.hpp>
#include <Engine/Component/Component.hpp>
#include <Engine/Component/Transformation.hpp>

class Camera : public Component
{
public:
	enum Type
	{
		CAMERA,
		TEXTURE
	};

	struct Settings
	{
		double aspect;
		double fov;
		double near;
		double far;
		double size;
		bool isOrtho;

		Settings ortho(double aspect = 16.0 / 9.0, double near = 0.01, double far = 1000.0, double size = 1.0) {
			this->near = near;
			this->far = far;
			this->aspect = aspect;
			this->isOrtho = true;
			this->size = size;
			return *this;
		}

		Settings perspect(double aspect = 16.0 / 9.0, double near = 0.01, double far = 1000.0, double fov = 90.0) {
			this->near = near;
			this->far = far;
			this->aspect = aspect;
			this->isOrtho = false;
			this->fov = fov;
			return *this;
		}

		static Settings orthographic(double aspect = 16.0 / 9.0, double near = 0.01, double far = 1000.0, double size = 1.0) {
			Settings res;
			return res.ortho(aspect, near, far, size);
		}

		static Settings perspective(double aspect = 16.0 / 9.0, double near = 0.01, double far = 1000.0, double fov = 90.0) {
			Settings res;
			return res.perspect(aspect, near, far, fov);
		}
	};

	struct Data
	{
		glm::mat4 view;
	};

protected:

	struct FrustumDataPerpective {
		glm::vec4 cols[4];
		FrustumDataPerpective() {
			for (int i = 0; i < 4; i++) {
				cols[i] = glm::vec4(0.0);
			}
		}
		FrustumDataPerpective(glm::mat4 pv) {
			for (int i = 0; i < 4; i++) {
				glm::vec4 col;
				for (int j = 0; j < 4; j++) {
					col[j] = pv[j][i];
				}
				cols[i] = col;
			}
		}
	};

	struct FrustumDataOrtho {
		glm::vec3 min, max;

		FrustumDataOrtho() {
			this->min = max = glm::vec4(0);
		}

		FrustumDataOrtho(glm::vec3 min, glm::vec3 max) {
			this->min = min;
			this->max = max;
		}
	};


	GameObject* attachment;
	Transformation* transform;
	Type type = Type::CAMERA;
	Settings settings;

	glm::mat4 projection;

	FrustumDataPerpective frustumDataPerspective;
	FrustumDataOrtho frustumDataOrtho;

public:

	Camera(GameObject* attachment, Settings settings = Settings::perspective(), Type type = Type::CAMERA)
	{
		this->attachment = attachment;
		this->transform = this->attachment->getFirstComponentByType<Transformation>();
		this->settings = settings;
		this->type = type;

		UpdateData();
	}

	glm::mat4 GetProjection() {
		return this->projection;
	}

	glm::mat4 GetView() {
		if (this->transform != nullptr)
			return this->transform->getMatrix();
		return glm::mat4(1.0);
	}

	Type GetType() {
		return this->type;
	}

	void SetType(Type type) {
		this->type = type;
	}

	Settings GetSettings() {
		return this->settings;
	}

	void SetSettings(Settings settings) {
		this->settings = settings;
		UpdateProjection();
	}

	void UpdateData()
	{
		UpdateProjection();
		UpdateFrustum();
	}

	void UpdateProjection()
	{
		if (this->settings.isOrtho)
		{
			double w = this->settings.aspect * this->settings.size;
			double h = this->settings.size;

			this->projection = glm::ortho<double>(-w, w, -h, h, this->settings.near, this->settings.far);
		}
		else
		{
			this->projection = glm::perspective(glm::radians(this->settings.fov), this->settings.aspect, this->settings.near, this->settings.far);
		}
	}

	void UpdateFrustum() {
		if (this->settings.isOrtho) {
			double w = this->settings.aspect * this->settings.size;
			double h = this->settings.size;

			frustumDataOrtho = FrustumDataOrtho(glm::vec3(-w, -h, this->settings.near), glm::vec3(w, h, this->settings.far));
		}
		else
		{
			frustumDataPerspective = FrustumDataPerpective(this->projection * this->GetView());
		}
	}


	bool IsInView(Transformation* transform)
	{
		if (transform == nullptr)
			return false;



		if (this->settings.isOrtho) {
			glm::vec4 v = this->GetView() * glm::vec4(transform->getPosition(),1.0);
			v = this->projection * v;

			return v.x >= frustumDataOrtho.min.x && v.x <= frustumDataOrtho.max.x &&
				v.y >= frustumDataOrtho.min.y && v.y <= frustumDataOrtho.max.y &&
				-v.z >= frustumDataOrtho.min.z && v.z <= frustumDataOrtho.max.z;
		}
		else {
			glm::vec4 v = glm::vec4(transform->getPosition(), 1);
			glm::vec4 calc = glm::vec4(0);

			for (int i = 0; i < 4; i++) {
				calc[i] = glm::dot(v, frustumDataPerspective.cols[i]);
			}

			return (-calc.w < calc.x) && (calc.x < calc.w) &&
				(-calc.w < calc.y) && (calc.y < calc.w) &&
				(0 < calc.z) && (calc.z < calc.w);
		}
		return false;
	}
};

#endif