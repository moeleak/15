#include "ESP.h"
#include "../Utils/Utils.h"
#include "../Utils/Xorstr.h"

bool ESP::ESPEnabled()
{
    return 
        (  g_Settings.Visuals.Type.bShowSkeleton
        || g_Settings.Visuals.Type.bShowBoxes
        || g_Settings.Visuals.Type.bShowHealth
        || g_Settings.Visuals.Type.bShowIfIsScoped
        || g_Settings.Visuals.Type.bShowMoneny
        || g_Settings.Visuals.Type.bShowNames
        || g_Settings.Visuals.Type.bShowWeapons
        || g_Settings.Visuals.Type.bShowArmor
        );
}

void ESP::RenderSkeleton(C_BaseEntity* pEnt)
{
    VM_BEGIN("HACKS_ESP_RENDER_SKELETON")
    if (!pEnt)
        return;

    studiohdr_t* pStudioModel = g_pModelInfo->GetStudioModel(pEnt->GetModel());
    if (!pStudioModel)
        return;

    Vector vParent, vChild, sParent, sChild;

    for (int j = 0; j < pStudioModel->numbones; j++)
    {
        mstudiobone_t* pBone = pStudioModel->GetBone(j);

        if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
        {
            vChild = pEnt->GetBonePos(j);
            vParent = pEnt->GetBonePos(pBone->parent);

            Vector vBreastBone;
            Vector vUpperDirection = pEnt->GetBonePos(7) - pEnt->GetBonePos(6);
            vBreastBone = pEnt->GetBonePos(6) + vUpperDirection / 2;
            Vector vDeltaChild = vChild - vBreastBone;
            Vector vDeltaParent = vParent - vBreastBone;

            if (vDeltaParent.Length() < 9 && vDeltaChild.Length() < 9)
                vParent = vBreastBone;

            if (j == 5)
                vChild = vBreastBone;

            if (abs(vDeltaChild.z) < 5 && (vDeltaParent.Length() < 5 && vDeltaChild.Length() < 5) || j == 6)
                continue;

            if (Utils::WorldToScreen(vParent, sParent) && Utils::WorldToScreen(vChild, sChild))
            {
                g_Render.Line(sParent[0], sParent[1], sChild[0], sChild[1], 
                    pEnt->GetTeam() == g::pLocalEntity->GetTeam() ?
                    Color::FromArray(g_Settings.Color.flSkeletonTeam) :
                    Color::FromArray(g_Settings.Color.flSkeletonEnemy)
                );
            }
        }

    }
    VM_END
}

void ESP::RenderBox(float x, float y, float w, float h, C_BaseEntity* pEnt)
{
    VM_BEGIN("HACKS_ESP_RENDER_BOX")
    if (!pEnt)
        return;

    /* Draw rect around the entity */
    g_Render.Rect(x - w, y, x + w, y + h,
        pEnt->GetTeam() == g::pLocalEntity->GetTeam() ?
        Color::FromArray(g_Settings.Color.flBoxTeam) :
        Color::FromArray(g_Settings.Color.flBoxEnemy)
    );
    
    /* Draw rect outline */
    g_Render.Rect(x - w - 1, y - 1, x + w + 1, y + h + 1, Color::Black());
    g_Render.Rect(x - w + 1, y + 1, x + w - 1, y + h - 1, Color::Black());
    VM_END
}

void ESP::RenderName(float x, float y, float w, float h, C_BaseEntity* pEnt, int iterator)
{
    VM_BEGIN("HACKS_ESP_RENDER_NAME")
    if (!pEnt)
        return;

    PlayerInfo_t pInfo;
    g_pEngine->GetPlayerInfo(iterator, &pInfo);

    g_Render.String(x, y - 16,
        CD3DFONT_CENTERED_X | CD3DFONT_DROPSHADOW,
        pEnt->GetTeam() == g::pLocalEntity->GetTeam() ?
        Color::FromArray(g_Settings.Color.flTextTeam) :
        Color::FromArray(g_Settings.Color.flTextEnemy),
        g_Fonts.pFontVerdana6.get(), pInfo.szName);
    VM_END
}

