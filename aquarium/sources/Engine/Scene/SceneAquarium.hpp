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
#include <Graphics/ReceiveCaustics.hpp>
#include <Physics/GLPhysics/WaterPhysics.hpp>
#include <Physics/Physics/Rigidbody.hpp>
#include <Physics/Collider/SphereCollider.hpp>
#include <IA/FishBank.hpp>
#include <IA/FishRandom.hpp>
#include <Engine/RaycastObject.hpp>

/// <summary>
/// The aquarium Scene
/// </summary>
class SceneAquarium : public Scene
{
private :

public:
	SceneAquarium(std::string id) : Scene(id) {
		//Create the root object.
		this->root = new GameObject("Scene");

		//Create all the PBR materials needed.

		IMaterial* pipeMaterial = (new MaterialPBR())->SetFolderData("Pipe", "png");

		IMaterial* waterMaterial = new MaterialPBR(glm::vec4(0.0, 0.66, 0.8, 0.7), 0.5f, 0.0f, 1.33f, true);
		IMaterial* glassMaterial = new MaterialPBR(glm::vec4(1, 1, 1, 0.1), 0.0f, 0.0f, 1.5f, true);
		IMaterial* baseAquariumMaterial = new MaterialPBR(glm::vec4(1, 0, 1, 1.0));
		IMaterial* ballMaterial = new MaterialPBR(glm::vec4(0.0, 1.0, 0.0, 1.0));

		IMaterial* fishBankMaterial = new MaterialPBR(glm::vec4(1), 0.0f, 0.0f, 1.0f, true, textureFolder + "Fish/color.png");
		fishBankMaterial->SetShader("Fish/fish");

		//Create the camera, and add the Movement script and Water affected tag.
		GameObject* camera = new GameObject("Camera", this->root);
		Camera* renCam = new Camera(Camera::Settings::perspective(global.ScreenAspectRatio()));
		camera->addComponent(renCam);
		camera->addComponent(new WaterAffected());
		//change is start position.
		camera->GetTransform()->SetPosition(glm::vec3(0,0,-2));
		this->renderCamera.push_back(renCam);

		//Add a point light to the scene.
		GameObject* light = new GameObject("Light1", this->root);
		light->addComponent(new Light(Light::POINT, glm::vec3(1, 1, 1), 1000.0, 1.0));
		light->GetTransform()->SetPosition(glm::vec3(0, 0, 2));

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
		water->addComponent(new Displayable(10)); //cutom display priority, to show the water behind the glass.
		water->addComponent(waterModel);
		water->addComponent(new BoundingBoxCollider(waterModel->GetPoints()));
		WaterPhysics* waterP = new WaterPhysics(512,256, glm::vec2(8,4));
		water->addComponent(waterP);
		water->addComponent(new RaycastObject());
		water->GetTransform()->SetPosition(glm::vec3(0, 2.55,0));

		//Add a drop
		//waterP->AddDrop(glm::vec2(0.5, 0.5), 0.05f, 5.0f);

		//Generate the fish bank
		Spline* fishBankSpline = new Spline(std::vector<glm::vec3>{glm::vec3(-2,-1,0), glm::vec3(3,-1.5,-1), glm::vec3(1,0.2,1), glm::vec3(0,0,0)});
		GameObject* fishBank = new GameObject("Fish Bank", aquarium);
		ModelInstanced* fish= ModelGenerator::QuadInstanced(fishBankMaterial, 2,2, 0.25f, 0.25f);
		fishBank->addComponent(new Displayable(100));
		fishBank->addComponent(fish);
		fishBank->addComponent(new FishBank(fish, fishBankSpline, 0.1, 2));

		//Generate fish random

		GameObject * fishRandom = new GameObject("Fish Random", aquarium);
		ModelInstanced* fishR = ModelGenerator::QuadInstanced(fishBankMaterial, 2, 2, 0.25f, 0.25f);
		fishRandom->addComponent(new Displayable(100));
		fishRandom->addComponent(fishR);
		fishRandom->addComponent(new FishRandom(fishR, glm::vec3(7,2.5,3.5), glm::vec3(0,-0.5,0)));

		bottomAquarium->addComponent(new ReceiveCaustics());
		//Change the aquarium position.
		aquarium->GetTransform()->SetPosition(glm::vec3(0, -2, 2));

		camera->addComponent(new MovementScript({fishBank, fishRandom}));

	}




};

#endif
