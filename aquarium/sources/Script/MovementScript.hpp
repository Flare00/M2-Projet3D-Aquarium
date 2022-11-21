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
    float rSpeed = 1.0f;

    double lastXPos, lastYPos;
    bool firstMouse = true;

    //Movement
    int keyUp = GLFW_KEY_SPACE, keyDown = GLFW_KEY_LEFT_SHIFT;
    int keyFront = GLFW_KEY_W, keyBack = GLFW_KEY_S;
    int keyLeft = GLFW_KEY_A, keyRight = GLFW_KEY_D;

    //Rotations
    int keyRZneg =  GLFW_KEY_E, keyRZpos =GLFW_KEY_Q;
    
public:
    MovementScript(GameObject* attachment) : Script(attachment) {
        this->transformation = attachment->getFirstComponentByType<Transformation>();
    }

    virtual void loop(double deltaTime)
    {

        if (this->transformation == nullptr) {
            return;
        }

        keyboard(deltaTime);
        mouse(deltaTime);

        this->transformation->Update();
    }

    void keyboard(double deltaTime){
        //Translation X
        if (glfwGetKey(global.global_window, keyUp) == GLFW_PRESS) {
            this->transformation->Translate(glm::vec3(0, tSpeed*deltaTime,0));
        }
        if (glfwGetKey(global.global_window, keyDown) == GLFW_PRESS) {
            this->transformation->Translate(glm::vec3(0, -tSpeed * deltaTime, 0));
        }
        //Translation Y
        if (glfwGetKey(global.global_window, keyFront) == GLFW_PRESS) {
            this->transformation->Translate(glm::vec3(0, 0, tSpeed * deltaTime));
        }
        if (glfwGetKey(global.global_window, keyBack) == GLFW_PRESS) {
            this->transformation->Translate(glm::vec3(0, 0, -tSpeed * deltaTime));
        }
        //Translation Z
        if (glfwGetKey(global.global_window, keyRight) == GLFW_PRESS) {
            this->transformation->Translate(glm::vec3(tSpeed * deltaTime, 0, 0));
        }
        if (glfwGetKey(global.global_window, keyLeft) == GLFW_PRESS) {
            this->transformation->Translate(glm::vec3(-tSpeed * deltaTime, 0, 0));
        }

        //Rotation Z
        if (glfwGetKey(global.global_window, keyRZneg) == GLFW_PRESS) {
            this->transformation->Rotate(glm::vec3(0, 0, -rSpeed * deltaTime));
        }
        if (glfwGetKey(global.global_window, keyRZpos) == GLFW_PRESS) {
            this->transformation->Rotate(glm::vec3(0, 0, rSpeed * deltaTime));
        }
    }

    void mouse(double deltaTime){
        if(!firstMouse){
            double dx = global.mouseX - lastXPos, dy = global.mouseY - lastYPos;
            printf("%f , %f\n", dx, dy);

            //Rotation X
            if(dx != 0){
                this->transformation->Rotate(glm::vec3( 0 ,rSpeed * dx * deltaTime , 0));
            } 
            //Rotation Y
            if(dy != 0){
                this->transformation->Rotate(glm::vec3(rSpeed * dy * deltaTime, 0, 0));
            } 
        } else {
            firstMouse = false;
        }
        this->lastXPos = global.mouseX;
        this->lastYPos = global.mouseY;
    }

};

#endif