#ifndef __GL_PHYSIC_HPP__
#define __GL_PHYSIC_HPP__

#include <GL/glew.h>
#include <Physics/Physics/CPhysic.hpp>

class GLPhysic : public CPhysic
{
protected:
    GLuint texture = -1;
public:
    GLPhysic(){}
    GLuint GetTexture(){
        return this->texture;
    }
};

#endif