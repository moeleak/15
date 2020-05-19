#include "Backtrack.h"

#define TICK_INTERVAL			( g_pGlobalVars->intervalPerTick )


#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL *( t ) )
#define ROUND_TO_TICKS( t )		( TICK_INTERVAL * TIME_TO_TICKS( t ) )

float GetLerpTime()
{
    auto cl_updaterate = g_pCVar->FindVar("cl_updaterate");
    auto sv_minupdaterate = g_pCVar->FindVar("sv_minupdaterate");
    auto sv_maxupdaterate = g_pCVar->FindVar("sv_maxupdaterate");

    auto cl_interp = g_pCVar->FindVar("cl_interp");
    auto sv_client_min_interp_ratio = g_pCVar->FindVar("sv_client_min_interp_ratio");
    auto sv_client_max_interp_ratio = g_pCVar->FindVar("sv_client_max_interp_ratio");

    auto updateRate = cl_updaterate->GetInt();
    auto interpRatio = cl_interp->GetFloat();
    auto minInterpRatio = sv_client_min_interp_ratio->GetFloat();
    auto maxInterpRatio = sv_client_max_interp_ratio->GetFloat();
    auto minUpdateRate = sv_minupdaterate->GetInt();
    auto maxUpdateRate = sv_maxupdaterate->GetInt();

    auto clampedUpdateRate = std::clamp(updateRate, minUpdateRate, maxUpdateRate);
    auto clampedInterpRatio = std::clamp(interpRatio, minInterpRatio, maxInterpRatio);

    auto lerp = clampedInterpRatio / clampedUpdateRate;

    if (lerp <= cl_interp->GetFloat())
        lerp = cl_interp->GetFloat();

    return lerp;
}

bool Backtrack::StartLagCompensation(C_BaseEntity * player, CUserCmd * cmd, int bestrecord)
{
    if (!IsValid(player))
        return false;

    int idx = player->GetIClientEntity()->EntIndex();

    std::deque< LagRecord > *track = &m_PlayerTrack[idx - 1];

    LagRecord &record = track->at(bestrecord);

    INetChannelInfo *nci = g_pEngine->GetNetChannelInfo();
    auto lerpTime = GetLerpTime();


    auto predictedCmdArrivalTick = g_pGlobalVars->tickcount + 1 + TIME_TO_TICKS(nci->GetAvgLatency(FLOW_INCOMING) + nci->GetAvgLatency(FLOW_OUTGOING));
    auto deltaTime = std::clamp(lerpTime + nci->GetLatency(FLOW_OUTGOING), 0.f, 1.f) - TICKS_TO_TIME(predictedCmdArrivalTick + TIME_TO_TICKS(lerpTime) - TIME_TO_TICKS(record.m_flSimulationTime));

    if (fabs(deltaTime) > 0.2f)
        return false;

    BacktrackPlayer(player, record);

    return true;
}

int Backtrack::FixTickcount(C_BaseEntity* player, int bestrecord)
{
    int idx = player->GetIClientEntity()->EntIndex();

    std::deque< LagRecord > *track = &m_PlayerTrack[idx - 1];
    LagRecord &record = track->at(bestrecord);

    if (record.m_flSimulationTime == 0.0f)
        return TIME_TO_TICKS(player->GetSimulationTime() + GetLerpTime()) + 1;

    int iLerpTicks = TIME_TO_TICKS(GetLerpTime());
    int iTargetTickCount = TIME_TO_TICKS(record.m_flSimulationTime) + iLerpTicks;

    return iTargetTickCount;
}


