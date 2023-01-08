#ifndef __WATER_PHYSICS_MATERIAL__
#define __WATER_PHYSICS_MATERIAL__

#include <Physics/PhysicsMaterial/IPhysicsMaterial.hpp>
#include <Graphics/Texture/TextureFloatingPoint.hpp>

/// <summary>
/// Physics Material for Water, Inherit IPhysicsMaterial.
/// </summary>
class WaterPhysicsMaterial : public IPhysicsMaterial{
protected:
	TextureFloatingPoint* water;
public:
	/// <summary>
	/// Water Physics Material for GPU.
	/// </summary>
	/// <param name="width">The width of the Texture.</param>
	/// <param name="height">The height of the Texture.</param>
	WaterPhysicsMaterial(int width = 512, int height = 512) {
		//IPhysicsMaterial::settedShaders
		this->water = new TextureFloatingPoint(width, height);
	}

	/// <summary>
	/// Set the Data to the GPU.
	/// </summary>
	virtual void SetDataGPU() override {
	}
};

#endif // !__WATER_PHYSICS_MATERIAL__
