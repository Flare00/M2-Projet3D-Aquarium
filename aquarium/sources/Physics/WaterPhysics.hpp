#ifndef __WATER_PHYSICS_HPP__
#define __WATER_PHYSICS_HPP__

#include <glm/glm.hpp>
#include <string>
#include <GL/glew.h>
#include <vector>
#include <Physics/IPhysics.hpp>
#include <Graphics/Framebuffer.hpp>
#include <Engine/Shader.hpp>
#include <Engine/Tools/ModelGenerator.hpp>


class WaterPhysics : public IPhysics {
protected:
	struct Drop {
		glm::vec2 pos;
		float radius;
		float strength;
		Drop(glm::vec2 pos, float radius, float strength) {
			this->pos = pos;
			this->radius = radius;
			this->strength = strength;
		}
	};
	Shader* physicShader;
	Framebuffer framebuffer;
	std::vector<Drop> drops;
	int resolution;
	float invRes;

	int frameForCapture = 60;
	int frameForCaptureCurrent = 61;

	Model* quad;
public:
	WaterPhysics(int resolution = 1024) {
		this->physicShader = new Shader("Physics/water.vert", "Physics/water.frag");
		this->resolution = resolution;
		this->invRes = 1.0f / (float)resolution;
		this->quad = ModelGenerator::Quad(nullptr, resolution, resolution, 2, 2);


		float* data = new float[resolution * resolution * 4];

		for (int i = 0, max = this->resolution * this->resolution * 4; i < max; i++) {
			data[i] = (i%4 == 3 ? 1.0f : 0.0f);
		}
		glGenTextures(1, &this->texture);
		glBindTexture(GL_TEXTURE_2D, this->texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, resolution, resolution, 0, GL_RGBA, GL_FLOAT, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		delete data;
	}

	void PostAttachment() override {
		framebuffer.Generate(this->resolution, this->resolution, true);
	}

	void AddDrop(glm::vec2 pos, float radius, float strength) {
		this->drops.push_back(Drop(pos, radius, strength));
	}

	void Compute(double delta) override {
		if (glfwGetKey(global.global_window, GLFW_KEY_C) == GLFW_PRESS && frameForCaptureCurrent > frameForCapture) {
			frameForCaptureCurrent = frameForCapture;
		}
		//Bind framebuffer
		glViewport(0, 0, this->framebuffer.GetWidth(), this->framebuffer.GetHeight());
		glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer.GetFramebuffer());

		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0, 0, 1);

		glUseProgram(physicShader->GetProgram());
		//Set Data
		glUniform1f(glGetUniformLocation(physicShader->GetProgram(), "deltaTime"), delta);
		glUniform2f(glGetUniformLocation(physicShader->GetProgram(), "deltaMove"), invRes, invRes);
		glUniform1i(glGetUniformLocation(physicShader->GetProgram(), "tex"), 0);

		// Set Drop
		if (drops.size() > 0) {
			glUniform2f(glGetUniformLocation(physicShader->GetProgram(), "drop.center"), drops[0].pos.x, drops[0].pos.y);
			glUniform1f(glGetUniformLocation(physicShader->GetProgram(), "drop.radius"), drops[0].radius);
			glUniform1f(glGetUniformLocation(physicShader->GetProgram(), "drop.strength"), drops[0].strength);
			glUniform1i(glGetUniformLocation(physicShader->GetProgram(), "is_drop"), 1);
			drops.erase(drops.begin());
		}
		else {
			glUniform1i(glGetUniformLocation(physicShader->GetProgram(), "is_drop"), 0);
		}


		if (frameForCaptureCurrent <= frameForCapture && frameForCaptureCurrent > 0) {
			std::string name = "capture/" + std::to_string(frameForCapture - frameForCaptureCurrent) + ".0.png";
			this->framebuffer.WriteTextureToFile(name);
		}
		//Bind Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->texture);

		//Draw Mesh
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glVertexPointer(3, GL_FLOAT, 0, &quad[0]);
		//glDrawArrays(GL_TRIANGLES, 0, quad.size());

		Model::Data mData = this->quad->GetData();
		glBindVertexArray(mData.VAO);
		glDrawElements(GL_TRIANGLES, mData.sizeEBO, GL_UNSIGNED_INT, 0);

		if (frameForCaptureCurrent <= frameForCapture && frameForCaptureCurrent > 0) {
			std::string name = "capture/" + std::to_string(frameForCapture - frameForCaptureCurrent) + ".1.png";
			this->framebuffer.WriteTextureToFile(name);
		}


		glFlush();

		glBindTexture(GL_TEXTURE_2D, this->texture);
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, this->resolution, this->resolution);

		if (frameForCaptureCurrent <= frameForCapture && frameForCaptureCurrent > 0) {
			std::string name2 = "capture/" + std::to_string(frameForCapture - frameForCaptureCurrent) + ".2.png";

			glBindTexture(GL_TEXTURE_2D, this->texture);
			GLsizei stride = 4 * this->resolution;
			std::vector<GLfloat> datas(stride * this->resolution);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, datas.data());
			std::vector <GLchar> datasChar;
			for (int i = 0, max = datas.size(); i < max; i++) {
				datasChar.push_back((char)(datas[i] * 255.0));
			}
			stbi_write_png(name2.c_str(), this->resolution, this->resolution, 4, datasChar.data(), stride);
		}

		if (frameForCaptureCurrent <= frameForCapture) {
			frameForCaptureCurrent--;
		}

		if (frameForCaptureCurrent < 0) {
			frameForCaptureCurrent = frameForCapture + 1;
		}



		//Release
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, global.screen_width, global.screen_height);



	}

	GLuint GetHeightmap() {
		return this->framebuffer.GetTexColor();
	}
};

#endif
