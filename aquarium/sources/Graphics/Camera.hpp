#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <glm/glm.hpp>
#include <limits>
#include <Engine/GameObject.hpp>
#include <Engine/Component/Component.hpp>
#include <Engine/Component/Transformation.hpp>
#include <Engine/Component/Model.hpp>
#include <Engine/Shader.hpp>

class Camera : public Component
{
public:
	enum Type
	{
		RENDER,
		DEPTH_STENCIL
	};

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
			Settings res = Settings();
			return res.ortho(aspect, near, far, size);
		}

		static Settings perspective(double aspect = 16.0 / 9.0, double near = 0.01, double far = 1000.0, double fov = 90.0) {
			Settings res = Settings();
			return res.perspect(aspect, near, far, fov);
		}
	};

	struct Framebuffer {
		GLuint framebuffer;
		GLuint renderbuffer;
		GLuint tex_color;

	};


protected:

	struct FrustumPlaneState {
		bool left = false, right = false, top = false, bottom = false, near = false, far = false;
	};

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
	Settings settings;

	glm::mat4 projection;

	FrustumDataPerpective frustumDataPerspective;
	FrustumDataOrtho frustumDataOrtho;

	Framebuffer framebuffer;

	std::string shadername;
	IMaterial* renderMaterial;
public:
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
		//Create Framebuffer for this camera

	}

	void PostAttachment() override {
		if(renderMaterial != nullptr)
			this->attachment->addComponent(this->renderMaterial);
		GenerateFrameBuffer();
		UpdateData();
	}



	~Camera() {
		glDeleteTextures(1, &this->framebuffer.tex_color);
		glDeleteRenderbuffers(1, &this->framebuffer.renderbuffer);
		glDeleteFramebuffers(1, &this->framebuffer.framebuffer);
	}

	void GenerateFrameBuffer() {
		//create the framebuffer
		glGenFramebuffers(1, &this->framebuffer.framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer.framebuffer);

		// create a color attachment texture
		glGenTextures(1, &this->framebuffer.tex_color);
		glBindTexture(GL_TEXTURE_2D, this->framebuffer.tex_color);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, global.screen_width, global.screen_height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->framebuffer.tex_color, 0);

		//create the renderbuffer
		glGenRenderbuffers(1, &this->framebuffer.renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, this->framebuffer.renderbuffer);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, global.screen_width, global.screen_height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->framebuffer.renderbuffer);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	glm::mat4 GetProjection() {
		return this->projection;
	}

	glm::mat4 GetView() {
		Transformation* transform = this->attachment->GetTransform();
		if (transform != nullptr) {
			transform->Update();
			return glm::lookAt(transform->getPosition(), transform->getPosition() + transform->getFrontVector(), transform->getUpVector());
		}
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
			this->projection = glm::perspectiveLH(glm::radians(this->settings.fov), this->settings.aspect, this->settings.near, this->settings.far);
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
			frustumDataPerspective = FrustumDataPerpective( this->projection* this->GetView());
		}
	}


	bool IsInView(glm::vec3 point)
	{
		FrustumPlaneState fps = GetFrustumPlaneState(point);

		return fps.left && fps.right && fps.bottom && fps.top && fps.near && fps.far;
	}

	bool IsInView(glm::vec4 min, glm::vec4 max) {
		return IsInView(glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, max.y, max.z));
	}

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



	IMaterial* GetRenderMaterial() {
		return this->renderMaterial;
	}

	GLuint GetFrameBuffer() {
		return this->framebuffer.framebuffer;
	}

	GLuint GetColorTexture() {
		return this->framebuffer.tex_color;
	}

	glm::vec3 GetPosition(){
		return this->attachment->GetTransform()->getPosition();
	}

protected:
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