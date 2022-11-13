#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <GLFW/glfw3.h>
#include <Engine/Shader.hpp>
#include <Engine/Component/Component.hpp>

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

	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<Face> faces;
	std::vector<glm::vec2> uv;

	Shader* shader;

	bool generated = false;

public:
	Model(Shader* shader, std::string filename) {
		this->shader = shader;

	}

	Model(Shader* shader, std::vector<glm::vec3> pts, std::vector<glm::vec3> normals = std::vector<glm::vec3>(), std::vector<Face> faces = std::vector<Face>(), std::vector<glm::vec2> uv = std::vector<glm::vec2>()) {
		this->points = pts;
		this->normals = normals;
		this->faces = faces;
		this->uv = uv;
		this->shader = shader;
		GenerateBuffer();
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
		return this->shader;
	}

	static Model* Triangle(Shader* shader) {
		std::vector<glm::vec3> pts;
		pts.push_back(glm::vec3(-0.5, -0.5, 0.0f));
		pts.push_back(glm::vec3(0.5, -0.5, 0.0f));
		pts.push_back(glm::vec3(0.0, 0.5, 0.0f));

		std::vector<glm::vec3> normals;
		normals.push_back(glm::vec3(0, 0, 1));
		normals.push_back(glm::vec3(0, 0, 1));
		normals.push_back(glm::vec3(0, 0, 1));

		std::vector<Face> faces;
		faces.push_back(Face(0, 1, 2));

		std::vector<glm::vec2> uv;
		uv.push_back(glm::vec2(0, 0));
		uv.push_back(glm::vec2(1, 0));
		uv.push_back(glm::vec2(0.5, 1));
		return new Model(shader, pts, normals, faces, uv);
	}

	static Model* Quad(Shader* shader) {
		std::vector<glm::vec3> pts;
		pts.push_back(glm::vec3(-1.0, -1.0, 0.0f));
		pts.push_back(glm::vec3(1.0, -1.0, 0.0f));
		pts.push_back(glm::vec3(1.0, 1.0, 0.0f));
		pts.push_back(glm::vec3(-1.0, 1.0, 0.0f));

		std::vector<glm::vec3> normals;
		normals.push_back(glm::vec3(0, 0, 1));
		normals.push_back(glm::vec3(0, 0, 1));
		normals.push_back(glm::vec3(0, 0, 1));
		normals.push_back(glm::vec3(0, 0, 1));

		std::vector<Face> faces;
		faces.push_back(Face(0, 1, 2, 3));

		std::vector<glm::vec2> uv;
		uv.push_back(glm::vec2(0, 0));
		uv.push_back(glm::vec2(1, 0));
		uv.push_back(glm::vec2(1, 1));
		uv.push_back(glm::vec2(0, 1));

		return new Model(shader, pts, normals, faces, uv);
	}

	static Model* Cube(Shader* shader) {
		std::vector<glm::vec3> pts;
		pts.push_back(glm::vec3(-0.5, -0.5, 0.0f));
		pts.push_back(glm::vec3(0.5, -0.5, 0.0f));
		pts.push_back(glm::vec3(0.5, 0.5, 0.0f));
		pts.push_back(glm::vec3(-0.5, 0.5, 0.0f));

		pts.push_back(glm::vec3(-0.5, -0.5, -1.0f));
		pts.push_back(glm::vec3(0.5, -0.5, -1.0f));
		pts.push_back(glm::vec3(0.5, 0.5, -1.0f));
		pts.push_back(glm::vec3(-0.5, 0.5, -1.0f));

		std::vector<glm::vec3> normals;
		normals.push_back(glm::vec3(0, 0, 1));
		normals.push_back(glm::vec3(0, 0, 1));
		normals.push_back(glm::vec3(0, 0, 1));
		normals.push_back(glm::vec3(0, 0, 1));
		normals.push_back(glm::vec3(0, 0, -1));
		normals.push_back(glm::vec3(0, 0, -1));
		normals.push_back(glm::vec3(0, 0, -1));
		normals.push_back(glm::vec3(0, 0, -1));

		std::vector<Face> faces;
		//Front & Back
		faces.push_back(Face(0, 1, 2, 3));
		faces.push_back(Face(7, 6, 5, 4));
		// Bottom & Top
		faces.push_back(Face(4, 5, 1, 0));
		faces.push_back(Face(6, 7, 3, 2));
		// Left & Right
		faces.push_back(Face(1, 2, 6, 5));
		faces.push_back(Face(0, 3, 7, 4));

		std::vector<glm::vec2> uv;
		uv.push_back(glm::vec2(0, 0));
		uv.push_back(glm::vec2(1, 0));
		uv.push_back(glm::vec2(1, 1));
		uv.push_back(glm::vec2(0, 1));
		uv.push_back(glm::vec2(0, 0));
		uv.push_back(glm::vec2(1, 0));
		uv.push_back(glm::vec2(1, 1));
		uv.push_back(glm::vec2(0, 1));

		return new Model(shader, pts, normals, faces, uv);
	}
};

#endif // !__MODEL_HPP__
