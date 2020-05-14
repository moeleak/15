#pragma once

#include "../SDK/SDK.h"
#include "../Utils/GlobalVars.h"
#include "../Utils/SpoofedConvar.h"
#include "../Utils/Utils.h"

class Glow
{
private:
    ShaderStencilState_t sss;
    RenderableInstance_t rt;

    void SetStencilState(ShaderStencilState_t& s);
    void SetClr(float r, float g, float b, float a);

public:
    void* context;

public:
    Glow();
    ~Glow();
public:
    // Hooks
    void SceneEnd();
};

extern Glow g_Glow;
