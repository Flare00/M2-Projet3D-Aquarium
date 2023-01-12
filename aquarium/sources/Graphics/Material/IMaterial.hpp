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

/// <summary>
/// Material Interface for drawing.
/// </summary>
class IMaterial : public Component{
protected:
	Shader* shader = nullptr;
	bool handleLights;
	bool transparent;

public:

	/// <summary>
	/// Create an IMaterial
	/// </summary>
	/// <param name="shadername">The name of the shader</param>
	/// <param name="handleLights">Is handling light ?</param>
	/// <param name="transparent">Is handling transparency ?</param>
	IMaterial(std::string shadername, bool handleLights = true, bool transparent = false) {
		this->handleLights = handleLights;
		if (shadername.size() > 0) {
			this->shader = settedStdShaders.AddShader(shadername);
		}
	}

	/// <summary>
	/// Return the shader
	/// </summary>
	/// <returns>The shader</returns>
	Shader* GetShader() {
		return this->shader;
	}

	/// <summary>
	/// Set Data to the GPU
	/// </summary>
	/// <param name="M">Model matrix</param>
	/// <param name="V">View Matrix</param>
	/// <param name="P">Projection Matrix</param>
	/// <param name="camPos">Camera position</param>
	/// <param name="inWater">Is in water ?</param>
	virtual void SetDataGPU(glm::mat4 M, glm::mat4 V, glm::mat4 P, glm::vec3 camPos, bool inWater, bool mainRender, bool causticAffected) {
		GLuint program = this->shader->GetProgram();
		glUseProgram(program);

		glUniformMatrix4fv(glGetUniformLocation(program, "u_model"), 1, GL_FALSE, &M[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(program, "u_view"), 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(program, "u_projection"), 1, GL_FALSE, &P[0][0]);
		glUniform3f(glGetUniformLocation(program, "u_cameraPos"), camPos.x, camPos.y, camPos.z);
		glUniform1i(glGetUniformLocation(program, ("u_in_water")), inWater ? 1 : 0);

	}

	/// <summary>
	/// Set lights information on GPU.
	/// </summary>
	/// <param name="lights">List of lights.</param>
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

	/// <summary>
	/// return if it's transparent.
	/// </summary>
	/// <returns>If it's transparent.</returns>
	bool IsTransparent() {
		return this->transparent;
	}

	/// <summary>
	/// Change the current shaderfiles of the material.
	/// </summary>
	/// <param name="shadername">The name of the shader</param>
	void SetShader(std::string shadername) {
		if (shadername.size() > 0) {
			this->shader = settedStdShaders.AddShader(shadername);
		}
	}

};
#endif // !__I_MATERIAL_HPP__
