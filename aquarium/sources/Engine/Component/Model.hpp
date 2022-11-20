#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <vector>
#include <glm/glm.hpp>
#include <float.h>
#include <string>
#include <GLFW/glfw3.h>
#include <Engine/Shader.hpp>
#include <Engine/Component/Component.hpp>
#include <Graphics/Material.hpp>

#include <ofbx.h>

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

	Material* material;

	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<Face> faces;
	std::vector<glm::vec2> uv;

	glm::vec3 min, max;

	Shader* shader;

	bool generated = false;

public:

	Model(Shader* shader, std::vector<glm::vec3> pts, std::vector<glm::vec3> normals = std::vector<glm::vec3>(), std::vector<Face> faces = std::vector<Face>(), std::vector<glm::vec2> uv = std::vector<glm::vec2>(), Material* material = new Material()) {
		this->points = pts;
		this->normals = normals;
		this->faces = faces;
		this->uv = uv;
		this->shader = shader;
		this->material = material;
		ComputeMinMax();
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

	void ComputeMinMax() {
		this->min = glm::vec3(DBL_MAX);
		this->max = glm::vec3(-DBL_MAX);
		for (glm::vec3 p : this->points) {
			for (int i = 0; i < 3; i++) {
				if (p[i] < this->min[i]) {
					this->min[i] = p[i];
				}
				if (p[i] > this->max[i]) {
					this->max[i] = p[i];
				}
			}
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

	static Model* LoadFromFile(Shader* shader, std::string filename, Material* material = nullptr) 
	{
		bool set = false;
		std::vector<glm::vec3> pts;
		std::vector<glm::vec3> normals;
		std::vector<Face> faces;
		std::vector<glm::vec2> uv;

		//Load OBJ / STL / FBX
		std::string ext = Tools::GetExtensionLower(filename);
		if (ext.compare("fbx") == 0) {
			set = true;

			std::vector<unsigned char> data = Tools::GetFileContentByte(modelFolder + filename);
			ofbx::IScene* scene = ofbx::load(&data[0], data.size(), (ofbx::u64)ofbx::LoadFlags::TRIANGULATE);
			const ofbx::Geometry* g = scene->getGeometry(0);
			const std::vector<ofbx::Vec3> vert = g->getVerticesVector();
			const std::vector<ofbx::Vec3> norm = g->getNormalsVector();
			const std::vector<ofbx::Vec2> uvs = g->getUVsVector();
			for (int i = 0, max = vert.size(); i < max; i++) {
				pts.push_back(glm::vec3(vert[i].x * 0.01, vert[i].y * 0.01, vert[i].z * 0.01));
				normals.push_back(glm::vec3(norm[i].x, norm[i].y, norm[i].z));
				uv.push_back(glm::vec2(uvs[i].x, uvs[i].y));
			}

			const std::vector<int> indices = g->getFaceIndicesVector();
			for (int i = 0, max = indices.size(); i < max; i += 3) {
				int p0 = indices[i], p1 = indices[i + 1], p2 = indices[i + 2];
				p0 = p0 < 0 ? (-p0) - 1 : p0;
				p1 = p1 < 0 ? (-p1) - 1 : p1;
				p2 = p2 < 0 ? (-p2) - 1 : p2;
				Face f = Face(p0, p2, p1);

				faces.push_back(f);
			}

			if (material == nullptr) {
				//Load Material ? 
				material = new Material();
			}
		}
		else if (ext.compare("stl") == 0) {

		}
		else if (ext.compare("obj") == 0) {
			set = true;
			std::vector<std::vector<int>> facesIds;
			Tools::ObjLoader(modelFolder + filename, &pts, &normals, &uv, &facesIds);

			for (int i = 0, max = facesIds.size(); i < max; i ++) {
  				if (facesIds[i].size() == 3) {
					faces.push_back(Model::Face(facesIds[i][0], facesIds[i][2], facesIds[i][1]));
				}
				else if (facesIds[i].size() >= 4) {
					faces.push_back(Model::Face(facesIds[i][0], facesIds[i][3], facesIds[i][2], facesIds[i][1]));
				}
			}
		}

		if (set) {
			return new Model(shader, pts, normals, faces, uv, material);
		}
		else {
			return nullptr;
		}
	}

	static Model* Triangle(Shader* shader, Material* material = new Material()) {
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
		return new Model(shader, pts, normals, faces, uv, material);
	}

	static Model* Quad(Shader* shader, Material* material = new Material()) {
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

		return new Model(shader, pts, normals, faces, uv, material);
	}

	static Model* DQuad(Shader* shader, Material* material = new Material()) {
		std::vector<glm::vec3> pts;
		pts.push_back(glm::vec3(-1.0, -1.0, 0.0f));
		pts.push_back(glm::vec3(1.0, -1.0, 0.0f));
		pts.push_back(glm::vec3(1.0, 1.0, 0.0f));
		pts.push_back(glm::vec3(-1.0, 1.0, 0.0f));

		std::vector<glm::vec3> normals;
		normals.push_back(glm::vec3(0, 0, -1));
		normals.push_back(glm::vec3(0, 0, -1));
		normals.push_back(glm::vec3(0, 0, -1));
		normals.push_back(glm::vec3(0, 0, -1));


		std::vector<Face> faces;
		faces.push_back(Face(0, 1, 2, 3));
		faces.push_back(Face(1, 0, 3, 2));

		std::vector<glm::vec2> uv;
		uv.push_back(glm::vec2(0, 0));
		uv.push_back(glm::vec2(1, 0));
		uv.push_back(glm::vec2(1, 1));
		uv.push_back(glm::vec2(0, 1));

		return new Model(shader, pts, normals, faces, uv, material);
	}


	static Model* Cube(Shader* shader, Material* material = new Material()) {
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
		faces.push_back(Face(6, 7, 4, 5));
		faces.push_back(Face(1, 0, 3, 2));
		// Bottom & Top
		faces.push_back(Face(5, 4, 0, 1));
		faces.push_back(Face(7, 6, 2, 3));
		// Left & Right
		faces.push_back(Face(6, 5, 1, 2));
		faces.push_back(Face(3, 0, 4, 7));

		std::vector<glm::vec2> uv;
		uv.push_back(glm::vec2(0, 0));
		uv.push_back(glm::vec2(1, 0));
		uv.push_back(glm::vec2(1, 1));
		uv.push_back(glm::vec2(0, 1));
		uv.push_back(glm::vec2(0, 0));
		uv.push_back(glm::vec2(1, 0));
		uv.push_back(glm::vec2(1, 1));
		uv.push_back(glm::vec2(0, 1));

		return new Model(shader, pts, normals, faces, uv, material);
	}

	void SetMaterial(Material* material) {
		if (this->material != nullptr) {
			delete(this->material);
			this->material = nullptr;
		}
		this->material = material;
	}

	Material* GetMaterial() {
		return this->material;
	}

	glm::vec3 GetMin() {
		return this->min;
	}

	glm::vec3 GetMax() {
		return this->max;
	}
};

#endif // !__MODEL_HPP__
