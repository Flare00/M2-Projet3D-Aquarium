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
protected : 
	GameObject* attachment;
	Transformation* transformation;
	Data data;

public :
	Light(GameObject* attachment, Type type = Type::POINT, glm::vec3 color = glm::vec3(1.0f), float power = 1.0f, float attenuation = 1.0f) {
		this->attachment = attachment;
		this->transformation = attachment->getFirstComponentByType<Transformation>();

		this->data.type = type;
		this->data.color = color;
		this->data.power = power;
		this->data.attenuation = attenuation;

		UpdatePosDir();
	}

	void UpdatePosDir() {
		this->data.pos = this->transformation->getPosition();
		glm::vec4 tmpDir = this->transformation->getMatrix() * glm::vec4(0, -1, 0, 0);
		this->data.dir = glm::vec3(tmpDir.x, tmpDir.y, tmpDir.z);
	}
	
	Data GetData() {
		return this->data;
	}
};

#endif
