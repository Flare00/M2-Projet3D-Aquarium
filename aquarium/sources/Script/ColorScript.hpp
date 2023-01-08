#ifndef __COLOR_SCRIPT_HPP__
#define __COLOR_SCRIPT_HPP__

#include <cmath>
#include <Engine/Component/Script.hpp>

/// <summary>
/// Simple script to make the clear color change. (not used anymore)
/// </summary>
class ColorScript : public Script
{
private:
    int color_mode = 0; //0 : R, 1 : G, 2 : B
    double avance = 0.0;

public:
    /// <summary>
    /// Simple script to make the clear color change. not used anymore.
    /// </summary>
    /// <param name="color_mode"></param>
    ColorScript(int color_mode) {
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

    void loop(double deltaTime) override {
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