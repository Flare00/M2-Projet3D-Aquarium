#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <string>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <Engine/Tools.hpp>
#include <Engine/Global.hpp>
class Shader
{
protected:
	std::string shaderFolder = "assets/Shaders/";
	std::string vertexFilename, fragmentFilename, tesselationControlFilename, tesselationEvalFilename, geometryFilename; // Name of all shader files, even if it does not exist, to permit the reload in case of change / add / remove
	std::string vertexData, fragmentData, tesselationControlData, tesselationEvalData, geometryData; // data of each shaders

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
		LoadFiles();
		Compile();
	}

	GLuint GetProgram() {
		return program;
	}
protected:
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
			if (CheckIfCompiled(this->vertexShader, "vertex")) {
				glAttachShader(this->program, this->vertexShader);
			}
		}

		//fragment
		if (this->fragmentData.size() > 0) {
			this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			const char* fData = this->fragmentData.c_str();
			glShaderSource(this->fragmentShader, 1, &fData, NULL);
			glCompileShader(this->fragmentShader);
			if (CheckIfCompiled(this->fragmentShader, "fragment")) {
				glAttachShader(this->program, this->fragmentShader);
			}
		}
		//tesselation control
		if (this->tesselationControlData.size() > 0) {
			this->tesselationControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
			const char* tcData = this->tesselationControlData.c_str();
			glShaderSource(this->tesselationControlShader, 1, &tcData, NULL);
			glCompileShader(this->tesselationControlShader);
			if (CheckIfCompiled(this->tesselationControlShader, "tess control")) {
				glAttachShader(this->program, this->tesselationControlShader);
			}
		}

		//tesselation evaluation
		if (this->tesselationEvalData.size() > 0) {
			this->tesselationEvalShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
			const char* teData = this->tesselationEvalData.c_str();
			glShaderSource(this->tesselationEvalShader, 1, &teData, NULL);
			glCompileShader(this->tesselationEvalShader);

			if (CheckIfCompiled(this->tesselationEvalShader, "tess eval")) {
				glAttachShader(this->program, this->tesselationEvalShader);
			}
		}
		//geometry
		if (this->geometryData.size() > 0) {
			this->geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
			const char* gData = this->geometryData.c_str();
			glShaderSource(this->geometryShader, 1, &gData, NULL);
			glCompileShader(this->geometryShader);

			if (CheckIfCompiled(this->geometryShader, "geometry")) {
				glAttachShader(this->program, this->geometryShader);
			}
		}

		glLinkProgram(this->program);

		if (CheckIfLinked()) {
			//CleanAfterLink();
		}
	}

	bool CheckIfCompiled(GLuint shader, std::string name)
	{
		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			printf("%s failed.\n", name.c_str());
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

			// Provide the infolog in whatever manor you deem best.
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
		GLuint * shaders = new GLuint[maxCount];

		glGetAttachedShaders(this->program, maxCount, &count,shaders);

		for (int i = 0; i < count; i++) {
			glDetachShader(this->program, shaders[i]);
		}
	}
};
#endif