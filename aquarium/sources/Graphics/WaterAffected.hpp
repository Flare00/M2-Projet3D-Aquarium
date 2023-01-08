#ifndef __WATER_AFFECTED_HPP__
#define __WATER_AFFECTED_HPP__

#include <vector>
#include <Engine/Component/Component.hpp>

/// <summary>
/// Is Gameobject can be water affected ?
/// </summary>
class WaterAffected : public Component
{
	
public:
	bool wasInWater = false;
	/// <summary>
	/// Is Gameobject can be water affected ?
	/// </summary>
	WaterAffected(){}
};

#endif