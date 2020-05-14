#pragma once

#include "Imgui/imgui.h"

struct Color
{
    int red, green, blue, alpha;

    Color()
    {
        this->red   = 0;
        this->green = 0;
        this->blue  = 0;
        this->alpha = 255;
    }

    Color(int r, int g, int b, int a = 255)
        : red{ r }, green{ g }, blue{ b }, alpha{ a } { }

    Color& operator *=(float coeff)
    {
        this->red   = static_cast<int>(this->red * coeff);
        this->green = static_cast<int>(this->green * coeff);
        this->blue  = static_cast<int>(this->blue * coeff);
        return *this;
    }

    Color& operator /=(float div)
    {
        const float flDiv = 1.f / div;
        *this *= flDiv;
        return *this;
    }

    Color operator *(float coeff) const
    {
        Color color = *this;
        return color *= coeff;
    }

    static Color Black()    { return {0, 0, 0};       }
    static Color Grey()     { return {127, 127, 127}; }
    static Color White()    { return {255, 255, 255}; }
    static Color Red()      { return {255, 0, 0};     }
    static Color Green()    { return { 0, 255, 0 };   }
    static Color Blue()     { return { 0, 0, 255 };   }
    
    static Color FromImColor(const ImColor& clr)
    {
        return Color(
            (int)(clr.Value.x * 255),
            (int)(clr.Value.y * 255),
            (int)(clr.Value.z * 255),
            (int)(clr.Value.w * 255)
        );
    }

    static Color FromArray(const float* clr)
    {
        return Color(
            static_cast<int>(clr[0] * 255),
            static_cast<int>(clr[1] * 255),
            static_cast<int>(clr[2] * 255),
            (int)255
        );
    }

    static ImColor ToImColor(Color clr)
    {
        return ImColor(
            clr.red / 255.f,
            clr.green / 255.f,
            clr.blue / 255.f,
            clr.alpha / 255.f
        );
    }
};