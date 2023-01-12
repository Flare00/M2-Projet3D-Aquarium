#ifndef __RECEIVE_CAUSTICS_HPP__
#define __RECEIVE_CAUSTICS_HPP__

#include <vector>

#include <Engine/Component/Component.hpp>
//#include <Physics/GLPhysics/WaterPhysics.hpp>

/// <summary>
/// Is model receiving caustics ?
/// </summary>
class ReceiveCaustics : public Component
{
protected :
//	WaterPhysics* water;
public :
	/// <summary>
	/// Create a ReceiveCaustics object, with the water Physics reference
	/// </summary>
	/// <param name="water">the water Physics reference</param>
	/*ReceiveCaustics(WaterPhysics* water) {
		//this->water = water;
	}

	/// <summary>
	/// Return the water physics
	/// </summary>
	/// <returns>the water physics</returns>
	WaterPhysics* GetWaterPhysics() {
		return water;
	}*/
};

#endif