#ifndef __I_PHYSICS_HPP__
#define __I_PHYSICS_HPP__

#include <GL/glew.h>
#include <Engine/Component/Component.hpp>

class IPhysics : public Component
{
protected:
    GLuint texture = -1;
public:
    IPhysics(){}
    virtual void Compute(double delta){}

    GLuint GetTexture(){
        return this->texture;
    }
};

#endif