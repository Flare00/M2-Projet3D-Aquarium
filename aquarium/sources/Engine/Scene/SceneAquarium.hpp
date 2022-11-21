#ifndef __SCENE_AQUARIUM_HPP__
#define __SCENE_AQUARIUM_HPP__

#include <string>
#include <Script/ColorScript.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Displayable.hpp>
#include <Graphics/Light.hpp>
#include <Graphics/Material/MaterialPBR.hpp>
#include <Engine/Component/Model.hpp>
#include <Script/MovementScript.hpp>
#include "Scene.hpp"

class SceneAquarium : public Scene
{
public:
    SceneAquarium(std::string id) : Scene(id){
        this->root = new GameObject("Scene");

        Shader* shader = new Shader("pbr");
        this->AddShader(shader);

        MaterialPBR* cubesMaterial = (new MaterialPBR(glm::vec4(1)))->SetFolderData("Pipe", "png");
        MaterialPBR* groundMaterial = new MaterialPBR(glm::vec4(0.1,0.5,0.1,1));


        GameObject* camera = new GameObject("Camera", this->root);
        camera->addComponent(new Camera(camera, Camera::Settings::perspective(global.ScreenAspectRatio())));
        camera->addComponent(new MovementScript(camera));


        //GameObject* depthCam = new GameObject("depthCam", this->root);
        //depthCam->addComponent(new Camera(depthCam, Camera::Settings::perspective(global.ScreenAspectRatio()), Camera::DEPTH_STENCIL, this->AddShader(new Shader("depth"))));


        GameObject* obj0 = new GameObject("Obj", this->root);
        obj0->addComponent(new Displayable(obj0));
        obj0->addComponent(Model::LoadFromFile(shader, "Dragon/Dragon 2.5_fbx.fbx", cubesMaterial));
        obj0->getFirstComponentByType<Transformation>()->SetPosition(glm::vec3(0, 0, 5));;


        /*GameObject* obj1 = new GameObject("Obj1", this->root);
        obj1->addComponent(new Displayable(obj1));
        obj1->addComponent(Model::LoadFromFile(shader, "cube.obj", cubesMaterial));
        obj1->getFirstComponentByType<Transformation>()->SetPosition(glm::vec3(-5, 0, 5));;*/
        

        GameObject* obj2 = new GameObject("Obj2", this->root);
        obj2->addComponent(new Displayable(obj2));
        obj2->addComponent(Model::LoadFromFile(shader, "cube.obj", cubesMaterial));
        obj2->getFirstComponentByType<Transformation>()->SetPosition(glm::vec3(5, 0, 5));

        //obj2->addComponent(new MovementScript(obj2));

        GameObject* light = new GameObject("Light1", this->root);
        light->addComponent(new Light(light, Light::POINT, glm::vec3(1,1,1), 50.0, 1.0));
        light->getFirstComponentByType<Transformation>()->SetPosition(glm::vec3(0,5,0));


        GameObject* ground = new GameObject("Ground", this->root);
        ground->addComponent(new Displayable(ground));
        ground->addComponent(Model::DQuad(shader, groundMaterial));
        ground->getFirstComponentByType<Transformation>()->SetPosition(glm::vec3(0, -4, 5))->SetRotation(glm::vec3(90, 0, 0))->SetScale(50.0)->Update();
    }

};

#endif
