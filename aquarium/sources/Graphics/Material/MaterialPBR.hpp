#ifndef __MATERIAL_PBR_HPP__
#define __MATERIAL_PBR_HPP__

#include <glm/glm.hpp>
#include <GL/gl.h>
#include <string>
#include <Graphics/Material/IMaterial.hpp>
#include <Graphics/Texture/Texture.hpp>

std::string textureFolder = "assets/Texture/";

class MaterialPBR : public IMaterial {
public:
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

		Data(glm::vec4 albedo, float metallic, float roughness, Texture* albedoMap, Texture* normalMap, Texture* metallicMap, Texture* roughnessMap, Texture* aoMap, Texture* heightMap) {
			this->albedo = albedo;
			this->metallic = metallic;
			this->roughness = roughness;

			this->albedoMap = albedoMap;
			this->normalMap = normalMap;
			this->metallicMap = metallicMap;
			this->roughnessMap = roughnessMap;
			this->aoMap = aoMap;
			this->heightMap = heightMap;
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
	MaterialPBR(glm::vec4 albedo = glm::vec4(1.0), float metallic = 0.0f, float roughness = 0.0f,
		std::string albedoMapFile = "", std::string normalMapFile = "", std::string metallicMapFile = "", std::string roughnessMapFile = "", std::string aoMapFile = "", std::string heightMapFile = "")
		: IMaterial("pbr")
	{

		this->albedoMapFile = albedoMapFile;
		this->normalMapFile = normalMapFile;
		this->metallicMapFile = metallicMapFile;
		this->roughnessMapFile = roughnessMapFile;
		this->aoMapFile = aoMapFile;
		this->heightMapFile = heightMapFile;

		this->data = new Data(albedo, metallic, roughness,
			new Texture(albedoMapFile, glm::vec4(1)),
			new Texture(normalMapFile, glm::vec4(0)),
			new Texture(metallicMapFile, glm::vec4(0)),
			new Texture(roughnessMapFile, glm::vec4(0)),
			new Texture(aoMapFile, glm::vec4(0)),
			new Texture(heightMapFile, glm::vec4(0))
		);
	}

	MaterialPBR* SetAlbedo(glm::vec4 albedo) {
		this->data->albedo = albedo;
		return this;
	}

	MaterialPBR* SetMetallic(float metallic) {
		this->data->metallic = metallic;
		return this;
	}

	MaterialPBR* SetRoughness(float roughness) {
		this->data->roughness = roughness;
		return this;
	}

	MaterialPBR* SetAlbedoMapFile(std::string albedoMapFile) {
		this->albedoMapFile = albedoMapFile;
		this->data->albedoMap->SetPath(this->albedoMapFile);
		return this;
	}

	MaterialPBR* SetNormalMapFile(std::string normalMapFile) {
		this->normalMapFile = normalMapFile;
		this->data->normalMap->SetPath(this->normalMapFile);
		return this;
	}

	MaterialPBR* SetMetallicMapFile(std::string metallicMapFile) {
		this->metallicMapFile = metallicMapFile;
		this->data->metallicMap->SetPath(this->metallicMapFile);
		return this;
	}

	MaterialPBR* SetRoughnessMapFile(std::string roughnessMapFile) {
		this->roughnessMapFile = roughnessMapFile;
		this->data->roughnessMap->SetPath(this->roughnessMapFile);
		return this;
	}

	MaterialPBR* SetAOMapFile(std::string aoMapFile) {
		this->aoMapFile = aoMapFile;
		this->data->aoMap->SetPath(this->aoMapFile);
		return this;
	}


	MaterialPBR* SetHeightMapFile(std::string heightMapFile) {
		this->heightMapFile = heightMapFile;
		this->data->heightMap->SetPath(this->heightMapFile);
		return this;
	}

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

	const Data* GetData() {
		return this->data;
	}

	void SetDataGPU(glm::mat4 M, glm::mat4 V, glm::mat4 P, glm::vec3 camPos) override {
		IMaterial::SetDataGPU(M, V, P, camPos);
		GLuint program = this->shader->GetProgram();

		glUseProgram(program);
		glUniform4f(glGetUniformLocation(program, ("material.albedo")), this->data->albedo.x, this->data->albedo.y, this->data->albedo.z, this->data->albedo.w);
		glUniform1f(glGetUniformLocation(program, ("material.metallic")), this->data->metallic);
		glUniform1f(glGetUniformLocation(program, ("material.roughness")), this->data->roughness);

		glUniform1i(glGetUniformLocation(program, ("material.albedoMap")), 0);
		glUniform1i(glGetUniformLocation(program, ("material.normalMap")), 1);
		glUniform1i(glGetUniformLocation(program, ("material.metallicMap")), 2);
		glUniform1i(glGetUniformLocation(program, ("material.roughnessMap")), 3);
		glUniform1i(glGetUniformLocation(program, ("material.aoMap")), 4);
		glUniform1i(glGetUniformLocation(program, ("m_heightmap")),5);

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


	}
};

#endif