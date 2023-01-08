#ifndef __SCRIPT_HPP__
#define __SCRIPT_HPP__

#include <Engine/Component/Component.hpp>
#include <Engine/EngineBehavior.hpp>

/// <summary>
/// A Script class that combine Component and EngineBehavior.
/// </summary>
class Script : public Component, public EngineBehavior
{
};

#endif