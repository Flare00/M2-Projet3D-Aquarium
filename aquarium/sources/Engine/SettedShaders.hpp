#ifndef __SETTED_SHADERS_HPP__
#define __SETTED_SHADERS_HPP__

#include <string>
#include <vector>
#include <Engine/Shader.hpp>

class SettedShaders
{
public:
	struct SettedShader
	{
		std::string shadername;
		Shader* shader;

		SettedShader(std::string shadername = "", Shader* shader = nullptr) {
			this->shadername = shadername;
			this->shader = shader;
		}
	};
protected:
	std::vector<SettedShader> settedShaders;
public:
	SettedShaders() {
	}

	Shader* FoundShader(std::string shadername) { // Check if the shader already exist, to save memory.
		Shader* res = nullptr;
		for (size_t i = 0, max = settedShaders.size(); i < max && res == nullptr; i++) {
			if (settedShaders[i].shadername.compare(shadername) == 0) {
				res = settedShaders[i].shader;
			}
		}
		return res;
	}

	//Add a shader by is name and value, if same name already exist, return it, else return the entry value.
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

	std::vector<SettedShader> GetSettedShaders() {
		return this->settedShaders;
	}
};

#endif // !__SETTED_SHADER_HPP__
