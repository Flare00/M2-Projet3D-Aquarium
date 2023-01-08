#ifndef __GL_PHYSIC_HPP__
#define __GL_PHYSIC_HPP__

#include <GL/glew.h>
#include <Physics/Physics/CPhysic.hpp>

/// <summary>
/// Physic using GPU, inherit CPhysic.
/// </summary>
class GLPhysic : public CPhysic
{
protected:
    GLuint texture = -1;
public:
    /// <summary>
    /// Graphics Physics Element
    /// </summary>
    GLPhysic(): CPhysic(false){}
    /// <summary>
    /// Return the Texture of the Graphic physics
    /// </summary>
    /// <returns>the Texture</returns>
    GLuint GetTexture(){
        return this->texture;
    }
};

#endif