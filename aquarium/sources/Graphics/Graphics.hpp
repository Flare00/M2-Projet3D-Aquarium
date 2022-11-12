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


	Shader* standardShader;

public:
	Graphics() {
		this->standardShader = new Shader("std");
	}
	~Graphics() {
	}

	void Compute(std::vector<Camera*> cameras, std::vector<Displayable*> elements) {
		for (size_t indexCam = 0, nbCam = cameras.size(); indexCam < nbCam; indexCam++) {
			cameras[indexCam]->UpdateFrustum();
			for (size_t indexElem = 0, nbElem = elements.size(); indexElem < nbElem; indexElem++) {
				if (elements[indexElem]->IsAlwaysDraw()) {
					Draw(cameras[indexCam], elements[indexElem]);
				} else if (cameras[indexCam]->IsInView(elements[indexElem]->GetTransformation())) {
					printf("inView\n");
					Draw(cameras[indexCam], elements[indexElem]);
				}
			}
		}
	}

	void Draw(Camera * cam , Displayable* element) {
		if (element->GetTransformation() == nullptr) {
			return;
		}

		Model* model = element->GetGameObject()->getFirstComponentByType<Model>();

		if (model == nullptr) {
			return;
		}

		glUseProgram(standardShader->GetProgram());

		glUniformMatrix4fv(glGetUniformLocation(standardShader->GetProgram(), "u_model"), 1, GL_FALSE, &element->GetTransformation()->getMatrix()[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(standardShader->GetProgram(), "u_view"), 1, GL_FALSE, &cam->GetView()[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(standardShader->GetProgram(), "u_projection"), 1, GL_FALSE, &cam->GetProjection()[0][0]);

		if (global.wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else 
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		Model::Data mData = model->GetData();
		glBindVertexArray(mData.VAO);
		glDrawElements(GL_TRIANGLES, mData.sizeEBO, GL_UNSIGNED_INT, 0);
	}

	void ReloadShader() {
		this->standardShader->Reload();
	}

private:

	bool isInView(CameraFrustumData data, Transformation* transform) {


	}
};

#endif
