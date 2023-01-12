#ifndef __MODEL_GENERATOR_HPP__
#define __MODEL_GENERATOR_HPP__

#include <Engine/Component/Model.hpp>
#include <Engine/Component/ModelInstanced.hpp>
#include <Graphics/Material/IMaterial.hpp>
#include <Engine/Tools/Tools.hpp>
#include <ofbx.h>
#include <string>
#include <vector>

/// <summary>
/// Tools to generate Models.
/// </summary>
class ModelGenerator {
public:
	/// <summary>
	/// Load a model from a file, .FBX or .OBJ.
	/// </summary>
	/// <param name="filename">The filename.</param>
	/// <param name="material">The material of the model.</param>
	/// <param name="scale">the scale of the object.</param>
	/// <returns>The model loaded.</returns>
	static Model* LoadFromFile(std::string filename, IMaterial* material = new MaterialPBR(), double scale = 0.1)
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
			const std::vector < ofbx::Vec2 > uvs = g->getUVsVector(1);

			for (int i = 0, max = vert.size(); i < max; i++) {
				pts.push_back(glm::vec3(vert[i].x * scale, vert[i].y * scale, vert[i].z * scale));
				normals.push_back(glm::vec3(norm[i].x, norm[i].y, norm[i].z));
				uv.push_back(glm::vec2(uvs[i].x, 1.0 - uvs[i].y));
			}

