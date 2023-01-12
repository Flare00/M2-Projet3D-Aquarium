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
#include <IA/FishBank.hpp>
#include <Engine/RaycastObject.hpp>

/// <summary>
/// The graphics Engine
/// </summary>
class Graphics {
private:

public:
	Graphics() {
	}
	~Graphics() {
	}

	/// <summary>
	/// Initialize the Graphics
	/// </summary>
	void Init() {
	}

	/// <summary>
	/// Compute the graphics using render and raycast camera, and the root object of the scene, extracting informations.
	/// </summary>
	/// <param name="render_camera">The render camera</param>
	/// <param name="raycast_camera">The raycast camera</param>
	/// <param name="root">The root gameobject</param>
	void Compute(Camera* camera, GameObject* root, bool raycast) {
		glEnable(GL_DEPTH_TEST);
		std::vector<Light*> lights;
		std::vector<Displayable*> elements = Displayable::SortByPriority(root->getComponentsByTypeRecursive<Displayable>(true));
		std::vector<WaterPhysics*> waterPhysics = root->getComponentsByTypeRecursive<WaterPhysics>();
		if (raycast) {
			for (int i = elements.size() - 1; i >= 0; i--) {
				if (elements[i]->attachment->getFirstComponentByType<RaycastObject>() == nullptr)
				{
					elements.erase(elements.begin() + i);
				}
			}
		}
		else {
			lights = root->getComponentsByTypeRecursive<Light>(true);
		}

		Render(camera, lights, elements, waterPhysics, raycast);

		/*if (!raycast) {
			//DrawToScreen(camera);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, camera->GetFrameBuffer());
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glBlitFramebuffer(0, 0, global.screen_width, global.screen_height, 0, 0, camera->GetFramebufferObject()->GetWidth(), camera->GetFramebufferObject()->GetHeight(),
				GL_COLOR_BUFFER_BIT, GL_NEAREST);
		}*/
	}
	/// <summary>
	/// Compute the graphics using a list of camera, lights, and Displayable elements.
	/// </summary>
	/// <param name="cameras">List of camera in the scene</param>
	/// <param name="lights">List of lights in the scene</param>
	/// <param name="elements">List of displayable element in the scene.</param>

	/// <summary>
	/// Compute the graphics using a render and raycast camera, lights, and Displayable elements, and the water physics.
	/// </summary>
	/// <param name="camera">The camera to render</param>
	/// <param name="lights">List of lights in the scene</param>
	/// <param name="elements">List of displayable element in the scene.</param>
	/// <param name="waterPhysics">The water physics</param>
	void Render(Camera* camera, std::vector<Light*> lights, std::vector<Displayable*> elements, std::vector<WaterPhysics*> waterPhysics, bool raycast) {


		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//Update the camera Frustum
		camera->UpdateFrustum();
		//Bind the Framebuffer
		//Clear the Framebuffer

		bool isInWater = false;
		if (!raycast) {
			if (waterPhysics.size() > 0 && camera->attachment->getFirstComponentByType<WaterAffected>() != nullptr) {
				for (size_t wIndex = 0, wMax = waterPhysics.size(); wIndex < wMax && !isInWater; wIndex++) {
					BoundingBoxCollider* bb = waterPhysics[wIndex]->attachment->getFirstComponentByType<BoundingBoxCollider>();
					isInWater = CollisionDetection::Point_AABB(camera->GetPosition(), bb).collision;
				}
			}
			if (isInWater) {
				glClearColor(0.0f, 0.66f, 0.8f, 1.0f);
			}
			else {
				glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			}
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, camera->GetFrameBuffer());
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (size_t indexElem = 0, nbElem = elements.size(); indexElem < nbElem; indexElem++) {
			if (elements[indexElem]->IsAlwaysDraw()) {
				Draw(camera, elements[indexElem], lights, isInWater);
			}
			else
			{
				BoundingBoxCollider frustumCollider = elements[indexElem]->GetGameObject()->getFirstComponentByType<Model>()->GetFrustumCollider();
				if (camera->IsInView(frustumCollider.GetMinOriented(), frustumCollider.GetMaxOriented())) {
					//printf("In View\n");
					Draw(camera, elements[indexElem], lights, isInWater);
				}
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}






	/// <summary>
	/// Draw an element to a Camera
	/// </summary>
	/// <param name="cam">The camera</param>
	/// <param name="element">The element to draw</param>
	/// <param name="lights">The list of lights in the scene.</param>
	void Draw(Camera* cam, Displayable* element, std::vector<Light*> lights, bool waterFog = false) {
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
	void Draw(Camera* cam, Displayable* element, Model* model, std::vector<Light*> lights, bool waterFog = false) {

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

		renderMaterial->SetDataGPU(go->GetMatrixRecursive(), cam->GetView(), cam->GetProjection(), cam->GetPosition(), waterFog);
		renderMaterial->SetLightGPU(lights);

		glUseProgram(renderMaterial->GetShader()->GetProgram());

		if (global.wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}


		ModelInstanced* instanced = element->attachment->getFirstComponentByType<ModelInstanced>();
		if (instanced != nullptr) {
			ModelInstanced::DataInstanced mData = instanced->GetDataInstanced();

			glBindVertexArray(mData.VAO);
			glDrawElementsInstanced(GL_TRIANGLES, mData.sizeEBO, GL_UNSIGNED_INT, 0, instanced->GetPositions().size());
		}
		else {
			Model::Data mData = model->GetData();
			glBindVertexArray(mData.VAO);
			glDrawElements(GL_TRIANGLES, mData.sizeEBO, GL_UNSIGNED_INT, 0);
		}

		glFlush();
	}

private:

	/// <summary>
	/// Draw the result of the cameras to screen.
	/// </summary>
	/// <param name="d">The data (aka list of cameras)</param>
	/*void DrawToScreen(Camera* renderCamera) {

		//glBindFramebuffer(GL_FRAMEBUFFER, renderCamera->GetFrameBuffer());
		GLuint program = this->quad->GetShader()->GetProgram();

		glUseProgram(program);

		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, renderCamera->GetColorTexture());
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, renderCamera->GetDepthTexture());

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		Model::Data mData = this->quad->GetData();
		glBindVertexArray(mData.VAO);
		glDrawElements(GL_TRIANGLES, mData.sizeEBO, GL_UNSIGNED_INT, 0);
	}
	*/

};

#endif
