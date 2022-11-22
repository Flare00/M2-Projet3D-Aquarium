#ifndef __LIGHT_HPP__
#define __LIGHT_HPP__

#include <glm/glm.hpp>
#include <Engine/GameObject.hpp>
#include <Engine/Component/Component.hpp>
#include <Engine/Component/Transformation.hpp>
#include <Graphics/Camera.hpp>

class Light : public Component{
public :
	enum Type {
		POINT,
		DIRECTIONNAL,
	};
	struct Data {
		glm::vec3 pos;
		glm::vec3 dir;
		glm::vec3 color;
		float power;
		float attenuation;
		Type type;
	};
	struct LightUniform {
		GLuint pos;
		GLuint dir;
		GLuint color;
		GLuint power;
		GLuint directionnal;

		LightUniform(GLuint program, std::string pos, std::string dir, std::string color, std::string power, std::string directionnal) {
			glUseProgram(program);
			this->pos = glGetUniformLocation(program, pos.c_str());
			this->dir = glGetUniformLocation(program, dir.c_str());
			this->color = glGetUniformLocation(program, color.c_str());
			this->power = glGetUniformLocation(program, power.c_str());
			this->directionnal = glGetUniformLocation(program, directionnal.c_str());
		}
	};
protected : 
	Data data;

public :
	Light(Type type = Type::POINT, glm::vec3 color = glm::vec3(1.0f), float power = 1.0f, float attenuation = 1.0f) {

		this->data.type = type;
		this->data.color = color;
		this->data.power = power;
		this->data.attenuation = attenuation;
	}

	void PostAttachment() override{
		UpdatePosDir();
	}

	void UpdatePosDir() {
		this->data.pos = this->attachment->GetTransform()->getPosition();
		glm::vec4 tmpDir = this->attachment->GetTransform()->getMatrix() * glm::vec4(0, -1, 0, 0);
		this->data.dir = glm::vec3(tmpDir.x, tmpDir.y, tmpDir.z);
	}
	
	Data GetData() {
		return this->data;
	}
};

#endif
