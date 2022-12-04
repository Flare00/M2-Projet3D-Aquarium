#ifndef __TRANSFORMATION_HPP__
#define __TRANSFORMATION_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <Engine/Component/Component.hpp>

class Transformation : public Component
{
private:
	glm::vec3 position = glm::vec3(0);
	glm::vec3 rotation = glm::vec3(0);
	glm::vec3 scale = glm::vec3(1);
	glm::vec3 frontVector = glm::vec3(1, 0, 0);
	glm::vec3 upVector = glm::vec3(0,1,0);
	bool dirty = true;
	glm::mat4 matrix = glm::mat4(1.0);
	glm::mat4 rotationMatrix = glm::mat4(1);;

public:
	Transformation(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
		computeMatrix();
	}

	Transformation(glm::vec3 position) {
		this->position = position;
		this->rotation = glm::vec3(0.0);
		this->scale = glm::vec3(1, 1, 1);
		computeMatrix();
	}

	Transformation() {
		this->position = glm::vec3(0);
		this->rotation = glm::vec3(0);
		this->scale = glm::vec3(1, 1, 1);
		computeMatrix();
	}

	Transformation* SetPosition(glm::vec3 position) {
		this->dirty = true;
		this->position = position;
		return this;
	}

	Transformation* Translate(glm::vec3 translation) {
		this->dirty = true;
		glm::vec4 t = this->getMatrix() * glm::vec4(translation, 0);
		this->position += glm::vec3(t.x, t.y, t.z);
		return this;
	}

	Transformation* SetRotation(glm::vec3 rotation) {
		this->dirty = true;
		this->rotation = rotation;
		return this;
	}

	Transformation* Rotate(glm::vec3 rotation) {
		this->dirty = true;
		this->rotation += rotation;
		return this;
	}

	Transformation* SetScale(glm::vec3 scale) {
		this->dirty = true;
		this->scale = scale;
		return this;
	}

	Transformation* SetScale(double scale) {
		this->dirty = true;
		this->scale = glm::vec3(scale);
		return this;
	}


	Transformation* Scale(double scale) {
		this->dirty = true;
		this->scale += glm::vec3(scale);
		return this;
	}

	glm::vec3 getPosition() {
		return this->position;
	}
	glm::vec3 getRotation() {
		return this->rotation;
	}

	glm::mat4 getRotationMatrix() {
		return this->rotationMatrix;
	}
	glm::vec3 getScale() {
		return this->scale;
	}

	void computeMatrix() {
		//deplacer la matrice
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), this->position);

		//tourner la matrice selon les 3 axes
		this->rotationMatrix = glm::mat4(1);
		this->rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(this->rotation[1]), glm::vec3(0, 1, 0));
		this->rotationMatrix = glm::rotate(this->rotationMatrix, glm::radians(this->rotation[0]), glm::vec3(1, 0, 0));
		this->rotationMatrix = glm::rotate(this->rotationMatrix, glm::radians(this->rotation[2]), glm::vec3(0, 0, 1));

		//scale la matrice;
		this->matrix = translate * this->rotationMatrix * glm::scale(glm::mat4(1.0f), this->scale);

		this->dirty = false;

		glm::vec4 tmp = this->matrix * glm::vec4(0, 0, -1, 0);
		this->frontVector = glm::vec3(tmp.x, tmp.y, tmp.z);
		tmp = this->matrix * glm::vec4(0, 1, 0, 0);
		this->upVector = glm::vec3(tmp.x, tmp.y, tmp.z);
	}

	void Update() {
		if (dirty) {
			computeMatrix();
		}
	}

	glm::mat4 getMatrix() {
		if (dirty) {
			computeMatrix();
		}
		return this->matrix;
	}

	glm::vec3 getFrontVector() {
		return this->frontVector;
	}

	glm::vec3 getUpVector() {
		return this->upVector;
	}



};

#endif