void Backtrack::BacktrackPlayer(C_BaseEntity * player, LagRecord record)
{
    if (!player || record.m_flSimulationTime == 0.0f)
        return;

    static uintptr_t pInvalidateBoneCache = Utils::FindSignature(XORSTR("client_panorama.dll"), XORSTR("80 3D ? ? ? ? 00 74 16 A1"));
    unsigned long g_iModelBoneCounter = **reinterpret_cast< unsigned long** > (pInvalidateBoneCache + 10);
    *reinterpret_cast< unsigned int* >(reinterpret_cast< uintptr_t >(player) + 0x2914) = 0xFF7FFFFF;
    *reinterpret_cast< unsigned int* >(reinterpret_cast< uintptr_t >(player) + 0x2680) = (g_iModelBoneCounter - 1);

    player->SetAbsOrigin(record.m_vecOrigin);
    player->SetAbsAngles(record.m_vecAbsAngles);
    player->GetSimulationTime() = record.m_flSimulationTime;
    player->GetFlags() = (EntityFlags)record.m_fFlags;
    player->GetLowerBodyYawTarget() = record.m_flLowerBodyYawTarget;

    for (int poseparam = 0; poseparam < 24; poseparam++)
        player->GetPoseParameter()[poseparam] = record.m_flPoseParameter[poseparam];

    player->UpdateAnimations();
}

void Backtrack::FrameUpdatePostEntityThink(ClientFrameStage_t stage)
{
    if (stage != FRAME_NET_UPDATE_POSTDATAUPDATE_START)
        return;

    static auto sv_unlag = g_pCVar->FindVar("sv_unlag");
    static auto sv_maxunlag = g_pCVar->FindVar("sv_maxunlag");

    if ((g_pGlobalVars->maxClients <= 1) || !sv_unlag->GetBool())
    {
        ClearHistory();
        return;
    }

    auto flDeadtime = g_pGlobalVars->curtime - sv_maxunlag->GetFloat();

    for (int i = 1; i <= g_pGlobalVars->maxClients; i++)
    {
        auto pPlayer = g_pEntityList->GetClientEntity(i);

        auto &track = m_PlayerTrack[i - 1];

        if (!IsValid(pPlayer))
        {
            if (track.size() > 0)
            {
                track.clear();
            }

            continue;
        }

        *reinterpret_cast< int* > (reinterpret_cast< uintptr_t >(pPlayer) + 0xA30) = g_pGlobalVars->framecount;
        *reinterpret_cast< int* > (reinterpret_cast< uintptr_t >(pPlayer) + 0xA28) = 0;

        DisableInterpolation(pPlayer);

        Assert(track->Count() < 1000); // insanity check

                                       // remove tail records that are too old
        int tailIndex = track.size() - 1;
        while (track.size())
        {
            LagRecord &tail = *(track.end() - 1);

            // if tail is within limits, stop
            if (tail.m_flSimulationTime >= flDeadtime)
                break;

            // remove tail, get new tail
            track.pop_back();
            tailIndex = track.size() - 1;
        }

        // check if head has same simulation time
        if (track.size() > 0)
        {
            LagRecord &head = *track.begin();

            // check if player changed simulation time since last time updated
            if (head.m_flSimulationTime >= pPlayer->GetSimulationTime())
                continue; // don't add new entry for same or older time
        }

        // add new record to player track
        track.push_front(LagRecord());
        LagRecord &record = *track.begin();


        static uintptr_t pInvalidateBoneCache = Utils::FindSignature(XORSTR("client_panorama.dll"), XORSTR("80 3D ? ? ? ? 00 74 16 A1"));
        unsigned long g_iModelBoneCounter = **reinterpret_cast< unsigned long** > (pInvalidateBoneCache + 10);
        *reinterpret_cast< unsigned int* >(reinterpret_cast< uintptr_t >(pPlayer) + 0x2914) = 0xFF7FFFFF;
        *reinterpret_cast< unsigned int* >(reinterpret_cast< uintptr_t >(pPlayer) + 0x2680) = (g_iModelBoneCounter - 1);

        record.m_flSimulationTime = pPlayer->GetSimulationTime();
        record.m_vecAbsAngles = pPlayer->GetAbsAngles().Clamp();
        record.m_vecOrigin = pPlayer->GetOrigin();
        record.m_fFlags = pPlayer->GetFlags();
        record.m_flLowerBodyYawTarget = pPlayer->GetLowerBodyYawTarget();
        pPlayer->SetupBones(record.m_pMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0);

        for (int poseparam = 0; poseparam < 24; poseparam++)
            record.m_flPoseParameter[poseparam] = pPlayer->GetPoseParameter()[poseparam];

    }
}

