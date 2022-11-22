#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include <glm/glm.hpp>
#include <Graphics/Material/IMaterial.hpp>
class Material : public IMaterial{
public : 
    struct Data{
        glm::vec4 color;
        Data(glm::vec4 color){
            this->color = color;
        }
    };
protected :
    struct STDUniform {
        GLuint color;
        STDUniform(GLuint program, std::string color) {
            glUseProgram(program);
            this->color = glGetUniformLocation(program, color.c_str());
        }
    };
    STDUniform* stdUniform;
    Data* data;
public :
    Material(glm::vec4 color = glm::vec4(1)) : IMaterial("std") {
        this->data = new Data(color);
        this->stdUniform = new STDUniform(this->shader->GetProgram(), "material.color");
    }

    Material * SetColor(glm::vec4 color){
        this->data->color = color;
        return this;
    }

    Data * GetData(){
        return this->data;
    }

    void SetDataGPU(glm::mat4 M, glm::mat4 V, glm::mat4 P, glm::vec3 camPos) override {
        IMaterial::SetDataGPU(M, V, P, camPos);

        glUniform4f(stdUniform->color, this->data->color.x, this->data->color.y, this->data->color.z, this->data->color.w);
    }
};

#endif