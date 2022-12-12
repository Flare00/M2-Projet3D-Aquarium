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
public:
	Displayable(bool alwaysDraw = false)
	{
		this->alwaysDraw = alwaysDraw;
	}

	Transformation* GetTransformation() {
		return this->attachment->GetTransform();
	}

	bool IsAlwaysDraw() {
		return this->alwaysDraw;
	}
};

#endif
