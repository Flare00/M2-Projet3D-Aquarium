#ifndef __WATER_PHYSICS_HPP__
#define __WATER_PHYSICS_HPP__

#include <Graphics/Texture/TextureFloatingPoint.hpp>

class WaterPhysics {
protected :
	TextureFloatingPoint* water;
public :
	WaterPhysics() {
		this->water = new TextureFloatingPoint();
	}

	TextureFloatingPoint* getWaterPhysicTexture() {
		return this->water;
	}
};

#endif
