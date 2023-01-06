#ifndef __C_PHYSIC_HPP__
#define __C_PHYSIC_HPP__

#include <GL/glew.h>
#include <Engine/Component/Component.hpp>

class CPhysic : public Component
{
protected:
    bool threadable;
public:
    CPhysic(bool threadable){
        this->threadable = threadable;
    }
    virtual void Compute(double delta){}
};

#endif