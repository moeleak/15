#include "Glow.h"
#include "../Settings.h"

Glow::Glow() : context(nullptr) {}

Glow::~Glow() {}

void Glow::SetStencilState(ShaderStencilState_t& s)
{
    if (!context)
        return;

    typedef void(__thiscall* dfn)(void*, ShaderStencilState_t&);
    return Utils::CallVFuncOld<dfn>(context, 127)(context, s);
    
}

void Glow::SetClr(float r, float g, float b, float a)
{
    float clr[4] = { r, g , b, a };
    g_pRenderView->SetColorModulation(clr);
}


void Glow::SceneEnd()
{
    VM_BEGIN("HACKS_GLOW")
    if (!g_Settings.Visuals.Type.bGlows)
        return;
    if (!context)
        return;

    static SpoofedConvar r_drawothermodels("r_drawothermodels"); // i dont know why it doesn't work in members, but this works.

    C_BaseEntity* ent = nullptr;
    auto localTeam = g::pLocalEntity->GetTeam();
    auto entTeam = 0;

    sss.enable = true;
    sss.ref = 1;
    sss.pass = 3;
    sss.fail = 1;
    sss.zfail = 3;
    sss.compare = 8;
    sss.mask1 = 4294967295;
    sss.mask2 = 4294967295;
    
    SetStencilState(sss);
    g_pRenderView->SetBlend(0);
    for (int i = 0; i < g_pGlobalVars->maxClients; i++)
    {
        ent = g_pEntityList->GetClientEntity(i);
        if (!ent || !ent->IsAlive() || ent->IsImmune())
            continue;
        ent->DrawModel(1, 255);
    }

    g_pRenderView->SetBlend(1);
    sss.compare = 6;
    SetStencilState(sss);
    r_drawothermodels.SetValue(2);

    for (int i = 0; i < g_pGlobalVars->maxClients; i++)
    {
        ent = g_pEntityList->GetClientEntity(i);
        if (!ent 
            || !ent->IsAlive() 
            || ent->IsImmune()
            || (g_Settings.Visuals.Filter.bFriends && ent->GetTeam() == localTeam))
            continue;
        

        if (ent->GetTeam() == localTeam)
            SetClr(g_Settings.Color.flGlowTeam[0], g_Settings.Color.flGlowTeam[1], g_Settings.Color.flGlowTeam[2], 255.f);
        else
        {
            SetClr(g_Settings.Color.flGlowEnemy[0], g_Settings.Color.flGlowEnemy[1], g_Settings.Color.flGlowEnemy[2], 255.f);
            entTeam = ent->GetTeam();
        }
        ent->DrawModel(1, 255);
    }

    r_drawothermodels.SetValue(1);



    if (g_Settings.Visuals.Filter.bFriends && entTeam == localTeam)
        SetClr(g_Settings.Color.flGlowTeam[0], g_Settings.Color.flGlowTeam[1], g_Settings.Color.flGlowTeam[2], 255.f);
    else
    {
        if (entTeam == localTeam)
            SetClr(g_Settings.Color.flGlowTeam[0], g_Settings.Color.flGlowTeam[1], g_Settings.Color.flGlowTeam[2], 255.f);
        else
            SetClr(g_Settings.Color.flGlowEnemy[0], g_Settings.Color.flGlowEnemy[1], g_Settings.Color.flGlowEnemy[2], 255.f);
    }
    sss.enable = false;
    SetStencilState(sss);
    VM_END
}
