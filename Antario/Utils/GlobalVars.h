#pragma once
#include "../SDK/SDK.h"

namespace g
{
    extern CUserCmd*		pCmd;
    extern C_BaseEntity*	pLocalEntity;
    extern std::uintptr_t	uRandomSeed;
    extern HWND             hWnd;
    extern bool             bIsNewRound;
    extern int              iWeaponId;
    extern QAngle           RealAngle;
}