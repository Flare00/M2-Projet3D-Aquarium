#ifndef __GRAPHICS_HPP__
#define __GRAPHICS_HPP__

#include <vector>
#include <glm/glm.hpp>
#include <Engine/GameObject.hpp>
#include <Graphics/Displayable.hpp>
#include <Graphics/Camera.hpp>
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
	Model * quad = Model::Quad(new Shader("screen"));

public:
	Graphics() {
	}
	~Graphics() {
	}

	void Compute(std::vector<Camera*> cameras, std::vector<Displayable*> elements) {
		std::vector<Camera*> renderCamera;
		glEnable(GL_DEPTH_TEST);


		for (size_t indexCam = 0, nbCam = cameras.size(); indexCam < nbCam; indexCam++) {
			cameras[indexCam]->UpdateFrustum();
			glBindFramebuffer(GL_FRAMEBUFFER, cameras[indexCam]->GetFrameBuffer());
			glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			for (size_t indexElem = 0, nbElem = elements.size(); indexElem < nbElem; indexElem++) {
				if (elements[indexElem]->IsAlwaysDraw()) {
					Draw(cameras[indexCam], elements[indexElem]);
				}
				else if (cameras[indexCam]->IsInView(elements[indexElem]->GetTransformation())) {
					Draw(cameras[indexCam], elements[indexElem]);
				}
			}

			if (cameras[indexCam]->GetType() == Camera::RENDER)
				renderCamera.push_back(cameras[indexCam]);

		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (renderCamera.size() > 0) {
			GLuint drawing = renderCamera[0]->GetColorTexture();

			glUseProgram(this->quad->GetShader()->GetProgram());
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			Model::Data mData = this->quad->GetData();
			glBindVertexArray(mData.VAO);
			glBindTexture(GL_TEXTURE_2D, drawing);
			glDrawElements(GL_TRIANGLES, mData.sizeEBO, GL_UNSIGNED_INT, 0);
			/*glBindFramebuffer(GL_READ_FRAMEBUFFER, renderCamera[0]->GetFrameBuffer());
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glBlitFramebuffer(0, 0, global.screen_width, global.screen_height, 0, 0, global.screen_width, global.screen_height,
				GL_COLOR_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);*/
		}
	}

	void Draw(Camera* cam, Displayable* element) {
		if (element->GetTransformation() == nullptr) {
			return;
		}

		Model* model = element->GetGameObject()->getFirstComponentByType<Model>();

		if (model == nullptr) {
			return;
		}
		Shader* shader = cam->GetShader();
		if (shader == nullptr) {
			shader = model->GetShader();
		}

		glUseProgram(shader->GetProgram());

		glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "u_model"), 1, GL_FALSE, &element->GetTransformation()->getMatrix()[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "u_view"), 1, GL_FALSE, &cam->GetView()[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "u_projection"), 1, GL_FALSE, &cam->GetProjection()[0][0]);

		if (global.wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		Model::Data mData = model->GetData();
		glBindVertexArray(mData.VAO);
		glDrawElements(GL_TRIANGLES, mData.sizeEBO, GL_UNSIGNED_INT, 0);
	}

private:

	bool isInView(CameraFrustumData data, Transformation* transform) {


	}
};

#endif
