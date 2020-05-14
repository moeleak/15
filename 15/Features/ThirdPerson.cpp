#include "ThirdPerson.h"

namespace
{
    bool bInTP = false;
}

void ThirdPerson::FrameStageNotify(ClientFrameStage_t stage)
{
    if (stage != FRAME_RENDER_START)
        return;

	if (g_Settings.Miscellaneous.bThirdPerson && g::pLocalEntity->IsAlive())
		if (g_pInput->m_fCameraInThirdPerson)
			*reinterpret_cast<QAngle*>(reinterpret_cast<uintptr_t>(g::pLocalEntity) + 0x31C8) = g::RealAngle;

}

void ThirdPerson::OverrideView()
{
    if (!g::pLocalEntity || !g::pLocalEntity->IsAlive())
        return;

    if (g_Settings.Miscellaneous.bThirdPerson)
    {
        if (GetAsyncKeyState(0x54) & 1)
            bInTP = !bInTP;

        if (bInTP)
        {
            if (!g_pInput->m_fCameraInThirdPerson)
            {
                g_pInput->m_fCameraInThirdPerson = true;
                g_pInput->m_vecCameraOffset = Vector(g::RealAngle.x, g::RealAngle.y, 175.f);
            }
        }
        else
        {
            g_pInput->m_fCameraInThirdPerson = false;
            g_pInput->m_vecCameraOffset = Vector(g::RealAngle.x, g::RealAngle.y, 0.f);
        }
    }

}