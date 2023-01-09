#ifndef __GRAPHICS_HPP__
#define __GRAPHICS_HPP__

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <Engine/GameObject.hpp>
#include <Graphics/Displayable.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Light.hpp>
#include <Engine/Shader.hpp>
#include <Engine/Component/Model.hpp>
#include <Engine/Tools/ModelGenerator.hpp>
#include <Engine/Global.hpp>
#include <Physics/GLPhysics/WaterPhysics.hpp>
#include <Physics/CollisionDetection.hpp>

/// <summary>
/// The graphics Engine
/// </summary>
class Graphics {
private:
	/// <summary>
	/// Graphics engine data (List of different camera by type)
	/// </summary>
	struct Data {
		std::vector<Camera*> renderCamera;
		std::vector<Camera*> depthCamera;
	};

	/// <summary>
	/// The screen model.
	/// </summary>
	Model* quad;

public:
	Graphics() {
	}
	~Graphics() {
	}

	/// <summary>
	/// Initialize the Graphics
	/// </summary>
	void Init() {
		this->quad = ModelGenerator::QuadScreen(new IMaterial("screen"));
		GLuint prog = quad->GetShader()->GetProgram();
		glUseProgram(prog);
		glUniform1i(glGetUniformLocation(prog, "renderTexture"), 0);
		glUniform1i(glGetUniformLocation(prog, "depthTexture"), 1);
		//glUniform1i(glGetUniformLocation(prog, "stencilTexture"), 2);
	}

