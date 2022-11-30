#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <iostream>
#include <string>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <Engine/Tools/Tools.hpp>
#include <Engine/Global.hpp>
class Shader
{
public:
	enum Type {
		VERTEX,
		FRAGMENT,
		TESS_CONTROL,
		TESS_EVAL,
		GEOMETRY
	};

	struct DataOverride {
		Type type;
		std::string define;
		std::string value;
		DataOverride(Type type, std::string define, std::string value) {
			this->type = type;
			this->define = define;
			this->value = value;
		}
	};
protected:
	std::string shaderFolder = "assets/Shaders/";
	std::string vertexFilename, fragmentFilename, tesselationControlFilename, tesselationEvalFilename, geometryFilename; // Name of all shader files, even if it does not exist, to permit the reload in case of change / add / remove
	std::string vertexData, fragmentData, tesselationControlData, tesselationEvalData, geometryData; // data of each shaders

	std::vector<DataOverride> previousDataOverride;

	GLuint vertexShader, fragmentShader, tesselationControlShader, tesselationEvalShader, geometryShader; //Shaders
	GLuint program;
public:

	Shader(std::string name)
	{
		this->vertexFilename = name + ".vert";
		this->fragmentFilename = name + ".frag";
		this->tesselationControlFilename = name + ".tess.control";
		this->tesselationEvalFilename = name + ".tess.eval";
		this->geometryFilename = name + ".geom";
		LoadFiles();
		Compile();
	}

	Shader(std::string vertexFilename, std::string fragmentFilename, std::string tesselationControlFilename = "", std::string tesselationEvalFilename = "", std::string geometryFilename = "") {
		this->vertexFilename = vertexFilename;
		this->fragmentFilename = fragmentFilename;
		this->tesselationControlFilename = tesselationControlFilename;
		this->tesselationEvalFilename = tesselationEvalFilename;
		this->geometryFilename = geometryFilename;
		LoadFiles();
		Compile();
	}

	~Shader() {
		glDeleteProgram(program);
	}

	void SetData(std::string vertexData, std::string fragmentData, std::string tesselationControlData = "", std::string tesselationEvalData = "", std::string geometryData = "")
	{
		this->vertexData = vertexData;
		this->fragmentData = fragmentData;
		this->tesselationControlData = tesselationControlData;
		this->tesselationEvalData = tesselationEvalData;
		this->geometryData = geometryData;
		Compile();
	}

	void Reload()
	{
		glDeleteProgram(program);
		this->previousDataOverride.clear();
		LoadFiles();
		Compile();
	}

	bool DefineOverride(DataOverride data) {
		std::vector<DataOverride> datas;
		datas.push_back(data);
		return DefineOverride(datas) > 0;

	}

	int DefineOverride(std::vector<DataOverride> datas) {
		int changes = 0;
		for (DataOverride d : datas) {
			bool change = false;
			bool found = false;
			for (int i = 0, max = this->previousDataOverride.size(); i < max && !found; i++) {
				if (this->previousDataOverride[i].type == d.type) {
					if (this->previousDataOverride[i].define.compare(d.define) == 0) {
						found = true;
						if (this->previousDataOverride[i].value.compare(d.value) != 0) {
							change = true;
							previousDataOverride[i].value = d.value;
						} 
					}
				}
			}
			if (!found) {
				this->previousDataOverride.push_back(d);
			}
			if (change || !found) {
				changes++;
				switch (d.type) {
				case VERTEX:
					this->vertexData = ReplaceDefine(this->vertexData, d.define, d.value);
					break;
				case FRAGMENT:
					this->fragmentData = ReplaceDefine(this->fragmentData, d.define, d.value);
					break;
				case TESS_CONTROL:
					this->tesselationControlData = ReplaceDefine(this->tesselationControlData, d.define, d.value);
					break;
				case TESS_EVAL:
					this->tesselationEvalData = ReplaceDefine(this->tesselationEvalData, d.define, d.value);
					break;
				case GEOMETRY:
					this->geometryData = ReplaceDefine(this->geometryData, d.define, d.value);
					break;
				}
			}
		}
		if (changes > 0) {
			glDeleteProgram(program);
			Compile();
		}
		return changes;
	}

