#ifndef __DISPLAYABLE_HPP__
#define __DISPLAYABLE_HPP__

#include <algorithm>
#include <vector>
#include <Engine/Component/Transformation.hpp>
#include <Engine/Component/Component.hpp>
#include <Engine/GameObject.hpp>

/// <summary>
/// A Displayable Component, with priority, to draw elements on screen.
/// </summary>
class Displayable : public Component
{
protected:
	bool alwaysDraw = false;
	int priority = 0; // the draw priority of the object, the higher, the later it is draw.
public:
	/// <summary>
	/// Create a displayable component
	/// </summary>
	/// <param name="priority">The priority of the elements</param>
	/// <param name="alwaysDraw">if the object need to be always draw</param>
	Displayable(int priority = 0, bool alwaysDraw = false)
	{
		this->alwaysDraw = alwaysDraw;
		this->priority = priority;
	}

	/// <summary>
	/// return the transformation component
	/// </summary>
	/// <returns>The transformation</returns>
	Transformation* GetTransformation() {
		return this->attachment->GetTransform();
	}

	/// <summary>
	/// Return if is always draw
	/// </summary>
	/// <returns>is always draw</returns>
	bool IsAlwaysDraw() {
		return this->alwaysDraw;
	}

	/// <summary>
	/// Return the priority of the object. 
	/// </summary>
	/// <returns>The draw priority</returns>
	int GetPriority() const {
		return priority;
	}

	/// <summary>
	/// Compare structure for displayable sorting.
	/// </summary>
	struct ComparePriority
	{
		/// <summary>
		/// Replace the () operator to create a comparator for the sort.
		/// </summary>
		/// <param name="d1"></param>
		/// <param name="d2"></param>
		/// <returns></returns>
		inline bool operator() (const Displayable * d1, const Displayable * d2) 
		{ 
			return d1->GetPriority() > d2->GetPriority();
		}
	};

	/// <summary>
	/// Sort a vector of Displayable by priority.
	/// </summary>
	/// <param name="displayable">List of displayable reference</param>
	/// <returns>Sorted displayable reference list.</returns>
	static std::vector<Displayable*> SortByPriority(std::vector<Displayable*> displayable) {
		std::sort(displayable.begin(), displayable.end(), ComparePriority());
		return displayable;
	}

};

#endif
