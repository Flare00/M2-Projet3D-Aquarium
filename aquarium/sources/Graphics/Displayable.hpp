#ifndef __DISPLAYABLE_HPP__
#define __DISPLAYABLE_HPP__

#include <vector>
#include <Engine/Component/Transformation.hpp>
#include <Engine/Component/Component.hpp>
#include <Engine/GameObject.hpp>

class Displayable : public Component
{
protected:
	bool alwaysDraw = false;
	GameObject* attachment;
	Transformation* transformation = nullptr;
public:
	Displayable(GameObject* attachement, bool alwaysDraw = false)
	{
		this->attachment = attachement;
		this->alwaysDraw = alwaysDraw;
		UpdateTransformation();
	}

	void UpdateTransformation() {
		if (this->attachment != nullptr)
			this->transformation = this->attachment->getFirstComponentByType<Transformation>();
	}

	GameObject* GetGameObject() {
		return attachment;
	}

	Transformation* GetTransformation() {
		return this->transformation;
	}

	bool IsAlwaysDraw() {
		return this->alwaysDraw;
	}
};

#endif
