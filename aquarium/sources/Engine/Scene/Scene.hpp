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

class Scene : public EngineBehavior
{
protected:
	GameObject* root = NULL;
	int pbrShowMode = 0;
public:
	std::string id;

	Scene(std::string id) {
		this->id = id;
	}
	~Scene() {}

	//EngineBehavior
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

	GameObject* GetRoot() {
		return this->root;
	}

	void ReloadShaders() {
		for (SettedShaders::SettedShader s : settedStdShaders.GetSettedShaders()) {
			s.shader->Reload();
		}
	}

};

#endif