#ifndef __GRAPHICS_HPP__
#define __GRAPHICS_HPP__

#include <vector>
#include <glm/glm.hpp>
#include <Engine/GameObject.hpp>
#include <Graphics/Displayable.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Light.hpp>
#include <Engine/Shader.hpp>
#include <Engine/Component/Model.hpp>
#include <Engine/Tools/ModelGenerator.hpp>
#include <Engine/Global.hpp>

class Graphics {
private:
	struct CameraFrustumData {
		glm::vec4 cols[4];
		CameraFrustumData(glm::mat4 proj) {
			for (int i = 0; i < 4; i++) {
				glm::vec4 col(0,0,0,0);
				for (int j = 0; j < 4; j++) {
					col[j] = proj[j][i];
				}
				cols[i] = col;
			}
		}
	};


	struct Data {
		std::vector<Camera*> renderCamera;
		std::vector<Camera*> depthCamera;
	};


	Model* quad;

public:
	Graphics() {
	}
	~Graphics() {
	}

	void Init() {
		this->quad = ModelGenerator::QuadScreen(new IMaterial("screen"));
		GLuint prog = quad->GetShader()->GetProgram();
		glUseProgram(prog);
		glUniform1i(glGetUniformLocation(prog, "renderTexture"), 0);
		glUniform1i(glGetUniformLocation(prog, "depthTexture"), 1);
	}

	void Compute(GameObject* root) {
		Compute(root->getComponentsByTypeRecursive<Camera>(true), root->getComponentsByTypeRecursive<Light>(true), root->getComponentsByTypeRecursive<Displayable>(true));
	}

	void Compute(std::vector<Camera*> cameras, std::vector<Light*> lights, std::vector<Displayable*> elements) {

		Data data;
		glEnable(GL_DEPTH_TEST);

		GLuint program = this->quad->GetShader()->GetProgram();

		for (size_t indexCam = 0, nbCam = cameras.size(); indexCam < nbCam; indexCam++) {
			//Update the camera Frustum
			cameras[indexCam]->UpdateFrustum();
			//Bind the Framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, cameras[indexCam]->GetFrameBuffer());
			//Clear the Framebuffer
			glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (size_t indexElem = 0, nbElem = elements.size(); indexElem < nbElem; indexElem++) {
				if (elements[indexElem]->IsAlwaysDraw()) {
					Draw(cameras[indexCam], elements[indexElem], lights);
				}
				else
				{
					BoundingBoxCollider frustumCollider = elements[indexElem]->GetGameObject()->getFirstComponentByType<Model>()->GetFrustumCollider();
					if (cameras[indexCam]->IsInView(frustumCollider.GetMinOriented(), frustumCollider.GetMaxOriented())) {
						//printf("In View\n");
						Draw(cameras[indexCam], elements[indexElem], lights);
					}
				}
			}

			glUseProgram(program);

			switch (cameras[indexCam]->GetType()) {
			case Camera::RENDER:
				//data.renderCamera.push_back(cameras[indexCam]);
				glActiveTexture(GL_TEXTURE0 + 0);
				glBindTexture(GL_TEXTURE_2D, cameras[indexCam]->GetColorTexture());
				break;
			case Camera::DEPTH_STENCIL:
				//data.depthCamera.push_back(cameras[indexCam]);
				glActiveTexture(GL_TEXTURE0 + 1);
				glBindTexture(GL_TEXTURE_2D, cameras[indexCam]->GetColorTexture());
				break;
			}


			glBindFramebuffer(GL_FRAMEBUFFER, 0);

		}

		DrawToScreen(data);
	}

	void Draw(Camera* cam, Displayable* element, std::vector<Light*> lights) {
		Model* model = element->GetGameObject()->getFirstComponentByType<Model>();
		Draw(cam, element, model, lights);
	}

	void Draw(Camera* cam, Displayable * element, Model* model, std::vector<Light*> lights) {
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
		
		if (renderMaterial->IsTransparent()) {
			glDisable(GL_DEPTH_TEST);
		}
		else {
			glEnable(GL_DEPTH_TEST);
		}
		renderMaterial->SetDataGPU(go->GetMatrixRecursive(), cam->GetView(), cam->GetProjection(), cam->GetPosition());
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

	GameObject* RaycastStencil(Camera* camera, std::vector<GameObject*> elems) {
		// Use the Depth/Stencil (maybe faster) ? Or use collision ?
		for (size_t i = 0, max = elems.size(); i < max; i++) {

		}
		return nullptr;
	}

private:

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
