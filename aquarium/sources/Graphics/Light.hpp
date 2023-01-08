#ifndef __LIGHT_HPP__
#define __LIGHT_HPP__

#include <glm/glm.hpp>
#include <Engine/GameObject.hpp>
#include <Engine/Component/Component.hpp>
#include <Engine/Component/Transformation.hpp>
#include <Graphics/Camera.hpp>

/// <summary>
/// A light component
/// </summary>
class Light : public Component{
public :
	/// <summary>
	/// Type of light
	/// </summary>
	enum Type {
		POINT,
		DIRECTIONNAL,
	};
	/// <summary>
	/// Data of Light
	/// </summary>
	struct Data {
		glm::vec3 pos;
		glm::vec3 dir;
		glm::vec3 color;
		float power;
		float attenuation;
		Type type;
	};
	/// <summary>
	/// Uniform location for light
	/// </summary>
	struct LightUniform {
		GLuint pos;
		GLuint dir;
		GLuint color;
		GLuint power;
		GLuint directionnal;

		/// <summary>
		/// Generate a LightUniform data.
		/// </summary>
		/// <param name="program">The shader program.</param>
		/// <param name="pos">the light position shader location</param>
		/// <param name="dir">the light direction shader location</param>
		/// <param name="color">the light color shader location</param>
		/// <param name="power">the light power shader location</param>
		/// <param name="directionnal">the light directionnal shader location</param>
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
	/// <summary>
	/// Create a Light
	/// </summary>
	/// <param name="type">The type of light</param>
	/// <param name="color">The color of the light</param>
	/// <param name="power">The power of the light</param>
	/// <param name="attenuation">The attenuation of the light.</param>
	Light(Type type = Type::POINT, glm::vec3 color = glm::vec3(1.0f), float power = 1.0f, float attenuation = 1.0f) {

		this->data.type = type;
		this->data.color = color;
		this->data.power = power;
		this->data.attenuation = attenuation;
	}

	/// <summary>
	/// Update position and direction after post attachment.
	/// </summary>
	void PostAttachment() override{
		UpdatePosDir();
	}

	/// <summary>
	/// Update Pos and direction with the transform.
	/// </summary>
	void UpdatePosDir() {
		this->data.pos = this->attachment->GetTransform()->getPosition();
		glm::vec4 tmpDir = this->attachment->GetTransform()->getMatrix() * glm::vec4(0, -1, 0, 0);
		this->data.dir = glm::vec3(tmpDir.x, tmpDir.y, tmpDir.z);
	}
	
	/// <summary>
	/// return the light data.
	/// </summary>
	/// <returns>The light data</returns>
	Data GetData() {
		return this->data;
	}
};

#endif
