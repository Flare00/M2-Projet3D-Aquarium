#ifndef __MOVEMENT_SCRIPT_HPP__
#define __MOVEMENT_SCRIPT_HPP__


#include <Engine/Component/Script.hpp>
#include <Engine/EngineBehavior.hpp>
#include <Engine/Component/Transformation.hpp>
#include <GLFW/glfw3.h>


class MovementScript : public Script, public EngineBehavior
{
private:
    Transformation* transformation;
    float tSpeed = 1.0f;
    float rSpeed = 10.0f;

    //Movement
    int keyUp = GLFW_KEY_SPACE, keyDown = GLFW_KEY_LEFT_SHIFT;
    int keyFront = GLFW_KEY_W, keyBack = GLFW_KEY_S;
    int keyLeft = GLFW_KEY_A, keyRight = GLFW_KEY_D;

    //Rotations
    int keyRXneg = GLFW_KEY_I, keyRXpos = GLFW_KEY_K;
    int keyRYneg = GLFW_KEY_J, keyRYpos = GLFW_KEY_L;
    int keyRZneg = GLFW_KEY_U, keyRZpos = GLFW_KEY_O;
    
public:
    MovementScript(GameObject* attachment) : Script(attachment) {
        this->transformation = attachment->getFirstComponentByType<Transformation>();
    }

    virtual void loop(double deltaTime)
    {

         if (this->transformation == nullptr) {
            return;
        }

        //TRANSLATION
        if (glfwGetKey(global.global_window, keyUp) == GLFW_PRESS) {
            this->transformation->Translate(glm::vec3(0, tSpeed*deltaTime,0));
        }
        if (glfwGetKey(global.global_window, keyDown) == GLFW_PRESS) {
            this->transformation->Translate(glm::vec3(0, -tSpeed * deltaTime, 0));
        }

        if (glfwGetKey(global.global_window, keyFront) == GLFW_PRESS) {
            this->transformation->Translate(glm::vec3(0, 0, tSpeed * deltaTime));
        }
        if (glfwGetKey(global.global_window, keyBack) == GLFW_PRESS) {
            this->transformation->Translate(glm::vec3(0, 0, -tSpeed * deltaTime));
        }

        if (glfwGetKey(global.global_window, keyRight) == GLFW_PRESS) {
            this->transformation->Translate(glm::vec3(tSpeed * deltaTime, 0, 0));
        }
        if (glfwGetKey(global.global_window, keyLeft) == GLFW_PRESS) {
            this->transformation->Translate(glm::vec3(-tSpeed * deltaTime, 0, 0));
        }

        //ROTATION
        if (glfwGetKey(global.global_window, keyRXneg) == GLFW_PRESS) {
            this->transformation->Rotate(glm::vec3( -rSpeed * deltaTime,0 , 0));
        }
        if (glfwGetKey(global.global_window, keyRXpos) == GLFW_PRESS) {
            this->transformation->Rotate(glm::vec3(rSpeed * deltaTime, 0, 0));
        }

        if (glfwGetKey(global.global_window, keyRYneg) == GLFW_PRESS) {
            this->transformation->Rotate(glm::vec3(0, rSpeed * deltaTime, 0));
        }
        if (glfwGetKey(global.global_window, keyRYpos) == GLFW_PRESS) {
            this->transformation->Rotate(glm::vec3(0, -rSpeed * deltaTime, 0));
        }

        if (glfwGetKey(global.global_window, keyRZneg) == GLFW_PRESS) {
            this->transformation->Rotate(glm::vec3(0, 0, -rSpeed * deltaTime));
        }
        if (glfwGetKey(global.global_window, keyRZpos) == GLFW_PRESS) {
            this->transformation->Rotate(glm::vec3(0, 0, rSpeed * deltaTime));
        }

        this->transformation->Update();
    }
};

#endif