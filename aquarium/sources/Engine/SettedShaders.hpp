#ifndef __SETTED_SHADERS_HPP__
#define __SETTED_SHADERS_HPP__

#include <string>
#include <vector>
#include <Engine/Shader.hpp>

/// <summary>
/// List all setted shader for a Scene.
/// </summary>
class SettedShaders
{
public:
	/// <summary>
	/// A setted Shader
	/// </summary>
	struct SettedShader
	{
		std::string shadername;
		Shader* shader;  

		/// <summary>
		/// Create a SettedShader
		/// </summary>
		/// <param name="shadername"> name of the shader.</param>
		/// <param name="shader">the shader.</param>
		SettedShader(std::string shadername = "", Shader* shader = nullptr) {
			this->shadername = shadername;
			this->shader = shader;
		}
	};
protected:
	//List of all setted Shader.
	std::vector<SettedShader> settedShaders;
public:
	SettedShaders() {
	}

	/// <summary>
	/// Return a setted shader
	/// </summary>
	/// <param name="shadername">The name of the shader.</param>
	/// <returns>The shader (if exist, else nullptr)</returns>
	Shader* FoundShader(std::string shadername) { // Check if the shader already exist, to save memory.
		Shader* res = nullptr;
		for (size_t i = 0, max = settedShaders.size(); i < max && res == nullptr; i++) {
			if (settedShaders[i].shadername.compare(shadername) == 0) {
				res = settedShaders[i].shader;
			}
		}
		return res;
	}

	/// <summary>
	/// Add a shader by is name and value, if same name already exist, return it, else return the entry value.
	/// </summary>
	/// <param name="shadername">The shadername</param>
	/// <param name="shader">The shader</param>
	/// <param name="force">Force the insertion</param>
	/// <returns>If does not already exist (or is forced) return generated SetterShader, else return the current existing shader.</returns>
	Shader* AddShader(std::string shadername, Shader* shader, bool force = false) {
		if (!force) {
			Shader* s = FoundShader(shadername);
			if (s == nullptr) {
				this->settedShaders.push_back(SettedShader(shadername, shader));
				return shader;
			}
			return s;
		}
		else {
			this->settedShaders.push_back(SettedShader(shadername, shader));
			return shader;
		}
	}

	/// <summary>
	/// Add a shader by is name only.
	/// </summary>
	/// <param name="shadername">The shadername</param>
	/// <param name="force">Force the insertion</param>
	/// <returns>If does not already exist (or is forced) return generated SetterShader, else return the current existing shader.</returns>
	Shader* AddShader(std::string shadername, bool force = false) {
		if (!force) {
			Shader* s = FoundShader(shadername);
			if (s == nullptr) {
				Shader* shader = new Shader(shadername);
				this->settedShaders.push_back(SettedShaders::SettedShader(shadername, shader));
				return shader;
			}
			return s;
		}
		else {
			Shader* shader = new Shader(shadername);
			this->settedShaders.push_back(SettedShaders::SettedShader(shadername, shader));
			return shader;
		}
	}

	/// <summary>
	/// Return the list of settedShader.
	/// </summary>
	/// <returns>The list of setted shaders.</returns>
	std::vector<SettedShader> GetSettedShaders() {
		return this->settedShaders;
	}
};

#endif // !__SETTED_SHADER_HPP__
