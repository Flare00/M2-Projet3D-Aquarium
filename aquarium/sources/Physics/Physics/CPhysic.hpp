#ifndef __C_PHYSIC_HPP__
#define __C_PHYSIC_HPP__

#include <GL/glew.h>
#include <Engine/Component/Component.hpp>

/// <summary>
/// A standard Compute Physics, using CPU.
/// </summary>
class CPhysic : public Component
{
protected:
    bool threadable;
public:
    /// <summary>
    /// Create a Compute physic element (using CPU)
    /// </summary>
    /// <param name="threadable">Can be threaded ? Not implemented yet</param>
    CPhysic(bool threadable){
        this->threadable = threadable;
    }
    /// <summary>
    /// Compute the physic
    /// </summary>
    /// <param name="delta">Time since last frame</param>
    virtual void Compute(double delta){}
};

#endif