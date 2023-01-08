#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <glm/glm.hpp>
#include <limits>
#include <Engine/GameObject.hpp>
#include <Engine/Component/Component.hpp>
#include <Engine/Component/Transformation.hpp>
#include <Engine/Component/Model.hpp>
#include <Engine/Shader.hpp>
#include <Graphics/Framebuffer.hpp>

/// <summary>
/// Camera Object to draw the scene.
/// </summary>
class Camera : public Component
{
public:
	/// <summary>
	/// The type of camera.
	/// </summary>
	enum Type
	{
		RENDER,
		DEPTH_STENCIL
	};

	/// <summary>
	/// The settings of the camera.
	/// </summary>
	struct Settings
	{
		double aspect;
		double fov;
		double near;
		double far;
		double size;
		bool isOrtho;

		Settings() {
		}

		/// <summary>
		/// Set ortho data.
		/// </summary>
		/// <param name="aspect">Aspect ratio</param>
		/// <param name="near">the near plane</param>
		/// <param name="far">the far plane</param>
		/// <param name="size">the ortho size.</param>
		/// <returns>copy of self.</returns>
		Settings ortho(double aspect = 16.0 / 9.0, double near = 0.01, double far = 1000.0, double size = 1.0) {
			this->near = near;
			this->far = far;
			this->aspect = aspect;
			this->isOrtho = true;
			this->size = size;
			return *this;
		}

		/// <summary>
		/// Set perspective data,
		/// </summary>
		/// <param name="aspect">Aspect ratio</param>
		/// <param name="near">the near plance</param>
		/// <param name="far">the far plane</param>
		/// <param name="fov">the FOV</param>
		/// <returns>copy of self.</returns>
		Settings perspect(double aspect = 16.0 / 9.0, double near = 0.01, double far = 1000.0, double fov = 90.0) {
			this->near = near;
			this->far = far;
			this->aspect = aspect;
			this->isOrtho = false;
			this->fov = fov;
			return *this;
		}

		/// <summary>
		/// Generate a Orthographic camera Settings.
		/// </summary>
		/// <param name="aspect">Aspect ratio</param>
		/// <param name="near">the near plane</param>
		/// <param name="far">the far plane</param>
		/// <param name="size">the ortho size.</param>
		/// <returns>Generated Settings.</returns>
		static Settings orthographic(double aspect = 16.0 / 9.0, double near = 0.01, double far = 1000.0, double size = 1.0) {
			Settings res = Settings();
			return res.ortho(aspect, near, far, size);
		}

		/// <summary>
		/// Generate a Orthographic camera Settings.
		/// </summary>
		/// <param name="aspect">Aspect ratio</param>
		/// <param name="near">the near plane</param>
		/// <param name="far">the far plane</param>
		/// <param name="fov">the fov.</param>
		/// <returns>Generated Settings.</returns>
		static Settings perspective(double aspect = 16.0 / 9.0, double near = 0.01, double far = 1000.0, double fov = 90.0) {
			Settings res = Settings();
			return res.perspect(aspect, near, far, fov);
		}
	};


protected:

	/// <summary>
	/// Frustum plane state of the camera.
	/// </summary>
	struct FrustumPlaneState {
		bool left = false, right = false, top = false, bottom = false, near = false, far = false;
	};

	/// <summary>
	/// Frustum Data of the Perspective Camera
	/// </summary>
	struct FrustumDataPerpective {
		glm::vec4 cols[4] = { glm::vec4(0.0), glm::vec4(0.0), glm::vec4(0.0),glm::vec4(0.0) };

		FrustumDataPerpective() {
		}

		FrustumDataPerpective(glm::mat4 pv) {
			for (int i = 0; i < 4; i++) {
				glm::vec4 col = glm::vec4(0);
				for (int j = 0; j < 4; j++) {
					col[j] = pv[j][i];
				}
				cols[i] = col;
			}
		}
	};

	/// <summary>
	/// Frustum Data of the Orthogonal Camera
	/// </summary>
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

