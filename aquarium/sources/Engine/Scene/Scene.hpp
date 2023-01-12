#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <vector>
#include <string>

#include <Engine/EngineBehavior.hpp>
#include <Engine/GameObject.hpp>
#include <Engine/Shader.hpp>
#include <Engine/Component/Script.hpp>
#include <Script/ColorScript.hpp>
#include <Graphics/Material/IMaterial.hpp>
#include <Graphics/Material/MaterialPBR.hpp>

/// <summary>
/// A scene for the engine.
/// </summary>
class Scene : public EngineBehavior
{
public:
	//The root gameobject.
	GameObject* root = NULL;
	std::vector<Camera*> renderCamera;
	int activeCamera = 0;

	//The id of the scene.
	std::string id;

	/// <summary>
	/// A Scene Object for the engine
	/// </summary>
	/// <param name="id">The id of the scene.</param>
	Scene(std::string id) {
		this->id = id;
	}
	~Scene() {}

	/// <summary>
	/// Start the scene, and all the scripts linked to it.
	/// </summary>
	virtual void start() {
		//start all scripts
		if (root != NULL) {
			std::vector<Script*> engineBehaviors = root->getComponentsByTypeRecursive<Script>();

			for (size_t i = 0, max = engineBehaviors.size(); i < max; i++) {
				if (!engineBehaviors[i]->started) {
					engineBehaviors[i]->start();
				}
			}
		}
	}

	/// <summary>
	/// Loop the scene, and all the scripts linked to it.
	/// </summary>
	/// <param name="deltaT">The time since the last frame</param>
	virtual void loop(double deltaT) {
		if (root != NULL) {
			//Loop all scripts and start the non started one.

			std::vector<EngineBehavior*> engineBehavior = root->getComponentsByTypeRecursive<EngineBehavior>();
			
			for (size_t i = 0, max = engineBehavior.size(); i < max; i++) {
				if (!engineBehavior[i]->started) {
					engineBehavior[i]->start();
				}
				engineBehavior[i]->loop(deltaT);
			}
		}

	}

	/// <summary>
	/// Stop the scene, and all the scripts linked to it.
	/// </summary>
	virtual void stop() {
		if (root != NULL) {
			//stop all the started scripts
			std::vector<EngineBehavior*> gameBehaviors = root->getComponentsByTypeRecursive<EngineBehavior>();
			for (size_t i = 0, max = gameBehaviors.size(); i < max; i++) {
				if (gameBehaviors[i]->started) {
					gameBehaviors[i]->stop();
				}
			}
		}
	}

	/// <summary>
	/// Return the Root gameobject.
	/// </summary>
	/// <returns>The Root gameobject</returns>
	GameObject* GetRoot() {
		return this->root;
	}

	/// <summary>
	/// Reload the setted shader of this scene.
	/// </summary>
	void ReloadShaders() {
		for (SettedShaders::SettedShader s : settedStdShaders.GetSettedShaders()) {
			s.shader->Reload();
		}
	}
};

#endif