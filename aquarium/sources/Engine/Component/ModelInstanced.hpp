#ifndef __MODEL_INSTANCED_HPP__
#define __MODEL_INSTANCED_HPP__

#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <GLFW/glfw3.h>
#include <Engine/Shader.hpp>
#include <Engine/Component/Component.hpp>
#include <Engine/Component/Model.hpp>
#include <Graphics/Material/MaterialPBR.hpp>
#include <Physics/Collider/BoundingBoxCollider.hpp>



/// <summary>
/// ModelInstanced component, for graphical.
/// </summary>
class ModelInstanced : public Model {
public:

	/// <summary>
	/// Graphics data of this object, (Vertex Array, Vertex Buffer, Element Buffer)
	/// </summary>
	struct DataInstanced
	{
		GLuint VAO, VBO[4], EBO;
		size_t sizeEBO = 0;
	};

protected:
	//The data of the model.
	DataInstanced dataInstanced;
	std::vector<glm::vec3> iPositions;
public:

	/// <summary>
	/// Constructor of the Model.
	/// </summary>
	/// <param name="pts">List of the points of the model.</param>
	/// <param name="normals">List of the normals of the model.</param>
	/// <param name="faces">List of the face of the model.</param>
	/// <param name="uv">List of the uvs of the model.</param>
	/// <param name="material">The material of the model.</param>
	ModelInstanced(std::vector<glm::vec3> pts, std::vector<glm::vec3> normals = std::vector<glm::vec3>(), std::vector<Face> faces = std::vector<Face>(), std::vector<glm::vec2> uv = std::vector<glm::vec2>(), IMaterial* material = new MaterialPBR())
		:Model(pts, normals, faces, uv, material, false)
	{
		GenerateBuffer();
	}

	/// <summary>
	/// Destructor that automaticalle free the arrays and buffers.
	/// </summary>
	~ModelInstanced()
	{
		glDeleteVertexArrays(1, &this->dataInstanced.VAO);
		glDeleteBuffers(4, this->dataInstanced.VBO);
		if (this->faces.size() > 0)
		{
			glDeleteBuffers(1, &this->dataInstanced.EBO);
		}
	}

	/// <summary>
	/// Generate the VAO, VBO and EBO buffers.
	/// </summary>
	void GenerateBuffer() override
	{
		glGenVertexArrays(1, &this->dataInstanced.VAO);
		glGenBuffers(4, this->dataInstanced.VBO);

		glBindVertexArray(this->dataInstanced.VAO);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, this->dataInstanced.VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, this->points.size() * sizeof(glm::vec3), &this->points[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		if (this->normals.size() > 0)
		{
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, this->dataInstanced.VBO[1]);
			glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(glm::vec3), &this->normals[0], GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}

		if (this->uv.size() > 0) {
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, this->dataInstanced.VBO[2]);
			glBufferData(GL_ARRAY_BUFFER, this->uv.size() * sizeof(glm::vec2), &this->uv[0], GL_STATIC_DRAW);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}

		if (this->iPositions.size() > 0) {
			glEnableVertexAttribArray(3);
			glBindBuffer(GL_ARRAY_BUFFER, this->dataInstanced.VBO[3]);
			glBufferData(GL_ARRAY_BUFFER, this->iPositions.size() * sizeof(glm::vec3), &this->iPositions[0], GL_DYNAMIC_DRAW);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glVertexAttribDivisor(3, 1);
		}

		if (this->faces.size() > 0)
		{
			std::vector<unsigned int> indices;
			for (Face f : this->faces) {
				std::vector<unsigned int> tmp = f.ToTriangleVector();
				indices.insert(indices.end(), tmp.begin(), tmp.end());
			}
			glGenBuffers(1, &this->dataInstanced.EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->dataInstanced.EBO);

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
			this->dataInstanced.sizeEBO = indices.size();
		}

		this->generated = true;
	}

	/// <summary>
	/// Return the graphicals data of the object.
	/// </summary>
	/// <returns>The graphicals data.</returns>
	DataInstanced GetDataInstanced() {
		return this->dataInstanced;
	}

	/// <summary>
	/// Change the positions list of the model. (for instancing)
	/// </summary>
	/// <param name="pts">The new position list</param>
	void SetPositions(std::vector<glm::vec3> pts) {

		if (iPositions.size() == 0) {
			this->iPositions.insert(this->iPositions.begin(), pts.begin(), pts.end());
			glEnableVertexAttribArray(3);
			glBindBuffer(GL_ARRAY_BUFFER, this->dataInstanced.VBO[3]);
			glBufferData(GL_ARRAY_BUFFER, this->iPositions.size() * sizeof(glm::vec3), &this->iPositions[0], GL_DYNAMIC_DRAW);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glVertexAttribDivisor(3, 1);
		}
		else {
			this->iPositions.clear();
			this->iPositions.insert(this->iPositions.begin(), pts.begin(), pts.end());

			this->frustumCollider.Update(pts);


			glBindBuffer(GL_ARRAY_BUFFER, this->dataInstanced.VBO[3]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, this->iPositions.size() * sizeof(glm::vec3), &this->iPositions[0]);
		}

		
	}

	/// <summary>
	/// Return the position list of the model. (for instancing)
	/// </summary>
	/// <returns>The position list</returns>
	std::vector<glm::vec3> GetPositions() {
		return this->iPositions;
	}


};

#endif // !__MODEL_HPP__
