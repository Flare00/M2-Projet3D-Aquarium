#ifndef __MATERIAL_PBR_HPP__
#define __MATERIAL_PBR_HPP__

#include <glm/glm.hpp>
#include <GL/gl.h>
#include <string>
#include <Graphics/Material/IMaterial.hpp>
#include <Graphics/Texture/Texture.hpp>
#include <Physics/GLPhysics/GLPhysic.hpp>

std::string textureFolder = "assets/Texture/";

/// <summary>
/// Material for PBR drawing. 
/// </summary>
class MaterialPBR : public IMaterial {
public:
	/// <summary>
	/// Data for the GPU.
	/// </summary>
	struct Data {
		Texture* albedoMap;
		Texture* normalMap;
		Texture* metallicMap;
		Texture* roughnessMap;
		Texture* aoMap;
		Texture* heightMap;

		glm::vec4 albedo;
		float metallic;
		float roughness;

		float ior;
		int transparent;

		/// <summary>
		/// Create data for the GPU
		/// </summary>
		/// <param name="albedo">default albedo value</param>
		/// <param name="metallic">default metallic value</param>
		/// <param name="roughness">default roughness value</param>
		/// <param name="ior">default ior value</param>
		/// <param name="transparent">default transparent value</param>
		/// <param name="albedoMap">The albedo map texture</param>
		/// <param name="normalMap">The normal map texture</param>
		/// <param name="metallicMap">The metallic map texture</param>
		/// <param name="roughnessMap">The roughness map texture</param>
		/// <param name="aoMap">The ao map texture</param>
		/// <param name="heightMap">The Heightmap texture</param>
		Data(glm::vec4 albedo, float metallic, float roughness, float ior, int transparent, Texture* albedoMap, Texture* normalMap, Texture* metallicMap, Texture* roughnessMap, Texture* aoMap, Texture* heightMap) {

			this->albedo = albedo;
			this->metallic = metallic;
			this->roughness = roughness;
			this->ior = ior;
			this->transparent = transparent;

			this->albedoMap = albedoMap;
			this->normalMap = normalMap;
			this->metallicMap = metallicMap;
			this->roughnessMap = roughnessMap;
			this->aoMap = aoMap;
			this->heightMap = heightMap;
		}

		~Data() {
			delete albedoMap;
			delete normalMap;
			delete metallicMap;
			delete roughnessMap;
			delete aoMap;
			delete heightMap;
		}
	};

protected:
	Data* data;

	std::string albedoMapFile;
	std::string normalMapFile;
	std::string metallicMapFile;
	std::string roughnessMapFile;
	std::string aoMapFile;
	std::string heightMapFile;

public:
	/// <summary>
	/// Create a PBR Material
	/// </summary>
	/// <param name="albedo">default albedo value</param>
	/// <param name="metallic">default metallic value</param>
	/// <param name="roughness">default roughness value</param>
	/// <param name="ior">default ior value</param>
	/// <param name="transparent">default transparent value</param>
	/// <param name="albedoMapFile">The AlbedoMap filename</param>
	/// <param name="normalMapFile">The normalMap filename</param>
	/// <param name="metallicMapFile">The metallicMap filename</param>
	/// <param name="roughnessMapFile">The roughnessMap filename</param>
	/// <param name="aoMapFile">The aoMap filename</param>
	/// <param name="heightMapFile">The heightMap filename</param>
	MaterialPBR(glm::vec4 albedo = glm::vec4(1.0), float metallic = 0.0f, float roughness = 0.0f, float ior = 1.0f, bool transparent = false,
		std::string albedoMapFile = "", std::string normalMapFile = "", std::string metallicMapFile = "", std::string roughnessMapFile = "", std::string aoMapFile = "", std::string heightMapFile = "")
		: IMaterial("pbr", true, transparent)
	{

		this->albedoMapFile = albedoMapFile;
		this->normalMapFile = normalMapFile;
		this->metallicMapFile = metallicMapFile;
		this->roughnessMapFile = roughnessMapFile;
		this->aoMapFile = aoMapFile;
		this->heightMapFile = heightMapFile;

		this->data = new Data(albedo, metallic, roughness, ior, transparent ? 1 : 0,
			new Texture(albedoMapFile, glm::vec4(1)),
			new Texture(normalMapFile, glm::vec4(0)),
			new Texture(metallicMapFile, glm::vec4(0)),
			new Texture(roughnessMapFile, glm::vec4(0)),
			new Texture(aoMapFile, glm::vec4(0)),
			new Texture(heightMapFile, glm::vec4(0))
		);
	}


