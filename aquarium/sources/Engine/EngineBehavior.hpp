#ifndef __GAME_BEHAVIOR_HPP__
#define __GAME_BEHAVIOR_HPP__

class EngineBehavior
{
public:
    EngineBehavior(){}
    bool started = false;
    virtual void start(){
        started = true;
    };
    virtual void loop(double deltaT){}
    virtual void stop(){
        started = false;
    };
};

#endif