#ifndef __DISPLAYABLE_HPP__
#define __DISPLAYABLE_HPP__

#include <algorithm>
#include <vector>
#include <Engine/Component/Transformation.hpp>
#include <Engine/Component/Component.hpp>
#include <Engine/GameObject.hpp>


class Displayable : public Component
{
protected:
	bool alwaysDraw = false;
	int priority = 0;
public:
	Displayable(int priority = 0, bool alwaysDraw = false)
	{
		this->alwaysDraw = alwaysDraw;
		this->priority = priority;
	}

	Transformation* GetTransformation() {
		return this->attachment->GetTransform();
	}

	bool IsAlwaysDraw() {
		return this->alwaysDraw;
	}

	int GetPriority() const {
		return priority;
	}

	struct ComparePriority
	{
		inline bool operator() (const Displayable * d1, const Displayable * d2) 
		{ 
			return d1->GetPriority() > d2->GetPriority();
		}
	};

	static std::vector<Displayable*> SortByPriority(std::vector<Displayable*> displayable) {
		std::sort(displayable.begin(), displayable.end(), ComparePriority());
		return displayable;
	}

};

#endif