	Type type = Type::RENDER;
	//The camera setting
	Settings settings;

	//The projection matrix.
	glm::mat4 projection;

	//The frustums data.
	FrustumDataPerpective frustumDataPerspective;
	FrustumDataOrtho frustumDataOrtho;

	// The framebuffer of the camera.
	Framebuffer framebuffer; 

	//The material of the camera, for special effect.
	std::string shadername;
	IMaterial* renderMaterial;

public:
	/// <summary>
	/// Create a camera
	/// </summary>
	/// <param name="settings">The settings of the camera (Perspective or Orthogonal)</param>
	/// <param name="type">The type of camera.</param>
	/// <param name="shadername">The shadername (if needed)</param>
	Camera(Settings settings = Settings::perspective(), Type type = Type::RENDER, std::string shadername = "")
	{
		this->settings = settings;
		this->type = type;
		if (shadername.size() > 0) {
			 this->renderMaterial = new IMaterial(shadername);
		}
		else {
			this->renderMaterial = nullptr;
		}
	}

	/// <summary>
	/// Add childs component after the Gameobject is attached.
	/// </summary>
	void PostAttachment() override {
		if(renderMaterial != nullptr)
			this->attachment->addComponent(this->renderMaterial);
		framebuffer.Generate();
		UpdateData();
	}



	~Camera() {
	}

	/// <summary>
	/// Return the projection matrix
	/// </summary>
	/// <returns>the projection matrix</returns>
	glm::mat4 GetProjection() {
		return this->projection;
	}

	/// <summary>
	/// Return the view Matrix
	/// </summary>
	/// <returns>The view matrix</returns>
	glm::mat4 GetView() {
		Transformation* transform = this->attachment->GetTransform();
		if (transform != nullptr) {
			transform->Update();
			return glm::lookAt(transform->getPosition(), transform->getPosition() + transform->getFrontVector(), transform->getUpVector());
		}
		return glm::mat4(1.0);
	}

	/// <summary>
	/// Return the camera type
	/// </summary>
	/// <returns>The camera type</returns>
	Type GetType() {
		return this->type;
	}

	/// <summary>
	/// Set the camera type
	/// </summary>
	/// <param name="type">The new camera type</param>
	void SetType(Type type) {
		this->type = type;
	}

	/// <summary>
	/// Return the camera settings
	/// </summary>
	/// <returns>The camera settings</returns>
	Settings GetSettings() {
		return this->settings;
	}

	/// <summary>
	/// Set the camera settings
	/// </summary>
	/// <param name="settings">the new camera settings</param>
	void SetSettings(Settings settings) {
		this->settings = settings;
		UpdateProjection();
	}

	/// <summary>
	/// Update projection and frustum data.
	/// </summary>
	void UpdateData()
	{
		UpdateProjection();
		UpdateFrustum();
	}

