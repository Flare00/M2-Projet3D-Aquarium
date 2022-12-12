#ifndef __RIGIDBODY_HPP__
#define __RIGIDBODY_HPP__


#include <Physics/Physics/CPhysic.hpp>
#include <glm/glm.hpp>

class Rigidbody : public CPhysic
{
protected:
    float mass = 1.0f;
    float drag = 0.0f;
    glm::vec3 velocity = glm::vec3(0);
    glm::vec3 acceleration = glm::vec3(0);
    bool gravity = false;

public :
    Rigidbody(float mass = 1.0f, float drag = 0.0f, bool gravity = false){
        this->mass = mass;
        this->drag = drag;
        this->gravity = gravity;
    }

    void SetVelocity(glm::vec3 velocity){
        this->velocity = velocity;
    }

    glm::vec3 GetVelocity(){
        return this->velocity;
    }

    void SetAcceleration(glm::vec3 acceleration){
        this->acceleration = acceleration;
    }

    glm::vec3 GetAcceleration(){
        return this->acceleration;
    }

    bool IsGravity(){
        return gravity;
    }

    void Impulse(glm::vec3 impulse){

    }

    void AddForce(glm::vec3 force){

    }

    void Compute(double delta) override {
        this->attachment->GetTransform()->Translate(this->velocity * (float)delta);
    }
};
#endif