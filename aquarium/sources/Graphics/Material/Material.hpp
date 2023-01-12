#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include <glm/glm.hpp>
#include <Graphics/Material/IMaterial.hpp>

/// <summary>
/// Simple Material. deprecated
/// </summary>
class Material : public IMaterial{
public : 
    /// <summary>
    /// Material Data.
    /// </summary>
    struct Data{
        glm::vec4 color;
        Data(glm::vec4 color){
            this->color = color;
        }
    };
protected :
    Data* data;
public :
    /// <summary>
    /// A Simple material, deprecated.
    /// </summary>
    /// <param name="color">The color of the material</param>
    Material(glm::vec4 color = glm::vec4(1)) : IMaterial("std") {
        this->data = new Data(color);
    }

    /// <summary>
    /// Set the color.
    /// </summary>
    /// <param name="color">New color</param>
    /// <returns>Self.</returns>
    Material * SetColor(glm::vec4 color){
        this->data->color = color;
        return this;
    }

    /// <summary>
    /// return the data of the material.
    /// </summary>
    /// <returns>The data</returns>
    Data * GetData(){
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
    void SetDataGPU(glm::mat4 M, glm::mat4 V, glm::mat4 P, glm::vec3 camPos, bool inWater, bool mainRender, bool caustics) override {
        IMaterial::SetDataGPU(M, V, P, camPos, inWater, mainRender, caustics);
        GLuint program = this->shader->GetProgram();
        glUseProgram(program);
        glUniform4f(glGetUniformLocation(program, "material.color"), this->data->color.x, this->data->color.y, this->data->color.z, this->data->color.w);
    }
};

#endif