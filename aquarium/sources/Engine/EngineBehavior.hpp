#ifndef __ENGINE_BEHAVIOR_HPP__
#define __ENGINE_BEHAVIOR_HPP__

/// <summary>
/// Engine Behavior object. Used by engine to execute code that need to be called each frame.
/// </summary>
class EngineBehavior
{
public:

    bool started = false;

    /// <summary>
    /// Engine Behavior object. Used by engine to execute code that need to be called each frame.
    /// </summary>
    EngineBehavior(){}

    /// <summary>
    /// The Start element of the engine behavior.
    /// </summary>
    virtual void start(){
        started = true;
    };

    /// <summary>
    /// The engine behavior loop object.
    /// </summary>
    /// <param name="deltaT"></param>
    virtual void loop(double deltaT){}

    /// <summary>
    /// The Stop element of the engine behavior.
    /// </summary>
    virtual void stop(){
        started = false;
    };
};

#endif