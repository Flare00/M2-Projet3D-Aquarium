#ifndef __MODEL_GENERATOR_HPP__
#define __MODEL_GENERATOR_HPP__

#include <Engine/Component/Model.hpp>
#include <Graphics/Material/IMaterial.hpp>
#include <Engine/Tools/Tools.hpp>
#include <ofbx.h>
#include <string>
#include <vector>
class ModelGenerator{
public:
    static Model* LoadFromFile(std::string filename, IMaterial* material = new MaterialPBR()) 
	{
		bool set = false;
		std::vector<glm::vec3> pts;
		std::vector<glm::vec3> normals;
		std::vector<Model::Face> faces;
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
				Model::Face f = Model::Face(p0, p2, p1);

				faces.push_back(f);
			}

			if (material == nullptr) {
				//Load Material ? 
				material = new MaterialPBR();
			}
		}
		else if (ext.compare("stl") == 0) {

		}
		else if (ext.compare("obj") == 0) {
			set = true;
			std::vector<std::vector<int>> facesIds;
			Tools::ObjLoader(modelFolder + filename, &pts, &normals, &uv, &facesIds);

			//printf("SIZES | PTS : %d | N : %d | UV : %d | FACE : %d\n", pts.size(), normals.size(), uv.size(), facesIds.size());
			/*for(int i = 0; i < pts.size();i++){
				printf("PTS[%d] : [%f, %f, %f]\n", i, pts[i].x, pts[i].y, pts[i].z);
			}*/
			for (int i = 0, max = facesIds.size(); i < max; i ++) {
				
  				if (facesIds[i].size() == 3) {
					faces.push_back(Model::Face(facesIds[i][0], facesIds[i][1], facesIds[i][2]));
				}
				else if (facesIds[i].size() >= 4) {
					faces.push_back(Model::Face(facesIds[i][0], facesIds[i][3], facesIds[i][2], facesIds[i][1]));
				}
			}
		}

		if (set) {
			return new Model(pts, normals, faces, uv, material);
		}
		else {
			return nullptr;
		}
	}

	static Model* Triangle(Shader* shader, IMaterial* material = new MaterialPBR()) {
		std::vector<glm::vec3> pts;
		pts.push_back(glm::vec3(-0.5, -0.5, 0.0f));
		pts.push_back(glm::vec3(0.5, -0.5, 0.0f));
		pts.push_back(glm::vec3(0.0, 0.5, 0.0f));

		std::vector<glm::vec3> normals;
		normals.push_back(glm::vec3(0, 0, 1));
		normals.push_back(glm::vec3(0, 0, 1));
		normals.push_back(glm::vec3(0, 0, 1));

		std::vector<Model::Face> faces;
		faces.push_back(Model::Face(0, 1, 2));

		std::vector<glm::vec2> uv;
		uv.push_back(glm::vec2(0, 0));
		uv.push_back(glm::vec2(1, 0));
		uv.push_back(glm::vec2(0.5, 1));
		return new Model(pts, normals, faces, uv, material);
	}

	static Model* Quad(IMaterial* material = new MaterialPBR()) {
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

		std::vector<Model::Face> faces;
		faces.push_back(Model::Face(0, 1, 2, 3));

		std::vector<glm::vec2> uv;
		uv.push_back(glm::vec2(0, 0));
		uv.push_back(glm::vec2(1, 0));
		uv.push_back(glm::vec2(1, 1));
		uv.push_back(glm::vec2(0, 1));

		return new Model(pts, normals, faces, uv, material);
	}

	static Model* DQuad(IMaterial* material = new MaterialPBR()) {
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


		std::vector<Model::Face> faces;
		faces.push_back(Model::Face(0, 1, 2, 3));
		faces.push_back(Model::Face(1, 0, 3, 2));

		std::vector<glm::vec2> uv;
		uv.push_back(glm::vec2(0, 0));
		uv.push_back(glm::vec2(1, 0));
		uv.push_back(glm::vec2(1, 1));
		uv.push_back(glm::vec2(0, 1));

		return new Model(pts, normals, faces, uv, material);
	}


	static Model* Cube(IMaterial* material = new MaterialPBR()) {
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

		std::vector<Model::Face> faces;
		//Front & Back
		faces.push_back(Model::Face(6, 7, 4, 5));
		faces.push_back(Model::Face(1, 0, 3, 2));
		// Bottom & Top
		faces.push_back(Model::Face(5, 4, 0, 1));
		faces.push_back(Model::Face(7, 6, 2, 3));
		// Left & Right
		faces.push_back(Model::Face(6, 5, 1, 2));
		faces.push_back(Model::Face(3, 0, 4, 7));

		std::vector<glm::vec2> uv;
		uv.push_back(glm::vec2(0, 0));
		uv.push_back(glm::vec2(1, 0));
		uv.push_back(glm::vec2(1, 1));
		uv.push_back(glm::vec2(0, 1));
		uv.push_back(glm::vec2(0, 0));
		uv.push_back(glm::vec2(1, 0));
		uv.push_back(glm::vec2(1, 1));
		uv.push_back(glm::vec2(0, 1));

		return new Model(pts, normals, faces, uv, material);
	} 
};

#endif