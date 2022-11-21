#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include <glm/glm.hpp>

class Material{
public : 
    struct Data{
        glm::vec3 color;
        Data(glm::vec3 color = glm::vec3(1,1,1)){
            this->color = color;
        }
    };
protected :
    Data data;
public :
    Material(Data data = Data()){
        this->data = data;
    }

    Material * SetColor(glm::vec3 color){
        this->data.color = color;
        return this;
    }

    Data GetData(){
        return this->data;
    }
};

#endif