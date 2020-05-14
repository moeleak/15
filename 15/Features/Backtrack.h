#pragma once
#include <deque>
#include "../SDK/SDK.h"
#include "../Utils/GlobalVars.h"
#include "../Utils/Math.h"
#include "../Utils/Utils.h"

#define MAX_PLAYERS 64

struct LagRecord
{
    LagRecord()
    {
        m_fFlags = 0;
        m_vecOrigin.Init();
        m_vecAbsAngles.Init();
        m_flSimulationTime = -1;
        m_flLowerBodyYawTarget = 0.f;
        m_masterCycle = 0.f;
        m_masterSequence = 0;
        m_pMatrix = nullptr;
    }

    LagRecord(const LagRecord& src)
    {
        m_fFlags = src.m_fFlags;
        m_vecOrigin = src.m_vecOrigin;
        m_vecAbsAngles = src.m_vecAbsAngles;
        m_flSimulationTime = src.m_flSimulationTime;
        m_flLowerBodyYawTarget = src.m_flLowerBodyYawTarget;
        m_masterCycle = src.m_masterCycle;
        m_masterSequence = src.m_masterSequence;
        m_pMatrix = src.m_pMatrix;

        for (int poseparams = 0; poseparams < 24; poseparams++)
            m_flPoseParameter[poseparams] = src.m_flPoseParameter[poseparams];
    }

    // Did player die this frame
    int                     m_fFlags;
    Vector                  m_vecOrigin;
    QAngle                  m_vecAbsAngles;
    float                   m_flLowerBodyYawTarget;

    float                   m_flSimulationTime;

    float                   m_flPoseParameter[24];

    float                   m_masterCycle;
    int                     m_masterSequence;

    matrix3x4_t*            m_pMatrix;

};

class VarMapEntry_t
{
public:
    unsigned short type;
    unsigned short m_bNeedsToInterpolate; // Set to false when this var doesn't
                                          // need Interpolate() called on it anymore.
    void *data;
    void *watcher;
};

struct VarMapping_t
{
    VarMapping_t()
    {
        m_nInterpolatedEntries = 0;
    }

    VarMapEntry_t* m_Entries;
    int m_nInterpolatedEntries;
    float m_lastInterpolationTime;
};



class Backtrack
{
public:
    bool StartLagCompensation(C_BaseEntity * player, CUserCmd * cmd, int bestrecord);
    int FixTickcount(C_BaseEntity * player, int bestrecord);
    void FrameUpdatePostEntityThink(ClientFrameStage_t stage);

    bool ValidTick(float simTime);

    void BacktrackPlayer(C_BaseEntity *player, LagRecord record);
    bool IsValid(C_BaseEntity * pPlayer);
    int GetBestTick(C_BaseEntity* pPlayer);

    void BackupPlayer(C_BaseEntity* pPlayer);
    void RestorePlayer(C_BaseEntity* pPlayer);

    void SceneEnd();
    void CreateMove();

    std::deque< LagRecord >	m_PlayerTrack[MAX_PLAYERS];
    LagRecord m_PrevRecords[MAX_PLAYERS];

    void ClearHistory()
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
            m_PlayerTrack[i].clear();
    }


    VarMapping_t* GetVarMap(void* pBaseEntity)
    {
        return reinterpret_cast<VarMapping_t*>((DWORD)pBaseEntity + 0x24); //0x4C );
    }
public:
    void DisableInterpolation(C_BaseEntity* pEntity)
    {
        VarMapping_t* map = GetVarMap(pEntity);
        if (!map) return;
        for (int i = 0; i < map->m_nInterpolatedEntries; i++)
        {
            VarMapEntry_t *e = &map->m_Entries[i];
            e->m_bNeedsToInterpolate = false;
        }
    }
};