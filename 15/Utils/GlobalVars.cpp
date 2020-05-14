#include "GlobalVars.h"

namespace g
{
    CUserCmd*		pCmd            = nullptr;
    C_BaseEntity*	pLocalEntity    = nullptr;
    std::uintptr_t	uRandomSeed     = NULL;
    HWND			hWnd            = NULL;
    bool            bIsNewRound     = false;
    int             iWeaponId       = 0;
    QAngle          RealAngle       = QAngle(0, 0, 0);
    //C_CSGameRulesProxy* pGameRulesProxy = nullptr;
}