void ESP::RenderInfo(float x, float y, float w, float h, C_BaseEntity* pEnt)
{
    VM_BEGIN("HACKS_ESP_RENDER_INFO")
    if (!pEnt)
        return;

    //std::string strWeaponName = pEnt->GetActiveWeapon()->GetName().erase(0, 7);
    std::string strWeaponName = g_pWeaponSystem->GetCSWpnData(pEnt->GetActiveWeapon()->GetItemDefinitionIndex())->szWeaponName;

    const auto stringToUpper = [](std::string strToConv) -> std::string
    {
        std::string tmp{};
        for (std::string::iterator p = strToConv.begin(); strToConv.end() != p; ++p)
        {
            *p = toupper(*p);
            tmp.push_back(*p);
        }

        return tmp;
    };

    int i = 0;


    if (g_Settings.Visuals.Type.bShowHealth)
        g_Render.String(x + w + 4, y + 2 + (8 * i++),
            CD3DFONT_DROPSHADOW,
            pEnt->GetTeam() == g::pLocalEntity->GetTeam() ?
            Color::FromArray(g_Settings.Color.flTextTeam) :
            Color::FromArray(g_Settings.Color.flTextEnemy),
            g_Fonts.pFontVerdana6.get(), std::string("HP:" + std::to_string(pEnt->GetHealth())).c_str());

    if (g_Settings.Visuals.Type.bShowArmor)
        g_Render.String(x + w + 4, y + 2 + (8 * i++),
            CD3DFONT_DROPSHADOW,
            pEnt->GetTeam() == g::pLocalEntity->GetTeam() ?
            Color::FromArray(g_Settings.Color.flTextTeam) :
            Color::FromArray(g_Settings.Color.flTextEnemy),
            g_Fonts.pFontVerdana6.get(), std::string("Armor:" + std::to_string(pEnt->GetArmor())).c_str());


    if (g_Settings.Visuals.Type.bShowWeapons && pEnt->GetActiveWeapon())
        g_Render.String(x + w + 4, y + 2 + (8 * i++),
            CD3DFONT_DROPSHADOW,
            pEnt->GetTeam() == g::pLocalEntity->GetTeam() ?
            Color::FromArray(g_Settings.Color.flTextTeam) :
            Color::FromArray(g_Settings.Color.flTextEnemy),
            g_Fonts.pFontVerdana6.get(), stringToUpper(strWeaponName.erase(0, 7)).c_str());

    if (g_Settings.Visuals.Type.bShowMoneny)
        g_Render.String(x + w + 4, y + 2 + (8 * i++),
            CD3DFONT_DROPSHADOW,
            pEnt->GetTeam() == g::pLocalEntity->GetTeam() ?
            Color::FromArray(g_Settings.Color.flTextTeam) :
            Color::FromArray(g_Settings.Color.flTextEnemy),
            g_Fonts.pFontVerdana6.get(), std::string(XORSTR("$") + std::to_string(pEnt->GetMoney())).c_str());

    if (g_Settings.Visuals.Type.bShowIfIsScoped)
        g_Render.String(x + w + 4, y + 2 + (8 * i++),
            CD3DFONT_DROPSHADOW,
            pEnt->GetTeam() == g::pLocalEntity->GetTeam() ? 
            Color::FromArray(g_Settings.Color.flTextTeam) :
            Color::FromArray(g_Settings.Color.flTextEnemy),
            g_Fonts.pFontVerdana6.get(), pEnt->IsScoped() ? XORSTR("SCOPED") : "");
    VM_END
}

void ESP::RenderHealth(float x, float y, float w, float h, C_BaseEntity* pEnt)
{
    VM_BEGIN("HACKS_ESP_RENDER_HP")
    if (!pEnt)
        return;

    auto hp = pEnt->GetHealth();
    if (hp > 100)
        hp = 100;
    float pos = h - (float)((hp * h) / 100);

    g_Render.RectFilled(x - w - 2.f, y + pos, x - w - 4.f, y + h, Color(255 - (int)(hp * 2.55f), (int)(hp * 2.55f), 0));
    g_Render.Rect(x - w - 2.f, y, x - w - 4.f, y + h, Color::Black());
    VM_END
}

void ESP::RenderArmor(float x, float y, float w, float h, C_BaseEntity* pEnt)
{
    VM_BEGIN("HACKS_ESP_RENDER_ARMOR")
    if (!pEnt)
        return;

    auto armor = pEnt->GetArmor();

    float pos = h - (float)((armor * h) / 100);
    
    g_Render.RectFilled(x - w - 6.f, y + pos, x - w - 8.f, y + h, Color(26, 104, 173));
    g_Render.Rect(x - w - 6.f, y, x - w - 8.f, y + h, Color::Black());
    VM_END
}

void ESP::Render()
{
    VM_BEGIN("HACKS_ESP_RENDER")
    if (!ESPEnabled() || !g::pLocalEntity || !g_pEngine->IsInGame() || !g_pEngine->IsConnected() )
        return;

    for (int it = 1; it <= g_pEngine->GetMaxClients(); ++it)
    {
        C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(it);


        if (!pPlayerEntity
            || !pPlayerEntity->IsAlive()
            || pPlayerEntity->IsDormant()
            || pPlayerEntity == g::pLocalEntity
            || (g_Settings.Visuals.Filter.bFriends && pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
            )
            continue;

        Vector max = pPlayerEntity->GetCollideable()->OBBMaxs();
        Vector top3D, pos3D;
        Vector top, pos;
        pos3D = pPlayerEntity->GetOrigin();
        top3D = pPlayerEntity->GetFlags() & FL_DUCKING ? pos3D + Vector(0, 0, 56.f) : pos3D + Vector(0, 0, 72.f);

        if (!Utils::WorldToScreen(pos3D, pos) || !Utils::WorldToScreen(top3D, top))
            continue;

        float height = (pos.y - top.y);
        float width = height / 4.f;

        if (g_Settings.Visuals.Type.bShowSkeleton)
            RenderSkeleton(pPlayerEntity);

        if (g_Settings.Visuals.Type.bShowBoxes)
            RenderBox(top.x, top.y, width, height, pPlayerEntity);

        if (g_Settings.Visuals.Type.bShowNames)
            RenderName(top.x, top.y, width, height, pPlayerEntity, it);

        if (g_Settings.Visuals.Type.bShowHealth)
            RenderHealth(top.x, top.y, width, height, pPlayerEntity);

        if (g_Settings.Visuals.Type.bShowArmor)
            RenderArmor(top.x, top.y, width, height, pPlayerEntity);

        RenderInfo(top.x, top.y, width, height, pPlayerEntity);
    }
    VM_END
}
