#ifndef __SCRIPT_HPP__
#define __SCRIPT_HPP__

#include <Engine/GameObject.hpp>
#include <Engine/Component/Component.hpp>

class Script : public Component
{
protected:
    GameObject * attachment;
public : 
    Script(GameObject * attachment){
        this->attachment = attachment;
    }
};

#endif