#include "NightMode.h"

void NightMode::SceneEnd()
{
    VM_BEGIN("HACKS_NIGHTMODE")

    static bool bPerformed = true;
    static bool bLastSetting;

    static SpoofedConvar r_DrawSpecificStaticProp("r_DrawSpecificStaticProp");

    if (!bPerformed)
    {
        for (MaterialHandle_t i = g_pMaterialSystem->FirstMaterial(); i != g_pMaterialSystem->InvalidMaterial(); i = g_pMaterialSystem->NextMaterial(i))
        {
            auto pMat = g_pMaterialSystem->GetMaterial(i);

            if (!pMat || pMat->IsErrorMaterial())
                continue;

            if (strstr(pMat->GetTextureGroupName(), XORSTR("World")))
            {
                if (bLastSetting)
                    pMat->ColorModulate(0.15f, 0.15f, 0.15f);
                else
                    pMat->ColorModulate(1.f, 1.f, 1.f);
            }
            else if (strstr(pMat->GetTextureGroupName(), XORSTR("Static")))
            {
                if (bLastSetting)
                {
                    r_DrawSpecificStaticProp.SetValue(0);
                    pMat->ColorModulate(0.30f, 0.30f, 0.30f);
                }
                else
                {
                    r_DrawSpecificStaticProp.SetValue(1);
                    pMat->ColorModulate(1.f, 1.f, 1.f);
                }
            }
            else if (strstr(pMat->GetTextureGroupName(), XORSTR("SkyBox")))
            {

                //pMaterial->ColorModulate(0.09f, 0.11f, 0.13f);
                //pMaterial->ColorModulate(0.29f, 0.31f, 0.33f);
                //pMaterial->ColorModulate(0.18f, 0.18f, 0.18f);
                if (bLastSetting)
                    pMat->ColorModulate(0.23f, 0.22f, 0.22f);
                else
                    pMat->ColorModulate(1.f, 1.f, 1.f);
            }
        }
        bPerformed = true;
    }
    if (bLastSetting != g_Settings.Visuals.Others.bNightMode)
    {
        bLastSetting = g_Settings.Visuals.Others.bNightMode;
        bPerformed = false;
    }
    if (!g_Settings.Cheat.bCheatActive)
    {
        bLastSetting = false;
        bPerformed = false;
    }
    VM_END
}