#ifndef __WATER_PHYSICS_MATERIAL__
#define __WATER_PHYSICS_MATERIAL__

#include <Physics/PhysicsMaterial/IPhysicsMaterial.hpp>
#include <Graphics/Texture/TextureFloatingPoint.hpp>

class WaterPhysicsMaterial : public IPhysicsMaterial{
protected:
	TextureFloatingPoint* water;
public:
	WaterPhysicsMaterial(int width = 512, int height = 512) {
		//IPhysicsMaterial::settedShaders
		this->water = new TextureFloatingPoint(width, height);
	}

	virtual void SetDataGPU() override {
	}
};

#endif // !__WATER_PHYSICS_MATERIAL__