	/// <summary>
	/// Destroy the PBR Material.
	/// </summary>
	~MaterialPBR() {
		delete data;
	}

	/// <summary>
	/// Set the albedo value.
	/// </summary>
	/// <param name="albedo">New Albedo Value.</param>
	/// <returns>Self.</returns>
	MaterialPBR* SetAlbedo(glm::vec4 albedo) {
		this->data->albedo = albedo;
		return this;
	}

	/// <summary>
/// Set the Metallic value.
/// </summary>
/// <param name="albedo">New Metallic Value.</param>
/// <returns>Self.</returns>
	MaterialPBR* SetMetallic(float metallic) {
		this->data->metallic = metallic;
		return this;
	}

	/// <summary>
/// Set the roughness value.
/// </summary>
/// <param name="albedo">New roughness Value.</param>
/// <returns>Self.</returns>
	MaterialPBR* SetRoughness(float roughness) {
		this->data->roughness = roughness;
		return this;
	}

	/// <summary>
	/// Set the albedo map file.
	/// </summary>
	/// <param name="albedo">New albedoMapFile Value.</param>
	/// <returns>Self.</returns>
	MaterialPBR* SetAlbedoMapFile(std::string albedoMapFile) {
		this->albedoMapFile = albedoMapFile;
		this->data->albedoMap->SetPath(this->albedoMapFile);
		return this;
	}

	/// <summary>
/// Set the normalMapFile.
/// </summary>
/// <param name="albedo">New normalMapFile Value.</param>
/// <returns>Self.</returns>
	MaterialPBR* SetNormalMapFile(std::string normalMapFile) {
		this->normalMapFile = normalMapFile;
		this->data->normalMap->SetPath(this->normalMapFile);
		return this;
	}

	/// <summary>
/// Set the metallicMapFile.
/// </summary>
/// <param name="albedo">New metallicMapFile Value.</param>
/// <returns>Self.</returns>
	MaterialPBR* SetMetallicMapFile(std::string metallicMapFile) {
		this->metallicMapFile = metallicMapFile;
		this->data->metallicMap->SetPath(this->metallicMapFile);
		return this;
	}

	/// <summary>
/// Set the roughnessMapFile.
/// </summary>
/// <param name="albedo">New roughnessMapFile Value.</param>
/// <returns>Self.</returns>
	MaterialPBR* SetRoughnessMapFile(std::string roughnessMapFile) {
		this->roughnessMapFile = roughnessMapFile;
		this->data->roughnessMap->SetPath(this->roughnessMapFile);
		return this;
	}

	/// <summary>
/// Set the aoMapFile.
/// </summary>
/// <param name="albedo">New aoMapFile Value.</param>
/// <returns>Self.</returns>
	MaterialPBR* SetAOMapFile(std::string aoMapFile) {
		this->aoMapFile = aoMapFile;
		this->data->aoMap->SetPath(this->aoMapFile);
		return this;
	}


	/// <summary>
/// Set the heightMapFile.
/// </summary>
/// <param name="albedo">New heightMapFile Value.</param>
/// <returns>Self.</returns>
	MaterialPBR* SetHeightMapFile(std::string heightMapFile) {
		this->heightMapFile = heightMapFile;
		this->data->heightMap->SetPath(this->heightMapFile);
		return this;
	}

