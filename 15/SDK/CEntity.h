#pragma once
#include "Definitions.h"
#include "IClientUnknown.h"
#include "IClientEntityList.h"
#include "Misc.h"
#include "../Utils/Math.h"
#include "../Utils/Utils.h"
#include "../Utils/NetvarManager.h"
#include "../Utils/Xorstr.h"

// class predefinition
class C_BaseCombatWeapon;


class C_BaseEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable
{
private:
    datamap_t * GetDataDescMap()
    {
        return Utils::CallVFunc<15, datamap_t*>(this);
    }

    datamap_t *GetPredDescMap()
    {
        return Utils::CallVFunc<17, datamap_t*>(this);
    }
public:
    OFFSET(GetMoveType, MoveType_t, 0x258)
    NETVAR(GetFlags, EntityFlags, "CBasePlayer->m_fFlags")
    NETVAR(GetTeam, int, "CBaseEntity->m_iTeamNum")
    NETVAR(GetLifeState, bool, "CBasePlayer->m_lifeState")
    NETVAR(GetArmor, int, "CCSPlayer->m_ArmorValue")
    NETVAR(GetHealth, int, "CBasePlayer->m_iHealth")
    NETVAR(IsImmune, bool, "CCSPlayer->m_bGunGameImmunity")
    NETVAR(GetTickBase, int, "CBasePlayer->m_nTickBase")
    NETVAR(IsScoped, bool, "CCSPlayer->m_bIsScoped")
    NETVAR(GetMoney, int, "CCSPlayer->m_iAccount")
    NETVAR(GetOrigin, Vector, "CBaseEntity->m_vecOrigin")
    NETVAR(GetViewOffset, Vector, "CBasePlayer->m_vecViewOffset[0]")
    NETVAR(GetAimPunchAngle, QAngle, "CBasePlayer->m_aimPunchAngle")
    NETVAR(GetHitboxSet, int, "CBaseAnimating->m_nHitboxSet")
    NETVAR(GetVelocity, Vector, "CBasePlayer->m_vecVelocity[0]")
    NETVAR(GetSimulationTime, float, "CBaseEntity->m_flSimulationTime")
    NETVAR(GetLowerBodyYawTarget, float, "CCSPlayer->m_flLowerBodyYawTarget")
    NETVAR(GetDuckAmount, float, "CCSPlayer->m_flDuckAmount")
    PNETVAR(GetActiveWeaponHandle, CBaseHandle, "CBaseCombatCharacter->m_hActiveWeapon")
    PNETVAR(GetPoseParameter, float, "CBaseAnimating->m_flPoseParameter")

    bool IsAlive() { return this->GetHealth() > 0 && this->GetLifeState() == 0; }

    Vector GetEyePosition() { return this->GetOrigin() + this->GetViewOffset(); }

    void UpdateAnimations() { return Utils::CallVFunc<218, void>(this); }

    QAngle & GetAbsAngles() { return Utils::CallVFunc<11, QAngle&>(this); }

    Vector & GetAbsOrigin() { return Utils::CallVFunc<10, Vector&>(this); }

    int GetWaterLevel()
    {
        static unsigned int m_nWaterLevel = Utils::FindInDataMap(GetPredDescMap(), XORSTR("m_nWaterLevel"));
        return *(int*)((uintptr_t)this + m_nWaterLevel);
    }

    int GetGroundEntity()
    {
        static unsigned int m_hGroundEntity = Utils::FindInDataMap(GetPredDescMap(), XORSTR("m_hGroundEntity"));
        return *(int*)((uintptr_t)this + m_hGroundEntity);
    }

    void SetAbsOrigin(Vector vec)
    {
        using SetAbsOriginFn = void(__thiscall*)(void*, const Vector &origin);
        static SetAbsOriginFn SetAbsOrigin;

        if (!SetAbsOrigin)
            SetAbsOrigin = reinterpret_cast< SetAbsOriginFn >(Utils::FindSignature(XORSTR("client.dll"), XORSTR("55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8 ?? ??")));

        SetAbsOrigin(this, vec);
    }

    void SetAbsAngles(QAngle vec)
    {
        using SetAbsAnglesFn = void(__thiscall*)(void*, const QAngle &angles);
        static SetAbsAnglesFn SetAbsAngles;

        if (!SetAbsAngles)
            SetAbsAngles = reinterpret_cast< SetAbsAnglesFn >((uintptr_t)Utils::FindSignature(XORSTR("client.dll"), XORSTR("55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8")));

        SetAbsAngles(this, vec);
    }

    C_BaseCombatWeapon* GetActiveWeapon() 
    { 
        auto handle = this->GetActiveWeaponHandle();
        if (!handle)
            return 0; 

        return reinterpret_cast<C_BaseCombatWeapon*>(g_pEntityList->GetClientEntityFromHandle(*handle)); 
    }

