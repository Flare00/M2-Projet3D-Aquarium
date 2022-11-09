#include "Scene.hpp"

class SceneAquarium : public Scene
{
public:
    Scene1(std::string id) : base(id){
        this->root = new GameObject("Scene");
        this->root->addComponent(new ColorScript(this, 2));
    }
};