	GLuint GetProgram() {
		return program;
	}
protected:
	std::string ReplaceDefine(std::string data, std::string define, std::string value) {
		std::string search = "#define " + define;
		size_t found = data.find(search);
		if (found >= 0) {
			found += search.size();
			size_t cursorStart = found + 1;
			size_t cursorEnd = cursorStart;
			char c = data[cursorStart];
			bool end = false;
			while (c != '\n' && c != ' ' && !end) {
				cursorEnd++;
				c = data[cursorEnd];
			}
			data = data.replace(cursorStart, cursorEnd - cursorStart, value).c_str();
		}
		//printf("DATA : %s\n", data.c_str());
		return data;
	}
	void LoadFiles()
	{
		this->vertexData = Tools::GetFileContent(this->shaderFolder + this->vertexFilename);
		this->fragmentData = Tools::GetFileContent(this->shaderFolder + this->fragmentFilename);
		this->tesselationControlData = Tools::GetFileContent(this->shaderFolder + this->tesselationControlFilename);
		this->tesselationEvalData = Tools::GetFileContent(this->shaderFolder + this->tesselationEvalFilename);
		this->geometryData = Tools::GetFileContent(this->shaderFolder + this->geometryFilename);
		if (global.debug_shader) {
			printf("VertexData : %s\n", vertexData.c_str());
			printf("fragmentData : %s\n", fragmentData.c_str());
			printf("tesselationControlData : %s\n", tesselationControlData.c_str());
			printf("tesselationEvalData : %s\n", tesselationEvalData.c_str());
			printf("geometryData : %s\n", geometryData.c_str());
		}
	}
	void Compile()
	{
		//Create Shader Program
		this->program = glCreateProgram();

		//Creates, Compile and attach shader
		//vertex
		if (this->vertexData.size() > 0) {
			this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
			const char* vData = this->vertexData.c_str();
			glShaderSource(this->vertexShader, 1, &vData, NULL);
			glCompileShader(this->vertexShader);
			if (CheckIfCompiled(this->vertexShader, "vertex", this->vertexFilename)) {
				glAttachShader(this->program, this->vertexShader);
			}
		}

		//fragment
		if (this->fragmentData.size() > 0) {
			this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			const char* fData = this->fragmentData.c_str();
			glShaderSource(this->fragmentShader, 1, &fData, NULL);
			glCompileShader(this->fragmentShader);
			if (CheckIfCompiled(this->fragmentShader, "fragment", this->fragmentFilename)) {
				glAttachShader(this->program, this->fragmentShader);
			}
		}
		//tesselation control
		if (this->tesselationControlData.size() > 0) {
			this->tesselationControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
			const char* tcData = this->tesselationControlData.c_str();
			glShaderSource(this->tesselationControlShader, 1, &tcData, NULL);
			glCompileShader(this->tesselationControlShader);
			if (CheckIfCompiled(this->tesselationControlShader, "tess control", this->tesselationControlFilename)) {
				glAttachShader(this->program, this->tesselationControlShader);
			}
		}

		//tesselation evaluation
		if (this->tesselationEvalData.size() > 0) {
			this->tesselationEvalShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
			const char* teData = this->tesselationEvalData.c_str();
			glShaderSource(this->tesselationEvalShader, 1, &teData, NULL);
			glCompileShader(this->tesselationEvalShader);

			if (CheckIfCompiled(this->tesselationEvalShader, "tess eval", this->tesselationEvalFilename)) {
				glAttachShader(this->program, this->tesselationEvalShader);
			}
		}
		//geometry
		if (this->geometryData.size() > 0) {
			this->geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
			const char* gData = this->geometryData.c_str();
			glShaderSource(this->geometryShader, 1, &gData, NULL);
			glCompileShader(this->geometryShader);

			if (CheckIfCompiled(this->geometryShader, "geometry", this->geometryFilename)) {
				glAttachShader(this->program, this->geometryShader);
			}
		}

		glLinkProgram(this->program);

		if (CheckIfLinked()) {
			//CleanAfterLink();
		}
	}

	bool CheckIfCompiled(GLuint shader, std::string name, std::string filename = "")
	{
		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			if(filename.size() > 0) {
				printf("%s | %s failed.\n", filename.c_str(), name.c_str());
			} else {
				printf("%s failed.\n", name.c_str());
			}
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

			// Provide the infolog in whatever manor you deem best.
			for(int i = 0; i < maxLength; i++){
				printf("%c", errorLog[i]);
			}
			// Exit with failure.
			glDeleteShader(shader); // Don't leak the shader.
			return false;
		}
		return true;
	}

	bool CheckIfLinked()
	{
		GLint isLinked = 0;
		glGetProgramiv(this->program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			printf("Linking failed.\n");
			GLint maxLength = 0;
			glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(this->program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(this->program);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			glDeleteShader(tesselationControlShader);
			glDeleteShader(tesselationEvalShader);
			glDeleteShader(geometryShader);

			return false;
		}
		return true;
	}

	void CleanAfterLink() {
		GLsizei maxCount = 10;
		GLsizei count;
		GLuint* shaders = new GLuint[maxCount];

		glGetAttachedShaders(this->program, maxCount, &count, shaders);

		for (int i = 0; i < count; i++) {
			glDetachShader(this->program, shaders[i]);
		}
	}
};
#endif