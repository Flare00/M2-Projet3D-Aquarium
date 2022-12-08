#ifndef __I_MATERIAL_HPP__
#define __I_MATERIAL_HPP__

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <Engine/Shader.hpp>
#include <Engine/Component/Component.hpp>
#include <Graphics/Light.hpp>
#include <Engine/SettedShaders.hpp>

SettedShaders settedStdShaders;

class IMaterial : public Component{
protected:
	Shader* shader;
	bool handleLights;
	bool transparent;

public:

	IMaterial(std::string shadername, bool handleLights = true, bool transparent = false) {
		this->handleLights = handleLights;
		if (shadername.size() > 0) {
			this->shader = settedStdShaders.AddShader(shadername);
		}
	}

	Shader* GetShader() {
		return this->shader;
	}

	virtual void SetDataGPU(glm::mat4 M, glm::mat4 V, glm::mat4 P, glm::vec3 camPos) {
		GLuint program = this->shader->GetProgram();
		glUseProgram(program);

		glUniformMatrix4fv(glGetUniformLocation(program, "u_model"), 1, GL_FALSE, &M[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(program, "u_view"), 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(program, "u_projection"), 1, GL_FALSE, &P[0][0]);
		glUniform3f(glGetUniformLocation(program, "u_cameraPos"), camPos.x, camPos.y, camPos.z);
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

	bool IsTransparent() {
		return this->transparent;
	}
};
#endif // !__I_MATERIAL_HPP__
