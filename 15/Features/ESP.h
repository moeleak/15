#pragma once
#include "../Utils/DrawManager.h"
#include "../Utils/GlobalVars.h"
#include "../Settings.h"
#include "../SDK/SDK.h"

class ESP
{
public:
    void Render();

private:

    // R e n d e r
    void RenderSkeleton(C_BaseEntity* pEnt);
    void RenderBox(float, float, float, float, C_BaseEntity* pEnt);
    void RenderName(float, float, float, float, C_BaseEntity* pEnt, int);
    void RenderInfo(float, float, float, float, C_BaseEntity* pEnt);
    void RenderHealth(float, float, float, float, C_BaseEntity* pEnt);
    void RenderArmor(float, float, float, float, C_BaseEntity* pEnt);

    // M i s c 
    bool ESPEnabled();
};