	/// <summary>
	/// Update the projection data.
	/// </summary>
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
			this->projection = glm::perspectiveLH(glm::radians(this->settings.fov), this->settings.aspect, this->settings.near, this->settings.far);
		}
	}

	/// <summary>
	/// Update the frustum culling data.
	/// </summary>
	void UpdateFrustum() {
		if (this->settings.isOrtho) {
			double w = this->settings.aspect * this->settings.size;
			double h = this->settings.size;

			frustumDataOrtho = FrustumDataOrtho(glm::vec3(-w, -h, this->settings.near), glm::vec3(w, h, this->settings.far));
		}
		else
		{
			frustumDataPerspective = FrustumDataPerpective( this->projection* this->GetView());
		}
	}

	/// <summary>
	/// Check if an point is in view
	/// </summary>
	/// <param name="point">The point to test</param>
	/// <returns>True if is in view, else False</returns>
	bool IsInView(glm::vec3 point)
	{
		FrustumPlaneState fps = GetFrustumPlaneState(point);

		return fps.left && fps.right && fps.bottom && fps.top && fps.near && fps.far;
	}

	/// <summary>
	/// Check if a bounding box is in view.
	/// </summary>
	/// <param name="min">Min of the AABB</param>
	/// <param name="max">Max of the AABB</param>
	/// <returns>If is in view</returns>
	bool IsInView(glm::vec4 min, glm::vec4 max) {
		return IsInView(glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, max.y, max.z));
	}

	/// <summary>
	/// Check if a bounding box is in view
	/// </summary>
	/// <param name="min">Min of the AABB</param>
	/// <param name="max">Max of the AABB</param>
	/// <returns>Is in view</returns>
	bool IsInView(glm::vec3 min, glm::vec3 max)
	{
		//printf("min : [%f, %f, %f] | max : [%f, %f, %f]\n", min.x, min.y, min.z, max.x, max.y, max.z);
		FrustumPlaneState minState = GetFrustumPlaneState(min);
		FrustumPlaneState maxState = GetFrustumPlaneState(max);


		//printf("MIN | L : %d | R : %d | B : %d | T : %d | N : %d | F : %d\n", minState.left, minState.right, minState.bottom, minState.top, minState.near, minState.far);
		//printf("MAX | L : %d | R : %d | B : %d | T : %d | N : %d | F : %d\n", maxState.left, maxState.right, maxState.bottom, maxState.top, maxState.near, maxState.far);

		bool lr = true;
		bool tb = true;
		bool nf = true;

		return lr && tb && nf;
	}


	/// <summary>
	/// Return the Material of the camera
	/// </summary>
	/// <returns>The material</returns>
	IMaterial* GetRenderMaterial() {
		return this->renderMaterial;
	}

	/// <summary>
	/// Return the Framebuffer of the camera
	/// </summary>
	/// <returns>The framebuffer</returns>
	GLuint GetFrameBuffer() {
		return this->framebuffer.GetFramebuffer();
	}

	/// <summary>
	/// Return the color texture of the framebuffer
	/// </summary>
	/// <returns>The color texture.</returns>
	GLuint GetColorTexture() {
		return this->framebuffer.GetTexColor();
	}

	/// <summary>
	/// Return the depth texture of the framebuffer
	/// </summary>
	/// <returns>The depth texture.</returns>
	GLuint GetDepthTexture() {
		return this->framebuffer.GetTexDepth();
	}
	/*GLuint GetStencilTexture() {
		return this->framebuffer.GetTexStencil();
	}*/

	/// <summary>
	/// Return the position of the camera
	/// </summary>
	/// <returns>The position</returns>
	glm::vec3 GetPosition(){
		return this->attachment->GetTransform()->getPosition();
	}

protected:
	/// <summary>
	/// Get the frustum plane state for a point.
	/// </summary>
	/// <param name="point">The point to test.</param>
	/// <returns>The frustum plane data</returns>
	FrustumPlaneState GetFrustumPlaneState(glm::vec3 point)
	{
		FrustumPlaneState res;

		if (this->settings.isOrtho) {
			glm::vec4 v = this->GetView() * glm::vec4(point, 1.0);
			v = this->projection * v;

			res.left = v.x >= frustumDataOrtho.min.x;
			res.right = v.x <= frustumDataOrtho.max.x;
			res.bottom = v.y >= frustumDataOrtho.min.y;
			res.top = v.y <= frustumDataOrtho.max.y;
			res.near = -v.z >= frustumDataOrtho.min.z;
			res.far = v.z <= frustumDataOrtho.max.z;
		}
		else {
			glm::vec4 v = glm::vec4(point, 1.0);
			glm::vec4 calc = glm::vec4(0);

			for (int i = 0; i < 4; i++) {
				calc[i] = glm::dot(v, frustumDataPerspective.cols[i]);
			}

			res.left =  -calc.w < calc.x ;
			res.right = calc.x < calc.w;
			res.bottom = -calc.w < calc.y;
			res.top = calc.y < calc.w;
			res.near = -calc.w < calc.z;
			res.far = calc.z < calc.w;
		}


		return  res;
	}

};

#endif