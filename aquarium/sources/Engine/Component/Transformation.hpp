#ifndef __TRANSFORMATION_HPP__
#define __TRANSFORMATION_HPP__

#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Engine/Component/Component.hpp>

class Transformation : public Component
{
private :
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    bool dirty = true;
    glm::mat4 matrix;

public :
    Transformation(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
        this->position = position;
        this->rotation = rotation;
        this->scale = scale;
    }

    Transformation(glm::vec3 position){
        this->position = position;
        this->rotation = glm::vec3(0.0);
        this->scale = glm::vec3(1);
    }

    Transformation(){
        this->position = glm::vec3(0);
        this->rotation = glm::vec3(0);
        this->scale = glm::vec3(1);
    }

    void setPosition(glm::vec3 position){
        this->dirty = true;
        this->position = position;
    }

    void setRotation(glm::vec3 rotation){
        this->dirty = true;
        this->rotation = rotation;
    }

    void setScale(glm::vec3 scale){
        this->dirty = true;
        this->scale = scale;
    }

    void setScale(double scale){
        this->dirty = true;
        this->scale = glm::vec3(scale);
    }

    glm::vec3 getPosition(){
        return this->position;
    }
    glm::vec3 getRotation(){
        return this->rotation;
    }
    glm::vec3 getScale(){
        return this->scale;
    }

    void computeMatrix(){
        //faire la matrice identitée
        this->matrix = glm::mat4(1.0f);
        //scale la matrice;
        this->matrix = glm::scale(this->matrix, this->scale);
        //tourner la matrice selon les 3 axes
        this->matrix = glm::rotate(this->matrix, this->rotation[1], glm::vec3(0,1,0));
        this->matrix = glm::rotate(this->matrix, this->rotation[0], glm::vec3(1,0,0));
        this->matrix = glm::rotate(this->matrix, this->rotation[2], glm::vec3(0,0,1));
        //deplacer la matrice
        this->matrix = glm::translate(this->matrix, this->position);
    }

    glm::mat4 getMatrix(){
        if(dirty){
            computeMatrix();
        }
        return this->matrix;
    }



};

#endif
