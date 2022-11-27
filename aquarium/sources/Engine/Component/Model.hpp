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
class Model : public Component {
public:
	struct Face {
		bool quad;
		unsigned int* linkedPoints;
		Face(unsigned int p1, unsigned int p2, unsigned int p3) {
			quad = false;
			linkedPoints = new unsigned int[3]();
			linkedPoints[0] = p1;
			linkedPoints[1] = p2;
			linkedPoints[2] = p3;
		}
		Face(unsigned int p1, unsigned int p2, unsigned int p3, unsigned int p4) {
			quad = true;
			linkedPoints = new unsigned int[4]();
			linkedPoints[0] = p1;
			linkedPoints[1] = p2;
			linkedPoints[2] = p3;
			linkedPoints[3] = p4;
		}

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

	struct Data
	{
		GLuint VAO, VBO[3], EBO;
		size_t sizeEBO = 0;
	};

protected:
	Data data;

	IMaterial* material;

	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<Face> faces;
	std::vector<glm::vec2> uv;

	BoundingBoxCollider frustumCollider;

	bool generated = false;

public:

	Model(std::vector<glm::vec3> pts, std::vector<glm::vec3> normals = std::vector<glm::vec3>(), std::vector<Face> faces = std::vector<Face>(), std::vector<glm::vec2> uv = std::vector<glm::vec2>(), IMaterial* material = new MaterialPBR()) {
		this->points = pts;
		this->normals = normals;
		this->faces = faces;
		this->uv = uv;
		this->material = material;
		ComputeFrustumCollider();
		GenerateBuffer();
	}

	void PostAttachment() override {
		this->attachment->addComponent(material);
	}

	~Model()
	{
		glDeleteVertexArrays(1, &this->data.VAO);
		glDeleteBuffers(3, this->data.VBO);
		if (this->faces.size() > 0)
		{
			glDeleteBuffers(1, &this->data.EBO);
		}
	}

	void ComputeFrustumCollider() {
		this->frustumCollider = BoundingBoxCollider(this->points);
	}

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

	Data GetData() {
		return this->data;
	}

	Shader* GetShader() {
		return this->material->GetShader();
	}

	

	void SetMaterial(MaterialPBR* material) {
		if (this->material != nullptr) {
			delete(this->material);
			this->material = nullptr;
		}
		this->material = material;
	}

	IMaterial* GetRenderMaterial() {
		return this->material;
	}

	BoundingBoxCollider GetFrustumCollider() {
		return this->frustumCollider;
	}
};

#endif // !__MODEL_HPP__
