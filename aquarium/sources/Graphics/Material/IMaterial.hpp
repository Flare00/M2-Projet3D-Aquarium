#ifndef __I_MATERIAL_HPP__
#define __I_MATERIAL_HPP__

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <Engine/Shader.hpp>
#include <Engine/Component/Component.hpp>
#include <Graphics/Light.hpp>

struct SettedShader
{
	std::string shadername;
	Shader* shader;

	SettedShader(std::string shadername = "", Shader* shader = nullptr) {
		this->shadername = shadername;
		this->shader = shader;
	}
};

std::vector<SettedShader> settedShaders;

class IMaterial : public Component{
protected:
	struct MVPCUniform {
		GLuint M;
		GLuint V;
		GLuint P;
		GLuint C;

		MVPCUniform(GLuint program, std::string M, std::string V, std::string P, std::string C) {
			glUseProgram(program);
			this->M = glGetUniformLocation(program, M.c_str());
			this->V = glGetUniformLocation(program, V.c_str());
			this->P = glGetUniformLocation(program, P.c_str());
			this->C = glGetUniformLocation(program, C.c_str());
		}
	};

	MVPCUniform* mvpc;
	Shader* shader;
	bool handleLights;


public:


	IMaterial(std::string shadername, bool handleLights = true) {
		this->handleLights = handleLights;
		if (shadername.size() > 0) {
			size_t found = -1; // Check if the shader already exist, to save memory.
			for (size_t i = 0, max = settedShaders.size(); i < max && found < 0; i++) {
				if (settedShaders[i].shadername.compare(shadername) == 0) {
					found = i;
				}
			}
			if (found == -1) {
				this->shader = new Shader(shadername);
				settedShaders.push_back(SettedShader(shadername, this->shader));
			}
			else {
				this->shader = settedShaders[found].shader;
			}
			this->mvpc = new MVPCUniform(this->shader->GetProgram(), "u_model", "u_view", "u_projection", "u_cameraPos");

		}
	}

	Shader* GetShader() {
		return this->shader;
	}

	static std::vector<SettedShader> GetSettedShaders() {
		return settedShaders;
	}

	static void ClearSettedShaders() {
		settedShaders.clear();
	}

	virtual void SetDataGPU(glm::mat4 M, glm::mat4 V, glm::mat4 P, glm::vec3 camPos) {
		glUseProgram(this->shader->GetProgram());

		glUniformMatrix4fv(this->mvpc->M, 1, GL_FALSE, &M[0][0]);
		glUniformMatrix4fv(this->mvpc->V, 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(this->mvpc->P, 1, GL_FALSE, &P[0][0]);
		glUniform3f(this->mvpc->C, camPos.x, camPos.y, camPos.z);
	}

	void SetLightGPU(std::vector<Light*> lights) {
		if (handleLights) {
			this->shader->DefineOverride(Shader::DataOverride(Shader::FRAGMENT, "MAX_LIGHTS", std::to_string(lights.size() + 1)));
			for (int i = 0, max = lights.size(); i < max; i++) {
				std::string prefix = "lights[" + std::to_string(i) + "]";
				Light::LightUniform uniform(this->shader->GetProgram(), prefix + ".pos", prefix + ".dir", prefix + ".color", prefix + ".power", prefix + ".directional");

				Light::Data data = lights[i]->GetData();
				glUniform3f(uniform.pos, data.pos.x, data.pos.y, data.pos.z);
				glUniform3f(uniform.dir, data.dir.x, data.dir.y, data.dir.z);
				glUniform3f(uniform.color, data.color.x, data.color.y, data.color.z);
				glUniform1f(uniform.power, data.power);
				glUniform1i(uniform.directionnal, (lights[i]->POINT ? 0 : 1));
			}
		}
	}
};
#endif // !__I_MATERIAL_HPP__