bool Backtrack::ValidTick(float simTime)
{
    INetChannelInfo *nci = g_pEngine->GetNetChannelInfo();
    if (!nci)
        return false;

    auto lerpTime = GetLerpTime();

    auto predictedCmdArrivalTick = g_pGlobalVars->tickcount + 1 + TIME_TO_TICKS(nci->GetAvgLatency(FLOW_INCOMING) + nci->GetAvgLatency(FLOW_OUTGOING));
    auto deltaTime = std::clamp(lerpTime + nci->GetLatency(FLOW_OUTGOING), 0.f, 1.f) - TICKS_TO_TIME(predictedCmdArrivalTick + TIME_TO_TICKS(lerpTime) - TIME_TO_TICKS(simTime));

    return fabs(deltaTime) > 0.2f;
}

int Backtrack::GetBestTick(C_BaseEntity * pPlayer)
{
    int tickcount = g_pGlobalVars->tickcount;

    std::deque< LagRecord > &track = m_PlayerTrack[pPlayer->GetIClientEntity()->EntIndex() - 1];
    for (int i = track.size(); i > 0; i--)
    {
        if (StartLagCompensation(pPlayer, NULL, i - 1))
        {
            tickcount = FixTickcount(pPlayer, i - 1);
            break;
        }
    }

    return tickcount;
}


void Backtrack::BackupPlayer(C_BaseEntity * pPlayer)
{
    int idx = pPlayer->GetIClientEntity()->EntIndex();

    auto &record = m_PrevRecords[idx - 1];

    static uintptr_t pInvalidateBoneCache = Utils::FindSignature(XORSTR("client_panorama.dll"), XORSTR("80 3D ? ? ? ? 00 74 16 A1"));
    unsigned long g_iModelBoneCounter = **reinterpret_cast< unsigned long** > (pInvalidateBoneCache + 10);
    *reinterpret_cast< unsigned int* >(reinterpret_cast< uintptr_t >(pPlayer) + 0x2914) = 0xFF7FFFFF;
    *reinterpret_cast< unsigned int* >(reinterpret_cast< uintptr_t >(pPlayer) + 0x2680) = (g_iModelBoneCounter - 1);

    record.m_flSimulationTime = pPlayer->GetSimulationTime();
    record.m_vecAbsAngles = pPlayer->GetAbsAngles().Clamp();
    record.m_vecOrigin = pPlayer->GetAbsOrigin();
    record.m_fFlags = pPlayer->GetFlags();
    record.m_flLowerBodyYawTarget = pPlayer->GetLowerBodyYawTarget();

    for (int poseparam = 0; poseparam < 24; poseparam++)
        record.m_flPoseParameter[poseparam] = pPlayer->GetPoseParameter()[poseparam];

}

void Backtrack::RestorePlayer(C_BaseEntity * pPlayer)
{
    int idx = pPlayer->GetIClientEntity()->EntIndex();

    auto &record = m_PrevRecords[idx - 1];

    static uintptr_t pInvalidateBoneCache = Utils::FindSignature(XORSTR("client_panorama.dll"), XORSTR("80 3D ? ? ? ? 00 74 16 A1"));
    unsigned long g_iModelBoneCounter = **reinterpret_cast< unsigned long** > (pInvalidateBoneCache + 10);
    *reinterpret_cast< unsigned int* >(reinterpret_cast< uintptr_t >(pPlayer) + 0x2914) = 0xFF7FFFFF;
    *reinterpret_cast< unsigned int* >(reinterpret_cast< uintptr_t >(pPlayer) + 0x2680) = (g_iModelBoneCounter - 1);

    pPlayer->SetAbsOrigin(record.m_vecOrigin);
    pPlayer->SetAbsAngles(record.m_vecAbsAngles);
    pPlayer->GetSimulationTime() = record.m_flSimulationTime;
    pPlayer->GetFlags() = (EntityFlags)record.m_fFlags;
    pPlayer->GetLowerBodyYawTarget() = record.m_flLowerBodyYawTarget;

    for (int poseparam = 0; poseparam < 24; poseparam++)
        pPlayer->GetPoseParameter()[poseparam] = record.m_flPoseParameter[poseparam];

    pPlayer->UpdateAnimations();
}

