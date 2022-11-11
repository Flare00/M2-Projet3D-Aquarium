#ifndef __COLOR_SCRIPT_HPP__
#define __COLOR_SCRIPT_HPP__

#include <cmath>
#include <Engine/Component/Script.hpp>
#include <Engine/EngineBehavior.hpp>

class ColorScript : public Script, public EngineBehavior
{
private:
    int color_mode = 0; //0 : R, 1 : G, 2 : B
    double avance = 0.0;

public:
    ColorScript(GameObject* attachment, int color_mode) : Script(attachment) {
        if (color_mode <= 0) {
            this->color_mode = 0;
        }
        else if (color_mode >= 2) {
            this->color_mode = 2;
        }
        else {
            this->color_mode = 1;
        }
    }

    virtual void loop(double deltaTime) {
        this->avance += deltaTime;
        double value = (cos(avance) + 1.0) / 2.0;
        double r = 1.0f, g = 1.0f, b = 1.0f;
        switch (this->color_mode) {
        case 0: r = value;
            break;
        case 1: g = value;
            break;
        case 2: b = value;
            break;
        }
        glClearColor(r, g, b, 0.0f);
    }
};

#endif