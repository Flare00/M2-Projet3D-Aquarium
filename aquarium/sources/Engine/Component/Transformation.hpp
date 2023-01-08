#ifndef __TRANSFORMATION_HPP__
#define __TRANSFORMATION_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <Engine/Component/Component.hpp>

/// <summary>
/// The transformation component to give Gameobject some transformation.
/// </summary>
class Transformation : public Component
{
private:
	// object position, rotation and scale.
	glm::vec3 position = glm::vec3(0);
	glm::vec3 rotation = glm::vec3(0);
	glm::vec3 scale = glm::vec3(1);

	//The transformation Matrix and the rotationMatrix (Quaternion will be better, but i have encountered some problem implementing this.)
	glm::mat4 matrix = glm::mat4(1.0);
	glm::mat4 rotationMatrix = glm::mat4(1);;

	//The front and up vector of the object.
	glm::vec3 frontVector = glm::vec3(1, 0, 0);
	glm::vec3 upVector = glm::vec3(0, 1, 0);

	bool dirty = true;

public:

	/// <summary>
	/// Generate a transformation object
	/// </summary>
	/// <param name="position">The position</param>
	/// <param name="rotation">The rotation</param>
	/// <param name="scale">The scale</param>
	Transformation(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
		computeMatrix();
	}

	/// <summary>
	/// Generate a transformation object with default rotation and scale.
	/// </summary>
	/// <param name="position">The position</param>
	Transformation(glm::vec3 position) {
		this->position = position;
		this->rotation = glm::vec3(0.0);
		this->scale = glm::vec3(1, 1, 1);

		computeMatrix();
	}

	/// <summary>
	/// Generate a transformation object with default position, rotation and scale.
	/// </summary>
	Transformation() {
		this->position = glm::vec3(0);
		this->rotation = glm::vec3(0);
		this->scale = glm::vec3(1, 1, 1);
		computeMatrix();
	}

	/// <summary>
	/// Change the position of the object.
	/// </summary>
	/// <param name="position">The new position</param>
	/// <returns>Self.</returns>
	Transformation* SetPosition(glm::vec3 position) {
		this->dirty = true;

		this->position = position;
		return this;
	}

	/// <summary>
	/// Translate the position of the object.
	/// </summary>
	/// <param name="translation">the amount to traslate.</param>
	/// <returns>Self.</returns>
	Transformation* Translate(glm::vec3 translation) {
		this->dirty = true;
		glm::vec4 t = this->getMatrix() * glm::vec4(translation, 0);
		this->position += glm::vec3(t.x, t.y, t.z);
		return this;
	}

	/// <summary>
	/// Set the rotation of the object.
	/// </summary>
	/// <param name="rotation">The new Rotation.</param>
	/// <returns>Self.</returns>
	Transformation* SetRotation(glm::vec3 rotation) {
		this->dirty = true;
		this->rotation = rotation;
		return this;
	}

	/// <summary>
	/// Rotate the object with euler angles.
	/// </summary>
	/// <param name="rotation">The euler values for the rotation.</param>
	/// <returns>Self.</returns>
	Transformation* Rotate(glm::vec3 rotation) {
		this->dirty = true;
		this->rotation += rotation;
		return this;
	}

	/// <summary>
	/// Set the object Scales.
	/// </summary>
	/// <param name="scale">The scales for each axis.</param>
	/// <returns>Self.</returns>
	Transformation* SetScale(glm::vec3 scale) {
		this->dirty = true;
		this->scale = scale;
		return this;
	}

	/// <summary>
	/// Set the object Scales
	/// </summary>
	/// <param name="scale">The value of the scale.</param>
	/// <returns>Self.</returns>
	Transformation* SetScale(double scale) {
		this->dirty = true;
		this->scale = glm::vec3(scale);
		return this;
	}


	/// <summary>
	/// Scale the object by a value.
	/// </summary>
	/// <param name="scale">The value of the scale.</param>
	/// <returns>Self.</returns>
	Transformation* Scale(double scale) {
		this->dirty = true;
		this->scale += glm::vec3(scale);
		return this;
	}

	/// <summary>
	/// Return the position.
	/// </summary>
	/// <returns>The position.</returns>
	glm::vec3 getPosition() {
		return this->position;
	}

	/// <summary>
	/// Return the rotation.
	/// </summary>
	/// <returns>The rotation.</returns>
	glm::vec3 getRotation() {
		return this->rotation;
	}


	/// <summary>
	/// Return the rotation matrix.
	/// </summary>
	/// <returns>The rotation matrix.</returns>
	glm::mat4 getRotationMatrix() {
		return this->rotationMatrix;
	}

	/// <summary>
	/// Return the scale.
	/// </summary>
	/// <returns>The scale.</returns>
	glm::vec3 getScale() {
		return this->scale;
	}

	/// <summary>
	/// Compute the transform matrix and rotation matrix.
	/// </summary>
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

	/// <summary>
	/// Update the Transformation, if dirty that compute the matrix.
	/// </summary>
	void Update() {
		if (dirty) {
			computeMatrix();
		}
	}

	/// <summary>
	/// Return the transformation matrix, auto update if dirty.
	/// </summary>
	/// <returns>the transformation matrix.</returns>
	glm::mat4 getMatrix() {
		if (dirty) {
			computeMatrix();
		}
		return this->matrix;
	}

	/// <summary>
	/// Return the front vector.
	/// </summary>
	/// <returns>The front vector</returns>
	glm::vec3 getFrontVector() {
		return this->frontVector;
	}

	/// <summary>
	/// Return the up vector.
	/// </summary>
	/// <returns>The up vector</returns>
	glm::vec3 getUpVector() {
		return this->upVector;
	}



};

#endif
