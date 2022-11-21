#ifndef __MATERIAL_PBR_HPP__
#define __MATERIAL_PBR_HPP__

#include <glm/glm.hpp>
#include <GL/gl.h>
#include <string>
class MaterialPBR{
public : 
    struct DataGraphical{
        GLuint albedoMap;
        GLuint metallicMap;
        GLuint roughnessMap;
        GLuint normalMap;
        GLuint aoMap;

        glm::vec3 albedo;
        float metallic;
        float roughness;

        DataGraphical(glm::vec3 albedo, float metallic, float roughness, GLuint albedoMap, GLuint normalMap, GLuint metallicMap, GLuint roughnessMap, GLuint aoMap){
            this->albedo = albedo;
            this->metallic = metallic;
            this->roughness = roughness;
            this->albedoMap = albedoMap;
            this->normalMap = normalMap;
            this->metallicMap = metallicMap;
            this->roughnessMap = roughnessMap;
            this->aoMap = aoMap;
        }
    };

protected :
    DataGraphical dataGraphical;

    std::string albedoFile;
    std::string metallicFile;
    std::string normalMapFile;
    std::string aoMapFile;
    std::string heightMapFile;



public :
    MaterialPBR(){
        
    }

    MaterialPBR * SetAlbedo(glm::vec3 albedo){
        return this;
    }
    MaterialPBR * SetAlbedo(std::string albedoFile){

        return this;
    }

    Data GetData(){
        return this->data;
    }
};

#endif