bool Backtrack::IsValid(C_BaseEntity* pPlayer)
{
    if (!pPlayer)
        return false;

    if (pPlayer->IsDormant() || !pPlayer->IsAlive() || pPlayer->GetFlags() & FL_FROZEN)
        return false;

    if (pPlayer->GetTeam() == g::pLocalEntity->GetTeam())
        return false;

    if (pPlayer->GetClientClass()->ClassID != EClassIds::CCSPlayer)
        return false;

    if (pPlayer->GetIClientEntity()->EntIndex() == g_pEngine->GetLocalPlayer())
        return false;
        
    if (pPlayer->IsImmune())
        return false;

    return true;
}

void Backtrack::SceneEnd()
{

    IMaterial *mat = g_pMaterialSystem->FindMaterial("simple_regular", TEXTURE_GROUP_MODEL);

    if (!mat || mat->IsErrorMaterial())
        return;

    for (int i = 0; i < g_pGlobalVars->maxClients; i++)
    {
        auto pPlayer = g_pEntityList->GetClientEntity(i);

        if (!pPlayer || pPlayer == g::pLocalEntity || !pPlayer->IsAlive() || pPlayer->IsDormant() || pPlayer->GetTeam() == g::pLocalEntity->GetTeam())
             continue;

        auto &track = m_PlayerTrack[i];

        Utils::Log("track.size: %d", track.size());

        BackupPlayer(pPlayer);
        for (int j = track.size(); j > 0; j--)
        {
            if (StartLagCompensation(pPlayer, NULL, j - 1))
            {
                g_pRenderView->SetColorModulation(166 / 255, 167 / 255, 169 / 255);

                mat->IncrementReferenceCount();
                mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);

                g_pMdlRender->ForcedMaterialOverride(mat);

                pPlayer->DrawModel(0x1, 255);
                g_pMdlRender->ForcedMaterialOverride(nullptr);

                g_pRenderView->SetColorModulation(166 / 255, 167 / 255, 169 / 255);

                mat->IncrementReferenceCount();
                mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);

                g_pMdlRender->ForcedMaterialOverride(mat);

                pPlayer->DrawModel(0x1, 255);
                g_pMdlRender->ForcedMaterialOverride(nullptr);
                break;
            }
        }
        RestorePlayer(pPlayer);
    }
}


void Backtrack::CreateMove()
{
    if (g::pLocalEntity->IsAlive() && g::pLocalEntity->GetActiveWeapon())
        g::pCmd->viewangles -= g::pLocalEntity->GetAimPunchAngle() * g_pCVar->FindVar("weapon_recoil_scale")->GetFloat();

    for (int i = 0; i < g_pGlobalVars->maxClients; i++)
    {
        auto pEnt = g_pEntityList->GetClientEntity(i);

        if (!pEnt || pEnt == g::pLocalEntity || pEnt->GetClientClass()->ClassID != EClassIds::CCSPlayer || !pEnt->IsAlive() || pEnt->IsDormant() || pEnt->GetTeam() == g::pLocalEntity->GetTeam())
            continue;

        BackupPlayer(pEnt);
        int bestrec = GetBestTick(pEnt);
        RestorePlayer(pEnt);

        if (g::pCmd->buttons & IN_ATTACK)
            g::pCmd->tick_count = bestrec;

    }
}