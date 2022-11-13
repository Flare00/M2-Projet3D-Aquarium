#ifndef __SCENE_AQUARIUM_HPP__
#define __SCENE_AQUARIUM_HPP__

#include "Scene.hpp"
#include <Script/ColorScript.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Displayable.hpp>
#include <Engine/Component/Model.hpp>
#include <Script/MovementScript.hpp>

class SceneAquarium : public Scene
{
public:
    SceneAquarium(std::string id) : Scene(id){
        this->root = new GameObject("Scene");

        GameObject* camera = new GameObject("Camera", this->root);
        camera->addComponent(new Camera(camera, Camera::Settings::perspective(global.ScreenAspectRatio())));
        camera->addComponent(new MovementScript(camera));

        //GameObject* depthCam = new GameObject("depthCam", this->root);
        //depthCam->addComponent(new Camera(depthCam, Camera::Settings::perspective(global.ScreenAspectRatio()), Camera::TEXTURE, this->AddShader(new Shader("depth"))));

        GameObject* obj1 = new GameObject("Obj", this->root);
        obj1->addComponent(new Displayable(obj1));
        obj1->addComponent(Model::Cube(this->AddShader(new Shader("std"))));
        //obj1->addComponent(new MovementScript(obj1));
        Transformation* t = obj1->getFirstComponentByType<Transformation>();
        t->setPosition(glm::vec3(1, 0, -5 ));
    }

};

#endif