	/// <summary>
	/// Set the Map files data with a forlder (folder/color.ext, folder/normal.ext, folder/metalness.ext,  folder/roughness.ext, folder/ao.ext, folder/heightmap.ext)
	/// </summary>
	/// <param name="folder">The folder containing the files.</param>
	/// <param name="ext">The extension of the files.</param>
	/// <returns>Self.</returns>
	MaterialPBR* SetFolderData(std::string folder, std::string ext) { //Load Material by a folder, with color.ext, heightmap.ext, metalness.ext, normal.ext, roughness.ext, ao.ext
		std::string folderPath = textureFolder + folder + "/";
		this->SetAlbedoMapFile(folderPath + "color." + ext);
		this->SetNormalMapFile(folderPath + "normal." + ext);
		this->SetMetallicMapFile(folderPath + "metalness." + ext);
		this->SetRoughnessMapFile(folderPath + "roughness." + ext);
		this->SetAOMapFile(folderPath + "ao." + ext);
		this->SetHeightMapFile(folderPath + "heightmap." + ext);
		return this;
	}

	/// <summary>
	/// Return the PBR Datas.
	/// </summary>
	/// <returns>The PBR datas.</returns>
	const Data* GetData() {
		return this->data;
	}


	/// <summary>
	/// Set the Material Data to the GPU
	/// </summary>
	/// <param name="M">Model matrix</param>
	/// <param name="V">View Matrix</param>
	/// <param name="P">Projection Matrix</param>
	/// <param name="camPos">Camera position</param>
	/// <param name="inWater">Is in water ?</param>
	void SetDataGPU(glm::mat4 M, glm::mat4 V, glm::mat4 P, glm::vec3 camPos, bool inWater) override {
		IMaterial::SetDataGPU(M, V, P, camPos, inWater);
		GLuint program = this->shader->GetProgram();

		glUseProgram(program);

		glUniform4f(glGetUniformLocation(program, ("material.albedo")), this->data->albedo.x, this->data->albedo.y, this->data->albedo.z, this->data->albedo.w);
		glUniform1f(glGetUniformLocation(program, ("material.metallic")), this->data->metallic);
		glUniform1f(glGetUniformLocation(program, ("material.roughness")), this->data->roughness);
		glUniform1f(glGetUniformLocation(program, ("material.ior")), this->data->ior);
		glUniform1f(glGetUniformLocation(program, ("material.transparent")), this->data->transparent);

		glUniform1i(glGetUniformLocation(program, ("material.albedoMap")), 0);
		glUniform1i(glGetUniformLocation(program, ("material.normalMap")), 1);
		glUniform1i(glGetUniformLocation(program, ("material.metallicMap")), 2);
		glUniform1i(glGetUniformLocation(program, ("material.roughnessMap")), 3);
		glUniform1i(glGetUniformLocation(program, ("material.aoMap")), 4);
		glUniform1i(glGetUniformLocation(program, ("m_heightmap")), 5);
		glUniform1i(glGetUniformLocation(program, ("p_data_physics")), 6);

		glActiveTexture(GL_TEXTURE0);
		this->data->albedoMap->Bind();
		glActiveTexture(GL_TEXTURE1);
		this->data->normalMap->Bind();
		glActiveTexture(GL_TEXTURE2);
		this->data->metallicMap->Bind();
		glActiveTexture(GL_TEXTURE3);
		this->data->roughnessMap->Bind();
		glActiveTexture(GL_TEXTURE4);
		this->data->aoMap->Bind();

		glActiveTexture(GL_TEXTURE5);
		this->data->heightMap->Bind();


		GLPhysic* p = this->attachment->getFirstComponentByType<GLPhysic>();
		int isDataP = 0;
		if (p != nullptr) {
			if (p->GetTexture() != -1) {
				glActiveTexture(GL_TEXTURE6);
				glBindTexture(GL_TEXTURE_2D, p->GetTexture());
				isDataP = 1;
			}
		}

		glUniform1i(glGetUniformLocation(program, ("u_is_data_physics")), isDataP);
	}
};

#endif