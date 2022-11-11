#ifndef __SCENE_AQUARIUM_HPP__
#define __SCENE_AQUARIUM_HPP__

#include "Scene.hpp"
#include <Script/ColorScript.hpp>

class SceneAquarium : public Scene
{
public:
    SceneAquarium(std::string id) : Scene(id){
        this->root = new GameObject("Scene");
        this->root->addComponent(new ColorScript(root, 1));
    }
};

#endif
