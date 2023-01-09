#ifndef __SCENE_AQUARIUM_HPP__
#define __SCENE_AQUARIUM_HPP__

#include <string>
#include <Script/ColorScript.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Displayable.hpp>
#include <Graphics/WaterAffected.hpp>
#include <Graphics/Light.hpp>
#include <Graphics/Material/MaterialPBR.hpp>
#include <Graphics/Material/Material.hpp>
#include <Engine/Component/Model.hpp>
#include <Engine/Tools/ModelGenerator.hpp>
#include <Script/MovementScript.hpp>
#include "Scene.hpp"
#include <Physics/Collider/BoundingBoxCollider.hpp>
#include <Physics/GLPhysics/WaterPhysics.hpp>
#include <Physics/Physics/Rigidbody.hpp>
#include <Physics/Collider/SphereCollider.hpp>

/// <summary>
/// The aquarium Scene
/// </summary>
class SceneAquarium : public Scene
{
public:
	SceneAquarium(std::string id) : Scene(id) {
		//Create the root object.
		this->root = new GameObject("Scene");

		//Create all the PBR materials needed.

		IMaterial* pipeMaterial = (new MaterialPBR())->SetFolderData("Pipe", "png");
		IMaterial* waterMaterial = new MaterialPBR(glm::vec4(0.0, 0.66, 0.8, 0.7), 0.0f, 0.0f, 1.33f, true);
		IMaterial* glassMaterial = new MaterialPBR(glm::vec4(1, 1, 1, 0.1), 0.0f, 0.0f, 1.5f, true);
		IMaterial* baseAquariumMaterial = new MaterialPBR(glm::vec4(0.5, 0.5, 0.5, 1.0));
		IMaterial* ballMaterial = new MaterialPBR(glm::vec4(0.0, 1.0, 0.0, 1.0));

		//Create the camera, and add the Movement script and Water affected tag.
		GameObject* camera = new GameObject("Camera", this->root);
		camera->addComponent(new Camera(Camera::Settings::perspective(global.ScreenAspectRatio())));
		camera->addComponent(new MovementScript());
		camera->addComponent(new WaterAffected());
		//change is start position.
		camera->GetTransform()->SetPosition(glm::vec3(0,0,-2));

		//Add a point light to the scene.
		GameObject* light = new GameObject("Light1", this->root);
		light->addComponent(new Light(Light::POINT, glm::vec3(1, 1, 1), 10.0, 1.0));
		light->GetTransform()->SetPosition(glm::vec3(0, 0, 0));

		//Create Moving Sphere with a rigidbody
		/*GameObject* sphere1 = new GameObject("Sphere", this->root);
		sphere1->addComponent(ModelGenerator::UVSphere(ballMaterial, 16,32,0.1));
		sphere1->addComponent(new Displayable());
		sphere1->addComponent(new SphereCollider(0.1));
		Rigidbody * sphereRigid = new Rigidbody();
		sphere1->addComponent(sphereRigid);
		sphereRigid->SetVelocity(glm::vec3(0.1,0,0));
		sphere1->GetTransform()->SetPosition(glm::vec3(-1,0.2,0));

		//Create a 2nd Moving Sphere with a rigidbody
		GameObject* sphere2 = new GameObject("Sphere", this->root);
		sphere2->addComponent(ModelGenerator::UVSphere(ballMaterial, 16,32,0.1));
		sphere2->addComponent(new Displayable());
		sphere2->addComponent(new SphereCollider(0.1));
		Rigidbody * sphereRigid2 = new Rigidbody();
		sphere2->addComponent(sphereRigid2);
		sphereRigid2->SetVelocity(glm::vec3(-0.1,0,0));
		sphere2->GetTransform()->SetPosition(glm::vec3(1,0.2,0));*/

		//Create the aquarium
		GameObject* aquarium = new GameObject("Aquarium", this->root);
		GameObject* bottomAquarium = new GameObject("Aquarium Bottom", aquarium);
		bottomAquarium->addComponent(ModelGenerator::Cube(baseAquariumMaterial, 1, glm::vec3(4, 0.05, 2), glm::vec3(0, -2, 0)));
		bottomAquarium->addComponent(new Displayable(10));

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


		//Create Water of the aquarium, with the water physics.
		GameObject* water = new GameObject("water", aquarium);
		Model* waterModel = ModelGenerator::CubeWater(waterMaterial, 1024, 512, glm::vec3(8, 3, 4));
		water->addComponent(new Displayable(100)); //cutom display priority, to show the water behind the glass.
		water->addComponent(waterModel);
		water->addComponent(new BoundingBoxCollider(waterModel->GetPoints()));
		WaterPhysics* waterP = new WaterPhysics(512,256);
		water->addComponent(waterP);
		water->GetTransform()->SetPosition(glm::vec3(0, 2.55,0));

		//Add a drop
		waterP->AddDrop(glm::vec2(0.5, 0.5), 0.05f, 0.1f);

		//Change the aquarium position.
		aquarium->GetTransform()->SetPosition(glm::vec3(0, -2, 2));

	}

};

#endif
