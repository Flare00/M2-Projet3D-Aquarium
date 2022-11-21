#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include <vector>
#include <string>
#include <GLFW/glfw3.h>

#include "Scene/SceneAquarium.hpp"
#include <Engine/GameObject.hpp>
#include <Graphics/Graphics.hpp>

#include <Engine/Tools.hpp>
class Engine{
protected:
    size_t activeScene = -1;
    std::vector<Scene> scenes;
    size_t nbScene = 0;
    Graphics graphics;
public:
    Engine(){
        scenes.push_back(SceneAquarium("lol"));
        activeScene = 0;
        
        nbScene = scenes.size();
        if(activeScene >= 0 && activeScene < nbScene){
            scenes[activeScene].start();
        }
    }

    void loop(double deltaT){

        if(activeScene >= 0 && activeScene < nbScene){
            if (glfwGetKey(global.global_window, GLFW_KEY_R) == GLFW_PRESS) {
                scenes[activeScene].ReloadShaders();
            }

            scenes[activeScene].loop(deltaT);

            GameObject* root = scenes[activeScene].GetRoot();
            graphics.Compute(root);
        }
    }

    void changeScene(size_t scene_number){
        if(scene_number >= 0 && scene_number < nbScene){
            scenes[activeScene].stop();
            activeScene = scene_number;
            scenes[activeScene].start();
        }
    }
    void changeScene(std::string scene_id){
        size_t index = -1;
        for(size_t i = 0, max = scenes.size(); i < max && index == -1; i++){
            if(scenes[i].id == scene_id){
                index = i;
            }
        }
        this->changeScene(index);
    }
};

#endif