#ifndef __PHYSICS_HPP__
#define __PHYSICS_HPP__

#include <vector>
#include <Engine/SettedShaders.hpp>
#include <Engine/GameObject.hpp>
#include <Physics/IPhysics.hpp>

SettedShaders settedPhysicsShaders;

class Physics
{
public :
    Physics(){}


    void Compute(double deltatime, GameObject * root, int nbStep = 1){
        this->Compute(deltatime, root->getComponentsByTypeRecursive<IPhysics>(), nbStep);
    }

    void Compute( double deltatime, std::vector<IPhysics*> elems, int nbStep = 1){
        double stepDelta = deltatime / (double)nbStep;
        for(int i = 0 ; i < nbStep ; i++){
            double currentStep = stepDelta*(double)i;
            for(size_t j = 0, max = elems.size(); j < max ; j++)
                elems[j]->Compute(currentStep);
        }
    }

};

#endif