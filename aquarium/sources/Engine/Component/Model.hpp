#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <GLFW/glfw3.h>
#include <Engine/Shader.hpp>
#include <Engine/Component/Component.hpp>
#include <Graphics/Material/MaterialPBR.hpp>
#include <Physics/Collider/BoundingBoxCollider.hpp>



std::string modelFolder = "assets/Model/";

/// <summary>
/// Model component, for graphical.
/// </summary>
class Model : public Component {
public:

	/// <summary>
	/// A Face data structure, Triangle or Quad.
	/// </summary>
	struct Face {

		bool quad;
		unsigned int linkedPoints[4];
		Face(unsigned int p1, unsigned int p2, unsigned int p3) {
			quad = false;
			linkedPoints[0] = p1;
			linkedPoints[1] = p2;
			linkedPoints[2] = p3;
		}
		Face(unsigned int p1, unsigned int p2, unsigned int p3, unsigned int p4) {
			quad = true;
			linkedPoints[0] = p1;
			linkedPoints[1] = p2;
			linkedPoints[2] = p3;
			linkedPoints[3] = p4;
		}

		/// <summary>
		/// Cast this Face to an int vector, corresponding to a list of triangle.
		/// </summary>
		/// <returns></returns>
		std::vector<unsigned int> ToTriangleVector() {
			std::vector<unsigned int> res;

			res.push_back(linkedPoints[0]);
			res.push_back(linkedPoints[1]);
			res.push_back(linkedPoints[2]);

			if (quad) {
				res.push_back(linkedPoints[0]);
				res.push_back(linkedPoints[2]);
				res.push_back(linkedPoints[3]);
			}

			return res;
		}

	};

	/// <summary>
	/// Graphics data of this object, (Vertex Array, Vertex Buffer, Element Buffer)
	/// </summary>
	struct Data
	{
		GLuint VAO, VBO[3], EBO;
		size_t sizeEBO = 0;
	};

protected:
	//The data of the model.
	Data data;

	// The material of the Model.
	IMaterial* material;

	//Points, normals, face and UVs array.
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<Face> faces;
	std::vector<glm::vec2> uv;

	//The bounding box for the frustum computation.
	BoundingBoxCollider frustumCollider;

	//Is the object is generated ?
	bool generated = false;

public:

	/// <summary>
	/// Constructor of the Model.
	/// </summary>
	/// <param name="pts">List of the points of the model.</param>
	/// <param name="normals">List of the normals of the model.</param>
	/// <param name="faces">List of the face of the model.</param>
	/// <param name="uv">List of the uvs of the model.</param>
	/// <param name="material">The material of the model.</param>
	Model(std::vector<glm::vec3> pts, std::vector<glm::vec3> normals = std::vector<glm::vec3>(), std::vector<Face> faces = std::vector<Face>(), std::vector<glm::vec2> uv = std::vector<glm::vec2>(), IMaterial* material = new MaterialPBR()) {
		this->points = pts;
		this->normals = normals;
		this->faces = faces;
		this->uv = uv;
		this->material = material;
		ComputeFrustumCollider();
		GenerateBuffer();
	}

	/// <summary>
	/// Function called after the Gameobject attachement was set, automatically add the linked component.
	/// </summary>
	void PostAttachment() override {
		this->attachment->addComponent(material);
		this->attachment->addComponent(&frustumCollider);
	}

	/// <summary>
	/// Destructor that automaticalle free the arrays and buffers.
	/// </summary>
	~Model()
	{
		glDeleteVertexArrays(1, &this->data.VAO);
		glDeleteBuffers(3, this->data.VBO);
		if (this->faces.size() > 0)
		{
			glDeleteBuffers(1, &this->data.EBO);
		}
	}

	/// <summary>
	/// Compute the frustum collider for frustum computing.
	/// </summary>
	void ComputeFrustumCollider() {
		this->frustumCollider = BoundingBoxCollider(this->points);
	}

	/// <summary>
	/// Generate the VAO, VBO and EBO buffers.
	/// </summary>
	void GenerateBuffer()
	{
		glGenVertexArrays(1, &this->data.VAO);
		glGenBuffers(3, this->data.VBO);

		glBindVertexArray(this->data.VAO);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, this->data.VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, this->points.size() * sizeof(glm::vec3), &this->points[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		if (this->normals.size() > 0)
		{
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, this->data.VBO[1]);
			glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(glm::vec3), &this->normals[0], GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}

		if (this->uv.size() > 0) {
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, this->data.VBO[2]);
			glBufferData(GL_ARRAY_BUFFER, this->uv.size() * sizeof(glm::vec2), &this->uv[0], GL_STATIC_DRAW);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}

		if (this->faces.size() > 0)
		{
			std::vector<unsigned int> indices;
			for (Face f : this->faces) {
				std::vector<unsigned int> tmp = f.ToTriangleVector();
				indices.insert(indices.end(), tmp.begin(), tmp.end());
			}
			glGenBuffers(1, &this->data.EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->data.EBO);

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
			this->data.sizeEBO = indices.size();
		}

		this->generated = true;
	}

	/// <summary>
	/// Return the graphicals data of the object.
	/// </summary>
	/// <returns>The graphicals data.</returns>
	Data GetData() {
		return this->data;
	}

	/// <summary>
	/// Return the shader of the material.
	/// </summary>
	/// <returns>The shader of the material.</returns>
	Shader* GetShader() {
		return this->material->GetShader();
	}

	/// <summary>
	/// Set the Material of the object.
	/// </summary>
	/// <param name="material">The new Material.</param>
	void SetMaterial(MaterialPBR* material) {
		if (this->material != nullptr) {
			delete(this->material);
			this->material = nullptr;
		}
		this->material = material;
	}

	/// <summary>
	/// Return the Material of the object.
	/// </summary>
	/// <returns>The Material</returns>
	IMaterial* GetRenderMaterial() {
		return this->material;
	}

	/// <summary>
	/// Return the Frustum Collider of the object.
	/// </summary>
	/// <returns>The Frustum Collider</returns>
	BoundingBoxCollider GetFrustumCollider() {
		return this->frustumCollider;
	}


	/// <summary>
	/// Return the point list of the model.
	/// </summary>
	/// <returns>The point list</returns>
	std::vector<glm::vec3> GetPoints() {
		return this->points;
	}

	/// <summary>
	/// Return the normal list of the model.
	/// </summary>
	/// <returns>The normal list</returns>
	std::vector<glm::vec3> GetNormales() {
		return this->normals;
	}

	/// <summary>
	/// Return the uv list of the model.
	/// </summary>
	/// <returns>The uv list</returns>
	std::vector<glm::vec2> GetUVs() {
		return this->uv;
	}

	/// <summary>
	/// Return the face list of the model.
	/// </summary>
	/// <returns>The face list</returns>
	std::vector<Face> GetFaces() {
		return this->faces;
	}
};

#endif // !__MODEL_HPP__