    Vector GetHitboxPos(int hitbox_id)
    {
        matrix3x4_t boneMatrix[MAXSTUDIOBONES];

        if (SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) {
            auto studio_model = g_pModelInfo->GetStudioModel(GetModel());
            if (studio_model) {
                auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(hitbox_id);
                if (hitbox) {
                    auto
                        min = Vector{},
                        max = Vector{};

                    Math::VectorTransform(hitbox->bbmin, boneMatrix[hitbox->bone], min);
                    Math::VectorTransform(hitbox->bbmax, boneMatrix[hitbox->bone], max);

                    return (min + max) / 2.0f;
                }
            }
        }
        return Vector{};
    }

    Vector GetBonePos(int bone)
    {
        matrix3x4_t boneMatrix[128];
        if (this->SetupBones(boneMatrix, 128, BONE_USED_BY_HITBOX, (float)GetTickCount64()))
        {
            return Vector(boneMatrix[bone][0][3], boneMatrix[bone][1][3], boneMatrix[bone][2][3]);
        }
        return Vector(0, 0, 0);
    }

    bool CanSeePlayer(C_BaseEntity* player, int hitbox)
    {
        CGameTrace tr;
        Ray_t ray;
        CTraceFilter filter;
        filter.pSkip = this;

        auto endpos = player->GetHitboxPos(hitbox);

        ray.Init(GetEyePosition(), endpos);
        g_pEngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

        return (C_BaseEntity*)tr.hit_entity == player || tr.fraction > 0.97f;
    }


    bool CanSeePlayer(C_BaseEntity* player, const Vector& pos)
    {
        CGameTrace tr;
        Ray_t ray;
        CTraceFilter filter;
        filter.pSkip = this;

        auto start = GetEyePosition();
        auto dir = (pos - start);
        Math::NormalizeVector(dir);

        ray.Init(start, pos);
        g_pEngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

        return (C_BaseEntity*)tr.hit_entity == player || tr.fraction > 0.97f;
    }

};

class CPlayerResource {};

class C_CSPlayerResource : public CPlayerResource
{
public:

};
extern C_CSPlayerResource* g_pPlayerResource;

class C_CSGameRulesProxy
{
public:
    OFFSET(m_bFreezePeriod, bool, 0x20)
    OFFSET(m_bWarmupPeriod, bool, 0x21)
    OFFSET(m_bIsValveDS, bool, 0x75)
};

class IWeaponSystem
{
    virtual void unused0() = 0;
    virtual void unused1() = 0;
public:
    virtual const WeaponInfo_t* GetCSWpnData(ItemDefinitionIndex ItemDefinitionIndex) = 0;
};
extern IWeaponSystem* g_pWeaponSystem;

class C_BaseCombatWeapon : public C_BaseEntity
{
public:
    NETVAR(GetItemDefinitionIndex, ItemDefinitionIndex, "CBaseAttributableItem->m_iItemDefinitionIndex")
    NETVAR(GetNextPrimaryAttack, float, "CBaseCombatWeapon->m_flNextPrimaryAttack")
    NETVAR(GetAmmo, int, "CBaseCombatWeapon->m_iClip1")

    bool CanShoot()
    {
        auto i = this->GetItemDefinitionIndex();

        if (i == ItemDefinitionIndex::WEAPON_KNIFE || i == ItemDefinitionIndex::WEAPON_KNIFE_BAYONET || i == ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY || i == ItemDefinitionIndex::WEAPON_KNIFE_FALCHION || i == ItemDefinitionIndex::WEAPON_KNIFE_FLIP || i == ItemDefinitionIndex::WEAPON_KNIFE_GUT
            || i == ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT || i == ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET || i == ItemDefinitionIndex::WEAPON_KNIFE_PUSH || i == ItemDefinitionIndex::WEAPON_KNIFE_SURVIVAL_BOWIE || i == ItemDefinitionIndex::WEAPON_KNIFE_T || i == ItemDefinitionIndex::WEAPON_KNIFE_TACTICAL
            || i == ItemDefinitionIndex::WEAPON_TASER)
            return false;

        if (i == ItemDefinitionIndex::WEAPON_FLASHBANG || i == ItemDefinitionIndex::WEAPON_HEGRENADE || i == ItemDefinitionIndex::WEAPON_MOLOTOV || i == ItemDefinitionIndex::WEAPON_INCGRENADE || i == ItemDefinitionIndex::WEAPON_DECOY || i == ItemDefinitionIndex::WEAPON_SMOKEGRENADE)
            return false;

        if (i == ItemDefinitionIndex::WEAPON_C4)
            return false;

        if (this->GetAmmo() == 0)
            return false;

        float flNextAttackTime = this->GetNextPrimaryAttack() - g_pGlobalVars->curtime;

        if (flNextAttackTime >= 0.1f)
            return false;

        return true;
    }

    const WeaponInfo_t* GetCSWpnData()
    {
        using Fn = WeaponInfo_t * (__thiscall*)(void*);

        static auto fn = reinterpret_cast<Fn>(Utils::FindSignature(XORSTR("client.dll"), XORSTR("55 8B EC 81 EC ? ? ? ? 53 8B D9 56 57 8D 8B")));
        return fn(this);
    }

    std::string GetName()
    {
        ///TODO: Test if szWeaponName returns proper value for m4a4 / m4a1-s or it doesnt recognize them.
        return std::string(this->GetCSWpnData()->szWeaponName);
    }
};
