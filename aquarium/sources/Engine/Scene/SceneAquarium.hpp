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
	SceneAquarium(std::string id) : Scene(id) {
		this->root = new GameObject("Scene");

		IMaterial* pipeMaterial = (new MaterialPBR())->SetFolderData("Pipe", "png");
		//IMaterial* dragonMaterial = (new MaterialPBR())->SetFolderData("Dragon", "jpg");

		IMaterial* waterMaterial = new MaterialPBR(glm::vec4(0.0, 0.66, 0.8, 0.7), 0.0f, 0.0f, 1.33f, true);
		IMaterial* glassMaterial = new MaterialPBR(glm::vec4(1, 1, 1, 0.3), 0.0f, 0.0f, 1.5f, true);
		IMaterial* baseAquariumMaterial = new MaterialPBR(glm::vec4(0.5, 0.5, 0.5, 1.0));

		GameObject* camera = new GameObject("Camera", this->root);
		camera->addComponent(new Camera(Camera::Settings::perspective(global.ScreenAspectRatio())));
		camera->addComponent(new MovementScript());


		//GameObject* depthCam = new GameObject("depthCam", this->root);
		//depthCam->addComponent(new Camera(depthCam, Camera::Settings::perspective(global.ScreenAspectRatio()), Camera::DEPTH_STENCIL, this->AddShader(new Shader("depth"))));


		/*GameObject* obj0 = new GameObject("Dragon", this->root);
		obj0->addComponent(new Displayable());
		obj0->addComponent(ModelGenerator::LoadFromFile("Dragon/Dragon 2.5_fbx.fbx", dragonMaterial));
		obj0->GetTransform()->SetPosition(glm::vec3(0, 0, 4))->SetRotation(glm::vec3(-90, 180, 0));*/

		GameObject* light = new GameObject("Light1", this->root);
		light->addComponent(new Light(Light::POINT, glm::vec3(1, 1, 1), 10.0, 1.0));
		light->GetTransform()->SetPosition(glm::vec3(0, 0, 0));


		//Create the aquarium
		GameObject* aquarium = new GameObject("Aquarium", this->root);
		GameObject* bottomAquarium = new GameObject("Aquarium Bottom", aquarium);
		bottomAquarium->addComponent(ModelGenerator::Cube(baseAquariumMaterial, 1, glm::vec3(4, 0.05, 2), glm::vec3(0, -2, 0)));
		bottomAquarium->addComponent(new Displayable());

		GameObject* frontAquarium = new GameObject("Aquarium Front", aquarium);
		frontAquarium->addComponent(ModelGenerator::Cube(glassMaterial, 1, glm::vec3(4, 2, 0.05), glm::vec3(0, 0, -2)));
		frontAquarium->addComponent(new Displayable());

		GameObject* backAquarium = new GameObject("Aquarium Back", aquarium);
		backAquarium->addComponent(ModelGenerator::Cube(glassMaterial, 1, glm::vec3(4, 2, 0.05), glm::vec3(0, 0, 2)));
		backAquarium->addComponent(new Displayable());

		GameObject* leftAquarium = new GameObject("Aquarium Left", aquarium);
		leftAquarium->addComponent(ModelGenerator::Cube(glassMaterial, 1, glm::vec3(0.05, 2, 2), glm::vec3(-4, 0, 0)));
		leftAquarium->addComponent(new Displayable());

		GameObject* rightAquarium = new GameObject("Aquarium Right", aquarium);
		rightAquarium->addComponent(ModelGenerator::Cube(glassMaterial, 1, glm::vec3(0.05, 2, 2), glm::vec3(4, 0, 0)));
		rightAquarium->addComponent(new Displayable());





		//Create Water of the aquarium
		GameObject* water = new GameObject("water", aquarium);
		water->addComponent(new Displayable());
		water->addComponent(ModelGenerator::CubeWater(waterMaterial, 512, 512, 4, 4));
		WaterPhysics* waterP = new WaterPhysics(256,256);
		water->addComponent(waterP);
		water->GetTransform()->SetPosition(glm::vec3(0, 1,0));

		//Add a drop
		waterP->AddDrop(glm::vec2(0.5, 0.5), 0.05f, 0.1f);

		aquarium->GetTransform()->SetPosition(glm::vec3(0, -2, 2));

	}

};

#endif
