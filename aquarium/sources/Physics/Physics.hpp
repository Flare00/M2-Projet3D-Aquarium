#ifndef __PHYSICS_HPP__
#define __PHYSICS_HPP__

#include <vector>
#include <Engine/SettedShaders.hpp>
#include <Engine/GameObject.hpp>
#include <Engine/Engine.hpp>
#include <Physics/IPhysics.hpp>

SettedShaders settedPhysicsShaders;

class Physics
{
public :
    Physics(){}
    void Init(){}

    void Compute(double deltatime, GameObject * root, int nbStep = 1){
        this->Compute(deltatime, root->getComponentsByTypeRecursive<IPhysics>(), nbStep);
    }

    void Compute( double deltatime, std::vector<IPhysics*> elems, int nbStep = 1){
        double stepDelta = deltatime / (double)nbStep;

        for(int i = 0 ; i < nbStep ; i++){
            double currentStep = stepDelta*((double)(i+1));

            for(size_t j = 0, max = elems.size(); j < max ; j++)
                elems[j]->Compute(currentStep);
        }
    }

    GameObject* Raycast(std::vector<GameObject*> elems, glm::vec3 origin, glm::vec3 direction) {
        // Use the Depth/Stencil (maybe faster) ? Or use collision ?
        for (size_t i = 0, max = elems.size(); i < max; i++) {

        }
        return nullptr;
    }
};

#endif