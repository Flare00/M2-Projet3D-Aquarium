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
#include <Engine/Global.hpp>

class Graphics {
private:
	struct CameraFrustumData {
		glm::vec4 cols[4];
		CameraFrustumData(glm::mat4 proj) {
			for (int i = 0; i < 4; i++) {
				glm::vec4 col;
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
		this->quad = Model::Quad(new Shader("screen"));
		GLuint prog = quad->GetShader()->GetProgram();
		glUseProgram(prog);
		glUniform1i(glGetUniformLocation(prog, "renderTexture"), 0);
		glUniform1i(glGetUniformLocation(prog, "depthTexture"), 1);
	}
	~Graphics() {
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
					Model* model = elements[indexElem]->GetGameObject()->getFirstComponentByType<Model>();
					Transformation* t = elements[indexElem]->GetTransformation();
					if (cameras[indexCam]->IsInView(t->getMatrix() * glm::vec4(model->GetMin(), 1), t->getMatrix() * glm::vec4(model->GetMax(), 1))) {
						printf("In View\n");
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
		Transformation* transformation = element->GetTransformation();
		if (transformation == nullptr) {
			return;
		}

		if (model == nullptr) {
			return;
		}
		bool updateLight = false;
		Shader* shader = cam->GetShader();
		if (shader == nullptr) {
			shader = model->GetShader();
			updateLight = shader->DefineOverride(Shader::DataOverride(Shader::FRAGMENT, "MAX_LIGHTS", std::to_string(lights.size() + 1)));
		}
		GLuint program = shader->GetProgram();

		glUseProgram(program);

		glUniformMatrix4fv(glGetUniformLocation(program, "u_model"), 1, GL_FALSE, &transformation->getMatrix()[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(program, "u_view"), 1, GL_FALSE, &cam->GetView()[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(program, "u_projection"), 1, GL_FALSE, &cam->GetProjection()[0][0]);

		if (updateLight) {
			for (size_t i = 0, max = lights.size(); i < max; i++) {
				Light::Data data = lights[i]->GetData();
				std::string prefix = "lights[" + std::to_string(i) + "]";
				glUniform3f(glGetUniformLocation(program, (prefix + ".pos").c_str()), data.pos.x, data.pos.y, data.pos.z);
				glUniform3f(glGetUniformLocation(program, (prefix + ".dir").c_str()), data.dir.x, data.dir.y, data.dir.z);
				glUniform3f(glGetUniformLocation(program, (prefix + ".color").c_str()), data.color.x, data.color.y, data.color.z);
				glUniform1f(glGetUniformLocation(program, (prefix + ".power").c_str()), data.power);
				glUniform1f(glGetUniformLocation(program, (prefix + ".attenuation").c_str()), data.attenuation);
				glUniform1i(glGetUniformLocation(program, (prefix + ".directional").c_str()), (lights[i]->POINT ? 0 : 1));
			}
		}

		Material::Data material = model->GetMaterial()->GetData();
		glUniform3f(glGetUniformLocation(program, ("material.color")), material.color.x, material.color.y, material.color.z);


		if (global.wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		Model::Data mData = model->GetData();
		glBindVertexArray(mData.VAO);
		glDrawElements(GL_TRIANGLES, mData.sizeEBO, GL_UNSIGNED_INT, 0);
	}

private:

	void DrawToScreen(Data d) {

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/*GLuint renderTex = -1;
		if (d.renderCamera.size() > 0) {
			//glBindFramebuffer(GL_FRAMEBUFFER, d.renderCamera[0]->GetFrameBuffer());
			renderTex = d.renderCamera[0]->GetColorTexture();
		}
		GLuint depthTex = -1;
		if (d.depthCamera.size() > 0) {
			//glBindFramebuffer(GL_FRAMEBUFFER, d.depthCamera[0]->GetFrameBuffer());
			depthTex = d.depthCamera[0]->GetColorTexture();
		}*/



		GLuint program = this->quad->GetShader()->GetProgram();
		glUseProgram(program);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		Model::Data mData = this->quad->GetData();
		glBindVertexArray(mData.VAO);
		// Set Graphicals Texture

		/*if (renderTex > -1) {
			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_2D, renderTex);
		}
		if (depthTex > -1) {
			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D, depthTex);
		}*/
		glDrawElements(GL_TRIANGLES, mData.sizeEBO, GL_UNSIGNED_INT, 0);
	}


};

#endif
