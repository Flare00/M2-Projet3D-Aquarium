#ifndef __SCENE_AQUARIUM_HPP__
#define __SCENE_AQUARIUM_HPP__

#include <string>
#include <Script/ColorScript.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Displayable.hpp>
#include <Graphics/Light.hpp>
#include <Graphics/Material/MaterialPBR.hpp>
#include <Graphics/Material/Material.hpp>
#include <Engine/Component/Model.hpp>
#include <Engine/Tools/ModelGenerator.hpp>
#include <Script/MovementScript.hpp>
#include "Scene.hpp"

class SceneAquarium : public Scene
{
public:
    SceneAquarium(std::string id) : Scene(id){
        this->root = new GameObject("Scene");

        IMaterial* cubesMaterial = (new MaterialPBR())->SetFolderData("Pipe", "png");
        IMaterial* groundMaterial = new MaterialPBR(glm::vec4(0.1,0.5,0.1,1));


        GameObject* camera = new GameObject("Camera", this->root);
        camera->addComponent(new Camera(Camera::Settings::perspective(global.ScreenAspectRatio())));
        camera->addComponent(new MovementScript());


        //GameObject* depthCam = new GameObject("depthCam", this->root);
        //depthCam->addComponent(new Camera(depthCam, Camera::Settings::perspective(global.ScreenAspectRatio()), Camera::DEPTH_STENCIL, this->AddShader(new Shader("depth"))));


        GameObject* obj0 = new GameObject("Obj", this->root);
        obj0->addComponent(new Displayable(obj0));
        obj0->addComponent(ModelGenerator::LoadFromFile("Dragon/Dragon 2.5_fbx.fbx", cubesMaterial));
        obj0->GetTransform()->SetPosition(glm::vec3(0, 0, 2));;


        /*GameObject* obj1 = new GameObject("Obj1", this->root);
        obj1->addComponent(new Displayable(obj1));
        obj1->addComponent(ModelGenerator::LoadFromFile(shader, "cube.obj", cubesMaterial));
        obj1->getFirstComponentByType<Transformation>()->SetPosition(glm::vec3(-5, 0, 5));;*/
        

        GameObject* obj2 = new GameObject("Obj2", this->root);
        obj2->addComponent(new Displayable(obj2));
        obj2->addComponent(ModelGenerator::LoadFromFile("sphere.fbx", cubesMaterial));
        obj2->GetTransform()->SetPosition(glm::vec3(2, 0, 2));

        //obj2->addComponent(new MovementScript(obj2));

        GameObject* light = new GameObject("Light1", this->root);
        light->addComponent(new Light(Light::POINT, glm::vec3(1,1,1), 100.0, 1.0));
        light->GetTransform()->SetPosition(glm::vec3(0,0,0));


        GameObject* ground = new GameObject("Ground", this->root);
        ground->addComponent(new Displayable(ground));
        ground->addComponent(ModelGenerator::DQuad(groundMaterial));
        ground->GetTransform()->SetPosition(glm::vec3(0, -4, 5))->SetRotation(glm::vec3(90, 0, 0))->SetScale(50.0)->Update();
    }

};

#endif
