#ifndef __WATER_AFFECTED_HPP__
#define __WATER_AFFECTED_HPP__

#include <vector>
#include <Engine/Component/Component.hpp>

class WaterAffected : public Component
{
	
public:
	bool wasInWater = false;
	WaterAffected(){}
};

#endif