			const std::vector<int> indices = g->getFaceIndicesVector();
			for (size_t i = 0, max = indices.size(); i < max; i += 3) {
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
			scene->destroy();

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
			for (int i = 0, max = facesIds.size(); i < max; i++) {

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

	/// <summary>
	/// Generate a triangle
	/// </summary>
	/// <param name="material">The Material of the model </param>
	/// <returns>The model generated</returns>
	static Model* Triangle(IMaterial* material = new MaterialPBR()) {
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

	/// <summary>
	/// Generate a quad for the screen.
	/// </summary>
	/// <param name="material">The Material of the model </param>
	/// <returns>The model generated</returns>
	static Model* QuadScreen(IMaterial* material = new MaterialPBR()) {
		std::vector<glm::vec3> pts;
		pts.push_back(glm::vec3(-1.0, -1.0, 0.0f));
		pts.push_back(glm::vec3(1.0, -1.0, 0.0f));
		pts.push_back(glm::vec3(1.0, 1.0, 0.0f));
		pts.push_back(glm::vec3(-1.0, 1.0, 0.0f));

		std::vector<glm::vec3> normals;

		std::vector<Model::Face> faces;
		faces.push_back(Model::Face(0, 1, 2, 3));

		std::vector<glm::vec2> uv;
		uv.push_back(glm::vec2(0, 0));
		uv.push_back(glm::vec2(1, 0));
		uv.push_back(glm::vec2(1, 1));
		uv.push_back(glm::vec2(0, 1));

		return new Model(pts, normals, faces, uv, material);
	}


	/// <summary>
	/// Generate a Quad
	/// </summary>
	/// <param name="material">The material.</param>
	/// <param name="resX">Resolution X of the quad</param>
	/// <param name="resZ">Resolution Z of the quad</param>
	/// <param name="sizeX">The Size on X axis of the quad</param>
	/// <param name="sizeZ">The Size on Z axis of the quad</param>
	/// <param name="center">is the origin is centered in the model ?</param>
	/// <returns>The generated model.</returns>
	static Model* Quad(IMaterial* material = new MaterialPBR(), int resX = 2, int resZ = 2, float sizeX = 1, float sizeZ = 1, bool center = true) {
		if (resX < 1 || resZ < 1) {
			return nullptr;
		}
		std::vector<glm::vec3> pts;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uv;
		std::vector<Model::Face> faces;

		double pasX = 1.0 / (double)resX;
		double pasY = 1.0 / (double)resZ;
		int resZp1 = resZ + 1;

		for (int x = 0; x <= resX; x++) {
			for (int z = 0; z <= resZ; z++) {
				pts.push_back(glm::vec3(x * pasX, 0, z * pasY));
				normals.push_back(glm::vec3(0, 1, 0));
				uv.push_back(glm::vec2(x * pasX, z * pasY));

				if (x < resX && z < resZ) {
					int a = (x * resZp1) + z + 1;
					int b = (x * resZp1) + z;
					int c = ((x + 1) * resZp1) + z;
					int d = ((x + 1) * resZp1) + z + 1;
					faces.push_back(Model::Face(a, b, c, d));
				}
			}
		}

		for (int i = 0, max = pts.size(); i < max; i++) {
			if (center) {
				pts[i] -= glm::vec3(0.5);
			}
			pts[i] *= glm::vec3(sizeX, 0, sizeZ);
		}

		return new Model(pts, normals, faces, uv, material);
	}

	/// <summary>
	/// Generate a Quad for instanciation
	/// </summary>
	/// <param name="material">The material.</param>
	/// <param name="resX">Resolution X of the quad</param>
	/// <param name="resZ">Resolution Z of the quad</param>
	/// <param name="sizeX">The Size on X axis of the quad</param>
	/// <param name="sizeZ">The Size on Z axis of the quad</param>
	/// <param name="center">is the origin is centered in the model ?</param>
	/// <returns>The generated model.</returns>
	static ModelInstanced* QuadInstanced(IMaterial* material = new MaterialPBR(), int resX = 2, int resZ = 2, float sizeX = 1, float sizeZ = 1, bool center = true) {
		if (resX < 1 || resZ < 1) {
			return nullptr;
		}
		std::vector<glm::vec3> pts;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uv;
		std::vector<Model::Face> faces;

		double pasX = 1.0 / (double)resX;
		double pasY = 1.0 / (double)resZ;
		int resZp1 = resZ + 1;

		for (int x = 0; x <= resX; x++) {
			for (int z = 0; z <= resZ; z++) {
				pts.push_back(glm::vec3(x * pasX, z * pasY, 0));
				normals.push_back(glm::vec3(0, 0, 1));
				uv.push_back(glm::vec2(x * pasX, z * pasY));

				if (x < resX && z < resZ) {
					int a = (x * resZp1) + z + 1;
					int b = (x * resZp1) + z;
					int c = ((x + 1) * resZp1) + z;
					int d = ((x + 1) * resZp1) + z + 1;
					faces.push_back(Model::Face(a, b, c, d));
				}
			}
		}

		for (int i = 0, max = pts.size(); i < max; i++) {
			if (center) {
				pts[i] -= glm::vec3(0.5);
			}
			pts[i] *= glm::vec3(sizeX, sizeZ, 0);
		}

		return new ModelInstanced(pts, normals, faces, uv, material);
	}


	/// <summary>
	/// Generate a Quad without faces.
	/// </summary>
	/// <param name="resX">Resolution X of the quad</param>
	/// <param name="resZ">Resolution Z of the quad</param>
	/// <param name="sizeX">The Size on X axis of the quad</param>
	/// <param name="sizeZ">The Size on Z axis of the quad</param>
	/// <param name="center">is the origin is centered in the model ?</param>
	/// <returns>The generated model.</returns>
	static std::vector<glm::vec3> QuadPoints(int resX = 2, int resZ = 2, float sizeX = 1, float sizeZ = 1, bool center = true) {
		std::vector<glm::vec3> pts;

		if (resX < 1 || resZ < 1) {
			return pts;
		}

		double pasX = 1.0 / (double)resX;
		double pasY = 1.0 / (double)resZ;
		int resZp1 = resZ + 1;

		for (int x = 0; x <= resX; x++) {
			for (int z = 0; z <= resZ; z++) {
				glm::vec3 pt = glm::vec3(x * pasX, 0, z * pasY);
				if (center) {
					pt -= glm::vec3(0.5);
				}
				pt *= glm::vec3(sizeX, 0, sizeZ);
				pts.push_back(pt);
			}
		}

		return pts;
	}

	/// <summary>
	/// Generate an UV Sphere
	/// </summary>
	/// <param name="material">The material</param>
	/// <param name="nbMeridien">Number of meridian of the Sphere</param>
	/// <param name="nbParalleles">Number of paralleles of the Sphere</param>
	/// <param name="scale">The scale of the sphere</param>
	/// <returns>The generated model.</returns>
	static Model* UVSphere(IMaterial* material = new MaterialPBR(), int nbMeridien = 16, int nbParalleles = 32, float scale = 1.0f) {
		if (nbMeridien < 3 || nbParalleles < 3) {
			return nullptr;
		}
		double pi = glm::pi<double>();
		double pi2 = pi * 2.0;

		double pasMeridien = 1.0 / (double)nbMeridien;
		double pasParalleles = 1.0 / (double)nbParalleles;

		double pasU = 1 / (double)nbMeridien;
		double pasV = 1 / (double)nbMeridien;

		std::vector<glm::vec3> pts;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uv;
		std::vector<Model::Face> faces;
		int nbMp1 = nbMeridien + 1;

		for (int j = 0; j < nbMeridien; j++) {
			double meridien = pi2 * j * pasMeridien;
			double x = glm::sin(0) * glm::cos(meridien);
			double y = glm::cos(0);
			double z = sin(0) * sin(meridien);

			pts.push_back(glm::vec3(x, y, z) * scale);
			normals.push_back(glm::normalize(glm::vec3(x, y, z)));
			uv.push_back(glm::vec2((j * pasMeridien) + (0.5 * pasMeridien), 0));

			int a = j;
			int b = (nbMeridien)+j;
			int c = (nbMeridien)+j + 1;
			faces.push_back(Model::Face(a, b, c));

		}

		for (int i = 1; i < nbParalleles; i++) {
			double parallele = pi * i * pasParalleles;

			for (int j = 0; j <= nbMeridien; j++) {
				double meridien = pi2 * j * pasMeridien;
				double x = glm::sin(parallele) * glm::cos(meridien);
				double y = glm::cos(parallele);
				double z = sin(parallele) * sin(meridien);
				pts.push_back(glm::vec3(x, y, z) * scale);
				normals.push_back(glm::normalize(glm::vec3(x, y, z)));
				uv.push_back(glm::vec2(j * pasMeridien, i * pasParalleles));

				if (j < nbMeridien) {
					int im1NB = ((i - 1) * nbMp1) + nbMeridien;
					int iNB = (i * nbMp1) + nbMeridien;

					if (i < nbParalleles - 1) {
						int a = im1NB + j + 1;
						int b = im1NB + j;
						int c = iNB + j;
						int d = iNB + j + 1;
						faces.push_back(Model::Face(a, b, c, d));
					}
					else {
						int a = im1NB + j + 1;
						int b = im1NB + j;
						int c = iNB + j;
						faces.push_back(Model::Face(a, b, c));
					}
				}
			}
		}

		for (int j = 0; j < nbMeridien; j++) {
			double meridien = pi2 * j * pasMeridien;
			double x = glm::sin(pi) * glm::cos(meridien);
			double y = glm::cos(pi);
			double z = sin(pi) * sin(meridien);

			pts.push_back(glm::vec3(x, y, z) * scale);
			normals.push_back(glm::normalize(glm::vec3(x, y, z)));
			uv.push_back(glm::vec2((j * pasMeridien) + (0.5 * pasMeridien), 1));
		}

		return new Model(pts, normals, faces, uv, material);
	}

	/// <summary>
	/// Generate a cube
	/// </summary>
	/// <param name="material">The material.</param>
	/// <param name="res">The resolution of the cube, for all axis</param>
	/// <param name="halfSize">The half size of each axis</param>
	/// <param name="center">is the origin centered ?</param>
	/// <returns>The generated mesh.</returns>
	static Model* Cube(IMaterial* material = new MaterialPBR(), size_t res = 2, glm::vec3 halfSize = glm::vec3(0.5), glm::vec3 center = glm::vec3(0)) {
		double pas = 1.0 / (double)res;
		std::vector<glm::vec3> pts;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uv;
		std::vector<Model::Face> faces;

		size_t rp1 = res + 1;
		size_t rp1_2 = rp1 * rp1;

		for (int f = 0; f < 6; f++) {
			size_t fres2 = f * rp1_2;
			glm::vec2 pos(0);
			for (size_t x = 0; x <= res; x++) {
				pos.x = x * pas;
				for (size_t y = 0; y <= res; y++) {
					bool inverted = false;
					pos.y = y * pas;
					if (f == 0) { // bottom
						pts.push_back(glm::vec3(pos.x, 0, pos.y));
						inverted = true;
					}
					else if (f == 1) { // top
						pts.push_back(glm::vec3(pos.x, 1, pos.y));
					}
					else if (f == 2) { //Front
						pts.push_back(glm::vec3(pos.x, pos.y, 0));
					}
					else if (f == 3) { // Back
						pts.push_back(glm::vec3(pos.x, pos.y, 1));
						inverted = true;
					}
					else if (f == 4) { //Left
						pts.push_back(glm::vec3(0, pos.x, pos.y));
					}
					else if (f == 5) { // Right
						pts.push_back(glm::vec3(1, pos.x, pos.y));
						inverted = true;
					}
					uv.push_back(pos);
					if (x < res && y < res) {

						if (inverted)
							faces.push_back(Model::Face(fres2 + (x * rp1) + y, fres2 + (x * rp1) + y + 1, fres2 + ((x + 1) * rp1) + y + 1, fres2 + ((x + 1) * rp1) + y));
						else
							faces.push_back(Model::Face(fres2 + (x * rp1) + y + 1, fres2 + (x * rp1) + y, fres2 + ((x + 1) * rp1) + y, fres2 + ((x + 1) * rp1) + y + 1));
					}
				}
			}
		}


		for (size_t i = 0, max = pts.size(); i < max; i++) {
			pts[i] = (pts[i] * (2.0f * halfSize)) - halfSize;
			normals.push_back(glm::normalize(pts[i]));
			pts[i] += center;
		}


		return new Model(pts, normals, faces, uv, material);
	}

	/// <summary>
	/// Generate a Cube for the water simulation.
	/// </summary>
	/// <param name="material">The material</param>
	/// <param name="resX">Resolution X of the top quad.</param>
	/// <param name="resZ">Resolution Z of the top quad.</param>
	/// <param name="size">The size of each axis.</param>
	/// <param name="center">Is the origin is centered ?</param>
	/// <returns>The generated model.</returns>
	static Model* CubeWater(IMaterial* material = new MaterialPBR(), int resX = 2, int resZ = 2, glm::vec3 size = glm::vec3(1.0f), bool center = true) {
		if (resX < 1 || resZ < 1) {
			return nullptr;
		}
		std::vector<glm::vec3> pts;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uv;
		std::vector<Model::Face> faces;

		double pasX = 1.0 / (double)resX;
		double pasZ = 1.0 / (double)resZ;
		int resZp1 = resZ + 1;

		//TOP
		for (int x = 0; x <= resX; x++) {
			for (int z = 0; z <= resZ; z++) {
				pts.push_back(glm::vec3(x * pasX, 0, z * pasZ));
				normals.push_back(glm::vec3(0, 1, 0));
				uv.push_back(glm::vec2(x * pasX, z * pasZ));

				if (x < resX && z < resZ) {
					int a = (x * resZp1) + z + 1;
					int b = (x * resZp1) + z;
					int c = ((x + 1) * resZp1) + z;
					int d = ((x + 1) * resZp1) + z + 1;
					faces.push_back(Model::Face(a, b, c, d));
				}
			}
		}

		int decalage = pts.size();
		//Front
		for (int x = 0; x <= resX; x++) {
			pts.push_back(glm::vec3(x * pasX, -1, 0));
			normals.push_back(glm::vec3(1, 0, 0));
			uv.push_back(glm::vec2(x * pasX, 1));

			if (x < resX) {
				int a = ((x + 1) * resZp1);
				int b = (x * resZp1);
				int c = decalage + x;
				int d = decalage + x + 1;

				faces.push_back(Model::Face(a, b, c, d));
			}
		}

		decalage = pts.size();
		//Back
		for (int x = 0; x <= resX; x++) {
			pts.push_back(glm::vec3(x * pasX, -1, 1));
			normals.push_back(glm::vec3(-1, 0, 0));
			uv.push_back(glm::vec2(x * pasX, 1));

			if (x < resX) {
				int a = (x * resZp1) + resZ;
				int b = ((x + 1) * resZp1) + resZ;
				int c = decalage + x + 1;
				int d = decalage + x;

				faces.push_back(Model::Face(a, b, c, d));
			}
		}


		decalage = pts.size();
		//Left
		for (int z = 0; z <= resZ; z++) {
			pts.push_back(glm::vec3(0, -1, z * pasZ));
			normals.push_back(glm::vec3(0, 0, 1));
			uv.push_back(glm::vec2(1, z * pasZ));

			if (z < resZ) {

				int a = z;
				int b = z + 1;
				int c = decalage + z + 1;
				int d = decalage + z;
				faces.push_back(Model::Face(a, b, c, d));
			}
		}

		decalage = pts.size();
		//Right
		for (int z = 0; z <= resZ; z++) {
			pts.push_back(glm::vec3(1, -1, z * pasZ));
			normals.push_back(glm::vec3(0, 0, -1));
			uv.push_back(glm::vec2(1, z * pasZ));

			if (z < resZ) {

				int a = (resX * resZp1) + z + 1;
				int b = (resX * resZp1) + z;
				int c = decalage + z;
				int d = decalage + z + 1;
				faces.push_back(Model::Face(a, b, c, d));
			}
		}

		for (int i = 0, max = pts.size(); i < max; i++) {
			if (center) {
				pts[i] -= glm::vec3(0.5);
			}
			pts[i] *= size;
		}

		return new Model(pts, normals, faces, uv, material);
	}


	/// <summary>
	/// Generate an multi layer Sphere with pointLink, without UV or Faces.
	/// </summary>
	/// <param name="out_linkage">List of linkage between points (output) ([n][m], n is the point, m is the point linked to the point n.)</param>
	/// <param name="numberOfLayer">The number of layers, final size = distBetweenPoints * numberOfLayer</param>
	/// <param name="distBetweenPoints">The distance between two point on the sphere</param>
	/// <param name="linkSensibility">The sensibility of the link, used to compute the sphere of detection (radius = linkSensibility * distBetweenPoints) (Inferior to 1 : no link) </param>
	/// <returns>The model of the Multi</returns>
	static std::vector<glm::vec3> MultiLayerSphere(std::vector<std::vector<unsigned int>> & out_linkage, int numberOfLayer = 4, float distBetweenPoints = 0.2f, float linkSensibility = 1.5f) {

		double pi = glm::pi<double>();
		double pi2 = pi * 2.0;

		std::vector<glm::vec3> pts;

		//Ajoute le point centrale
		pts.push_back(glm::vec3(0, 0, 0));
		out_linkage.push_back(std::vector<unsigned int>());

		for (int layer = 0; layer < numberOfLayer; layer++) {

			float scale = (layer + 1) * distBetweenPoints;
			float circon = 2 * pi * scale;
			int nbPointCircon = (int)(circon / distBetweenPoints);
			double pas = 1.0 / (double)nbPointCircon;
			int pasP1 = pas + 1;

			//Ajoute le 1er sommet
			pts.push_back(glm::vec3(1, 0, 0) * scale);
			out_linkage.push_back(std::vector<unsigned int>());

			//Ajoute les points centraux selon le pas.
			for (int i = 1; i < nbPointCircon; i++) {
				double parallele = pi * i * pas;
				for (int j = 0; j < nbPointCircon; j++) {
					double meridien = pi2 * j * pas;
					double x = glm::sin(parallele) * glm::cos(meridien);
					double y = glm::cos(parallele);
					double z = sin(parallele) * sin(meridien);
					pts.push_back(glm::vec3(x, y, z) * scale);

					out_linkage.push_back(std::vector<unsigned int>());

				}
			}

			//Ajoute le 2e sommet
			pts.push_back(glm::vec3(-1, 0, 0) * scale);
			out_linkage.push_back(std::vector<unsigned int>());

		}

		//Lie les points entre eux par rapport à une sphere de "linkSensibility" fois la distance entre les points.
		for (size_t i = 0, max = pts.size(); i < max; i++) {
			for (size_t j = i + 1; j < max; j++) {
				if (glm::distance(pts[i], pts[j]) < linkSensibility * distBetweenPoints) {
					out_linkage[i].push_back(j);
					out_linkage[j].push_back(i);
				}
			}

		}

		return pts;
	}
};

#endif