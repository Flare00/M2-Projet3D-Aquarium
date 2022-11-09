#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <vector>
#include <string>

#include <Engine/EngineBehavior.hpp>
#include <Engine/GameObject.hpp>
#include <Script/ColorScript.hpp>
class Scene : public GameBehavior
{
protected :
    GameObject * root;

public : 
    std::string id;

    Scene(std::string id){ 
        this->id = id;
    }
    ~Scene(){}

    //GameBehavior
    virtual void start(){
        //start all scripts
        if(root != NULL){
            std::vector<Script*> scripts = root->getComponentsByTypeRecursive();
            for(size_t i = 0, max = scripts.size(); i < max ; i++){
                if(!scripts[i]->started){
                    scripts[i]->start();
                }
            }
        }

    }
    
    virtual void loop(double deltaT){
        //Loop all scripts and start the non started one.
        std::vector<Script*> scripts = root->getComponentsByTypeRecursive();
        for(size_t i = 0, max = scripts.size(); i < max ; i++){
            if(!scripts[i]->started){
                scripts[i]->start();
            }
            scripts[i]->loop(deltaT);
        }
    }

    virtual void stop(){
        //stop all the started scripts
        std::vector<Script*> scripts = root->getComponentsByTypeRecursive();
        for(size_t i = 0, max = scripts.size(); i < max ; i++){
            if(scripts[i]->started){
                scripts[i]->stop();
            }
        }
    }
};

#endif