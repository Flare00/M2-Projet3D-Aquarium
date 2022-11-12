#ifndef __TRANSFORMATION_HPP__
#define __TRANSFORMATION_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <Engine/Component/Component.hpp>

class Transformation : public Component
{
private :
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    bool dirty = true;
    glm::mat4 matrix = glm::mat4(1.0);

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

    void Translate(glm::vec3 translation) {
        this->dirty = true;
        this->position += translation;
    }

    void setRotation(glm::vec3 rotation){
        this->dirty = true;
        this->rotation = rotation;
    }

    void Rotate(glm::vec3 rotation) {
        this->dirty = true;
        this->rotation += rotation;
    }

    void setScale(glm::vec3 scale){
        this->dirty = true;
        this->scale = scale;
    }

    void setScale(double scale){
        this->dirty = true;
        this->scale = glm::vec3(scale);
    }


    void Scale(double scale) {
        this->dirty = true;
        this->scale += glm::vec3(scale);
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
        

        glm::mat4 scaleM(1.0f);
        glm::mat4 rotationM(1.0f);
        glm::mat4 translation(1.0f);
        scaleM = glm::scale(scaleM, this->scale);

        //tourner la matrice selon les 3 axes
        rotationM = glm::rotate(rotationM, this->rotation[1], glm::vec3(0, 1, 0));
        rotationM = glm::rotate(rotationM, this->rotation[0], glm::vec3(1, 0, 0));
        rotationM = glm::rotate(rotationM, this->rotation[2], glm::vec3(0, 0, 1));
        //scale la matrice;
        //deplacer la matrice
        translation = glm::translate(translation, this->position);

        this->matrix = translation * (rotationM * scaleM);
    }

    glm::mat4 getMatrix(){
        if(dirty){
            computeMatrix();
        }
        return this->matrix;
    }



};

#endif
