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
#include <Physics/Collider/BoundingBoxCollider.hpp>
#include <Physics/WaterPhysics.hpp>

class SceneAquarium : public Scene
{
public:
    SceneAquarium(std::string id) : Scene(id){
        this->root = new GameObject("Scene");

        BoundingBoxCollider* bb1 = new BoundingBoxCollider();
        BoundingBoxCollider* bb2 = new BoundingBoxCollider();
        bb1->IsColliding(bb2);

        IMaterial* pipeMaterial = (new MaterialPBR())->SetFolderData("Pipe", "png");
        IMaterial* dragonMaterial = (new MaterialPBR())->SetFolderData("Dragon", "jpg");
        IMaterial* waterMaterial = new MaterialPBR(glm::vec4(0.0,0.66,0.8,0.7));

        Shader * waterPhysicsShader = new Shader("Physics/water.vert", "Physics/water.frag");

        GameObject* camera = new GameObject("Camera", this->root);
        camera->addComponent(new Camera(Camera::Settings::perspective(global.ScreenAspectRatio())));
        camera->addComponent(new MovementScript());


        //GameObject* depthCam = new GameObject("depthCam", this->root);
        //depthCam->addComponent(new Camera(depthCam, Camera::Settings::perspective(global.ScreenAspectRatio()), Camera::DEPTH_STENCIL, this->AddShader(new Shader("depth"))));


        GameObject* obj0 = new GameObject("Dragon", this->root);
        obj0->addComponent(new Displayable(obj0));
        obj0->addComponent(ModelGenerator::LoadFromFile("Dragon/Dragon 2.5_fbx.fbx", dragonMaterial));
        obj0->GetTransform()->SetPosition(glm::vec3(0, 0, 4))->SetRotation(glm::vec3(-90, 180, 0));


        /*GameObject* obj1 = new GameObject("Obj1", this->root);
        obj1->addComponent(new Displayable(obj1));
        obj1->addComponent(ModelGenerator::LoadFromFile(shader, "cube.obj", cubesMaterial));
        obj1->getFirstComponentByType<Transformation>()->SetPosition(glm::vec3(-5, 0, 5));;*/
        

        GameObject* obj2 = new GameObject("Sphere", this->root);
        obj2->addComponent(new Displayable(obj2));
        //obj2->addComponent(ModelGenerator::LoadFromFile("sphere.fbx", cubesMaterial, 0.01));
        obj2->addComponent(ModelGenerator::Cube(pipeMaterial));
        obj2->GetTransform()->SetPosition(glm::vec3(0, -4, 2));

        //obj2->addComponent(new MovementScript(obj2));

        GameObject* light = new GameObject("Light1", this->root);
        light->addComponent(new Light(Light::POINT, glm::vec3(1,1,1), 10.0, 1.0));
        light->GetTransform()->SetPosition(glm::vec3(0,0,0));


        GameObject* water = new GameObject("water", this->root);
        water->addComponent(new Displayable(water));
        water->addComponent(ModelGenerator::Quad(waterMaterial, 128, 128, 4,4));
        //water->addComponent(new WaterPhysics(waterPhysicsShader, 128));
        water->GetTransform()->SetPosition(glm::vec3(0, -1, 2));
    }

};

#endif
