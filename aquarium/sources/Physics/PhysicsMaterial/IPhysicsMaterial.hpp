#ifndef __I_PHYSICS_MATERIAL_HPP__
#define __I_PHYSICS_MATERIAL_HPP__

#include <vector>
#include <string>
#include <Engine/Shader.hpp>
#include <Engine/Component/Component.hpp>

/// <summary>
/// Physics Material for GPU computing, with shaders.
/// </summary>
class IPhysicsMaterial : public Component 
{
protected:
	std::vector<Shader*> shaders; // need to be stocked in the compute order.
public:
	static SettedShaders settedShaders;

	/// <summary>
	/// Physics material for GPU computing.
	/// </summary>
	IPhysicsMaterial() {
	}

	virtual void SetDataGPU() {
	}

	std::vector<Shader*> GetShaders() {
		return this->shaders
	}



	virtual void SetDataGPU() {
	}
};

#endif // !__I_PHYSICS_MATERIAL_HPP__