	/// <summary>
	/// Compute the graphics using the root object of the scene, extracting all informations.
	/// </summary>
	/// <param name="root">The root Gameobject of the scene.</param>
	void Compute(GameObject* root) {
		Compute(root->getComponentsByTypeRecursive<Camera>(true), root->getComponentsByTypeRecursive<Light>(true), Displayable::SortByPriority(root->getComponentsByTypeRecursive<Displayable>(true)), root->getComponentsByTypeRecursive<WaterPhysics>());
	}
	/// <summary>
	/// Compute the graphics using a list of camera, lights, and Displayable elements.
	/// </summary>
	/// <param name="cameras">List of camera in the scene</param>
	/// <param name="lights">List of lights in the scene</param>
	/// <param name="elements">List of displayable element in the scene.</param>
	void Compute(std::vector<Camera*> cameras, std::vector<Light*> lights, std::vector<Displayable*> elements, std::vector<WaterPhysics*> waterPhysics) {

		Data data;
		glEnable(GL_DEPTH_TEST);

		GLuint program = this->quad->GetShader()->GetProgram();
		for (size_t indexCam = 0, nbCam = cameras.size(); indexCam < nbCam; indexCam++) {
			//Update the camera Frustum
			cameras[indexCam]->UpdateFrustum();
			//Bind the Framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, cameras[indexCam]->GetFrameBuffer());
			//Clear the Framebuffer

			bool isInWater = false;
			if (waterPhysics.size() > 0 && cameras[indexCam]->attachment->getFirstComponentByType<WaterAffected>() != nullptr) {
				for (size_t wIndex = 0, wMax = waterPhysics.size(); wIndex < wMax && !isInWater; wIndex++) {
					BoundingBoxCollider * bb = waterPhysics[wIndex]->attachment->getFirstComponentByType<BoundingBoxCollider>();
					isInWater = CollisionDetection::Point_AABB(cameras[indexCam]->GetPosition(), bb).collision;
				}
			}
			if (isInWater) {
				glClearColor(0.0f, 0.66f, 0.8f, 1.0f);
			}
			else {
				glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (size_t indexElem = 0, nbElem = elements.size(); indexElem < nbElem; indexElem++) {
				if (elements[indexElem]->IsAlwaysDraw()) {
					Draw(cameras[indexCam], elements[indexElem], lights, isInWater);
				}
				else
				{
					BoundingBoxCollider frustumCollider = elements[indexElem]->GetGameObject()->getFirstComponentByType<Model>()->GetFrustumCollider();
					if (cameras[indexCam]->IsInView(frustumCollider.GetMinOriented(), frustumCollider.GetMaxOriented())) {
						//printf("In View\n");
						Draw(cameras[indexCam], elements[indexElem], lights, isInWater);
					}
				}
			}

			glUseProgram(program);

			switch (cameras[indexCam]->GetType()) {
			case Camera::RENDER:
				//data.renderCamera.push_back(cameras[indexCam]);
				glActiveTexture(GL_TEXTURE0 + 0);
				glBindTexture(GL_TEXTURE_2D, cameras[indexCam]->GetColorTexture());
				glActiveTexture(GL_TEXTURE0 + 1);
				glBindTexture(GL_TEXTURE_2D, cameras[indexCam]->GetDepthTexture());
				//glActiveTexture(GL_TEXTURE0 + 2);
				//glBindTexture(GL_TEXTURE_2D, cameras[indexCam]->GetStencilTexture());


				break;
			case Camera::DEPTH_STENCIL:
				//data.depthCamera.push_back(cameras[indexCam]);
				//glActiveTexture(GL_TEXTURE0 + 1);
				//glBindTexture(GL_TEXTURE_2D, cameras[indexCam]->GetColorTexture());
				break;
			}


			glBindFramebuffer(GL_FRAMEBUFFER, 0);

		}

		DrawToScreen(data);
	}

	/// <summary>
	/// Draw an element to a Camera
	/// </summary>
	/// <param name="cam">The camera</param>
	/// <param name="element">The element to draw</param>
	/// <param name="lights">The list of lights in the scene.</param>
	void Draw(Camera* cam, Displayable* element, std::vector<Light*> lights,bool waterFog = false) {
		Model* model = element->GetGameObject()->getFirstComponentByType<Model>();
		Draw(cam, element, model, lights, waterFog);
	}

	/// <summary>
	/// Draw an element to the camera, with a specific model of him.
	/// </summary>
	/// <param name="cam">The camera</param>
	/// <param name="element">This displayable element to draw</param>
	/// <param name="model">The model to draw</param>
	/// <param name="lights">The list of lights in the scene</param>
	void Draw(Camera* cam, Displayable * element, Model* model, std::vector<Light*> lights, bool waterFog = false) {
		if(waterFog){
			glEnable(GL_FOG);
			GLfloat fogcolor[4] = {1.0, 0, 0, 1.0};
			glFogi(GL_FOG_MODE, GL_EXP);
			glFogfv(GL_FOG_COLOR, fogcolor);
			glFogf(GL_FOG_DENSITY, 0.33);
			glFogf(GL_FOG_START, 0.1);
			glFogf(GL_FOG_END, 1.0);
		} else {
			glDisable(GL_FOG);
		}

		GameObject* go = element->GetGameObject();
		if (go == nullptr) {
			return;
		}

		if (model == nullptr) {
			return;
		}
		bool updateLight = false;

		IMaterial* renderMaterial = cam->GetRenderMaterial();
		if (renderMaterial == nullptr) {
			renderMaterial = model->GetRenderMaterial();
		}
		
		/*if (renderMaterial->IsTransparent()) {
			glDisable(GL_DEPTH_TEST); // Do thing we don't want
		}
		else {

			glEnable(GL_DEPTH_TEST);
		}*/
		renderMaterial->SetDataGPU(go->GetMatrixRecursive(), cam->GetView(), cam->GetProjection(), cam->GetPosition(), waterFog);
		renderMaterial->SetLightGPU(lights);

		glUseProgram(renderMaterial->GetShader()->GetProgram());

		if (global.wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		Model::Data mData = model->GetData();
		glBindVertexArray(mData.VAO);
		glDrawElements(GL_TRIANGLES, mData.sizeEBO, GL_UNSIGNED_INT, 0);

		glFlush();
	}

	/// <summary>
	/// Raycast using the stencil, not implemented yet
	/// </summary>
	/// <param name="camera">The camera</param>
	/// <param name="elems">The elements</param>
	/// <returns>Return the hitted object.</returns>
	GameObject* RaycastStencil(Camera* camera, std::vector<GameObject*> elems) {
		// Use the Depth/Stencil (maybe faster) ? Or use collision ?
		for (size_t i = 0, max = elems.size(); i < max; i++) {

		}
		return nullptr;
	}

private:

	/// <summary>
	/// Draw the result of the cameras to screen.
	/// </summary>
	/// <param name="d">The data (aka list of cameras)</param>
	void DrawToScreen(Data d) {

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		GLuint program = this->quad->GetShader()->GetProgram();
		glUseProgram(program);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		Model::Data mData = this->quad->GetData();
		glBindVertexArray(mData.VAO);
		glDrawElements(GL_TRIANGLES, mData.sizeEBO, GL_UNSIGNED_INT, 0);
	}


};

#endif
