#include "Chams.h"
#include "Backtrack.h"

Chams::Chams() :
    materialRegular(nullptr), materialRegularIgnoreZ(nullptr),
    materialFlatIgnoreZ(nullptr), materialFlat(nullptr), materialGlossIgnoreZ(nullptr), materialGloss(nullptr)
{
    std::ofstream("csgo\\materials\\simple_regular.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";

    std::ofstream("csgo\\materials\\simple_ignorez.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";

    std::ofstream("csgo\\materials\\simple_flat.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";

    std::ofstream("csgo\\materials\\simple_flat_ignorez.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";


    std::ofstream("csgo\\materials\\gloss.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "models/inventory_items/trophy_majors/gloss"
  "$ignorez"      "0"
  "$envmap" "env_cubemap"
  "$normalmapalphaenvmapmask" 1
  "$envmapcontrast" 1
}
)#";

    std::ofstream("csgo\\materials\\gloss_ignorez.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "models/inventory_items/trophy_majors/gloss"
  "$ignorez"      "1"
  "$envmap" "env_cubemap"
  "$normalmapalphaenvmapmask" 1
  "$envmapcontrast" 1
}
)#";

    


}

Chams::~Chams()
{
    std::remove("csgo\\materials\\simple_regular.vmt");
    std::remove("csgo\\materials\\simple_ignorez.vmt");
    std::remove("csgo\\materials\\simple_flat.vmt");
    std::remove("csgo\\materials\\simple_flat_ignorez.vmt");
    std::remove("csgo\\materials\\gloss.vmt");
    std::remove("csgo\\materials\\gloss_ignorez.vmt");

}

void Chams::OverrideMaterial(bool ignoreZ, int nType , const float* rgba)
{
    IMaterial* material = nullptr;

    if (nType == 1)  // Flat
    {
        if (ignoreZ)
            material = materialFlatIgnoreZ;
        else
            material = materialFlat;
    }
    else if (nType == 2)
    {
        if (ignoreZ)
            material = materialFlatIgnoreZ;
        else
            material = materialFlat;
    }
    else if (nType == 3) // Gloss
    {
        if (ignoreZ)
            material = materialGlossIgnoreZ;
        else
            material = materialGloss;
    }
    else
    {
        if (ignoreZ)
            material = materialRegularIgnoreZ;
        else
            material = materialRegular;
    }


    if (nType == 2) 
        material->AlphaModulate(0.45f);
    else
        material->AlphaModulate(1.f);

    material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, 0);
    material->ColorModulate(rgba[0], rgba[1], rgba[2]);

    g_pMdlRender->ForcedMaterialOverride(material);
}

void Chams::DrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
    if (!g_Settings.Visuals.Chams.bEnabled)
        return;

    if (!g::pLocalEntity || !g_pEngine->IsInGame() || !g_pEngine->IsConnected())
        return;

    static auto oDrawModelExecute = g_Hooks.pMatRenderHook->GetOriginal<Hooks::DrawModelExecute_t>(vtable_indexes::drawmodelexecute);

    
    materialRegular = g_pMaterialSystem->FindMaterial("simple_regular", TEXTURE_GROUP_MODEL);
    materialRegularIgnoreZ = g_pMaterialSystem->FindMaterial("simple_ignorez", TEXTURE_GROUP_MODEL);
    materialFlatIgnoreZ = g_pMaterialSystem->FindMaterial("simple_flat_ignorez", TEXTURE_GROUP_MODEL);
    materialFlat = g_pMaterialSystem->FindMaterial("simple_flat", TEXTURE_GROUP_MODEL);
    materialGloss = g_pMaterialSystem->FindMaterial("gloss", TEXTURE_GROUP_MODEL);
    materialGlossIgnoreZ = g_pMaterialSystem->FindMaterial("gloss_ignorez", TEXTURE_GROUP_MODEL);

    const auto mdl = pInfo.pModel;

    bool is_arm = strstr(mdl->szName, "arms") != nullptr;
    bool is_player = strstr(mdl->szName, "models/player") != nullptr;
    bool is_sleeve = strstr(mdl->szName, "sleeve") != nullptr;
    //bool is_weapon = strstr(mdl->szName, "weapons/v_")  != nullptr;

    if (is_player && g_Settings.Visuals.Chams.bEnabled)
    {
        auto ent = g_pEntityList->GetClientEntity(pInfo.entity_index);

        if (ent && ent != g::pLocalEntity && ent->IsAlive())
        {
            const auto enemy = ent->GetTeam() != g::pLocalEntity->GetTeam();
            if (!enemy && g_Settings.Visuals.Filter.bFriends)
                return;

            const auto clr_front = enemy ? g_Settings.Color.flChamsVisibleEnemy : g_Settings.Color.flChamsVisibleTeam;
            const auto clr_back = enemy ? g_Settings.Color.flChamsOccludedEnemy : g_Settings.Color.flChamsOccludedEnemy;

            if (g_Settings.Visuals.Chams.bPlayerIgnorez)
            {
                OverrideMaterial(true, g_Settings.Visuals.Chams.nPlayerType, clr_back);
                oDrawModelExecute(g_pMdlRender, ctx, state, pInfo, pCustomBoneToWorld);
                OverrideMaterial(false,
                    g_Settings.Visuals.Chams.nPlayerType,
                    clr_front);
            }
            else
            {
                OverrideMaterial(
                    false, g_Settings.Visuals.Chams.nPlayerType,
                    clr_front);
            }
        }
        if (ent == g::pLocalEntity && g::pLocalEntity->IsAlive() && g_Settings.Visuals.Chams.bLocalPlayer && g_pInput->m_fCameraInThirdPerson)
        {
            OverrideMaterial(false, g_Settings.Visuals.Chams.nLocalPlayerType, g_Settings.Color.flChamsLocalPlayer);
        }
    }

    if (is_arm && g_Settings.Visuals.Chams.bArms)
    {
        if (!g::pLocalEntity->IsAlive())
            return;
        auto material = g_pMaterialSystem->FindMaterial(mdl->szName, TEXTURE_GROUP_MODEL);
        if (!material)
            return;
        if (g_Settings.Visuals.Chams.bPlayerIgnorez)
        {
            OverrideMaterial(true, g_Settings.Visuals.Chams.nArmsType, g_Settings.Color.flChamsVisibleEnemy);
            oDrawModelExecute(g_pMdlRender, ctx, state, pInfo, pCustomBoneToWorld);
            OverrideMaterial(false,
                g_Settings.Visuals.Chams.nArmsType,
                g_Settings.Color.flChamsVisibleTeam);
        }
        else
        {
            OverrideMaterial(
                false, g_Settings.Visuals.Chams.nArmsType,
                g_Settings.Color.flChamsVisibleTeam);
        }
    }


}
