#include "../SDK/SDK.h"
#include "../Utils/GlobalVars.h"
#include "../Settings.h"
#include "../Utils/Math.h"

#include "Aimbot.h"

namespace
{
    float curAimTime = 0.0;
    float deltaTime = 0.0;
}

namespace
{
    int GetClosestBone(C_BaseEntity *Entity, CUserCmd* cmd)
    {

        float BestDist = 360.f;
        int aimbone;

        matrix3x4_t matrix[MAXSTUDIOBONES];

        if (!Entity->SetupBones(matrix, 128, BONE_USED_BY_HITBOX, g_pEngine->GetLastTimeStamp()))
            return -1;

        studiohdr_t* pStudioModel = g_pModelInfo->GetStudioModel(Entity->GetModel());
        if (!pStudioModel)
            return -1;

        mstudiohitboxset_t* set = pStudioModel->GetHitboxSet(Entity->GetHitboxSet());
        if (!set)
            return -1;

        for (int i = 0; i < set->numhitboxes; i++)
        {
            if (i == (int)HitBox::HITBOX_RIGHT_THIGH || i == (int)HitBox::HITBOX_LEFT_THIGH || i == (int)HitBox::HITBOX_RIGHT_CALF
                || i == (int)HitBox::HITBOX_LEFT_CALF || i == (int)HitBox::HITBOX_RIGHT_FOOT || i == (int)HitBox::HITBOX_LEFT_FOOT
                || i == (int)HitBox::HITBOX_RIGHT_HAND || i == (int)HitBox::HITBOX_LEFT_HAND || i == (int)HitBox::HITBOX_RIGHT_UPPER_ARM
                || i == (int)HitBox::HITBOX_RIGHT_FOREARM || i == (int)HitBox::HITBOX_LEFT_UPPER_ARM || i == (int)HitBox::HITBOX_LEFT_FOREARM)
                continue;

            mstudiobbox_t* hitbox = set->GetHitbox(i);

            if (!hitbox)
                continue;

            auto thisdist = Math::GetFov(cmd->viewangles, Math::CalculateAngle(g::pLocalEntity->GetEyePosition(), Vector(matrix[hitbox->bone][0][3], matrix[hitbox->bone][1][3], matrix[hitbox->bone][2][3])));

            if (BestDist > thisdist)
            {
                BestDist = thisdist;
                aimbone = hitbox->bone;
                continue;
            }
        }
        return aimbone;

    }

    int GetTarget(C_BaseEntity* pLocalPlayer, CUserCmd* pCmd)
    {
        float flMax = 8192.f;
        int iBestTarget = -1;
        curAimTime = 0.f;

        if (pLocalPlayer->IsAlive() && !pLocalPlayer->IsDormant())
        {
            for (int i = 0; i < g_pEntityList->GetHighestEntityIndex(); i++)
            {
                C_BaseEntity* pEnt = g_pEntityList->GetClientEntity(i);
                if (!pEnt)
                    continue;
                if (pEnt == pLocalPlayer)
                    continue;
                if (!pEnt->IsAlive())
                    continue;
                if (pEnt->IsDormant())
                    continue;
                if (pEnt->GetTeam() == pLocalPlayer->GetTeam())
                    continue;
                if (!pEnt->CanSeePlayer(pEnt, GetClosestBone(pEnt, pCmd)))
                    continue;
                float flFoV = Math::GetFov(pCmd->viewangles, Math::CalculateAngle(pLocalPlayer->GetEyePosition(), pEnt->GetBonePos(GetClosestBone(pEnt, pCmd))));
                if (flFoV < flMax)
                {
                    flMax = flFoV;
                    iBestTarget = i;
                }
            }
        }
        return iBestTarget;
    }
}


void Aimbot::CreateMove(CUserCmd* cmd)
{
    if (!g::pLocalEntity || !g::pLocalEntity->IsAlive() || !g_Settings.LegitBot.WeaponConfig[g::iWeaponId].bShouldAimbot ||
        !g::pLocalEntity->GetActiveWeapon())
        return;

    if (!g::pLocalEntity->GetActiveWeapon()->CanShoot())
        return;

    auto pTarget = g_pEntityList->GetClientEntity(GetTarget(g::pLocalEntity, cmd));

    auto vecBone = pTarget->GetBonePos(GetClosestBone(pTarget, cmd));

    if (vecBone.IsZero())
        return;

    auto AimAngle = Math::CalculateAngle(g::pLocalEntity->GetEyePosition(), vecBone);

    if (!pTarget->CanSeePlayer(pTarget, GetClosestBone(pTarget, cmd)))
        return;

    if (g_Settings.Cheat.bMenuOpened)
        return;

}