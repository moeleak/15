#include "Bunnyhop.h"

void Bunnyhop::CreateMove(CUserCmd* pCmd)
{
    VM_BEGIN("HACKS_BHOP")

    if (!(g::pLocalEntity->GetFlags() & EntityFlags::FL_ONGROUND))
    {
        if (g::pLocalEntity->GetMoveType() == MoveType_t::MOVETYPE_LADDER || g::pLocalEntity->GetMoveType() == MoveType_t::MOVETYPE_NOCLIP || g::pLocalEntity->GetMoveType() == MoveType_t::MOVETYPE_OBSERVER)
            return;

        if (g_Settings.Miscellaneous.bAutobhop)
            if (g::pCmd->buttons & IN_JUMP)
                g::pCmd->buttons &= ~IN_JUMP;

        if (g_Settings.Miscellaneous.bAutoStrafe)
        {
            if (g::pCmd->sidemove != 0.f || g::pCmd->forwardmove != 0.f)
                return;

            auto redian_to_degrees = [](float ang) -> float
            {
                return (ang * 180.0f) / M_PI;
            };

            auto normalize_ang = [](float ang) -> float
            {
                while (ang < -180.0f) ang += 360.0f;
                while (ang > 180.0f) ang -= 360.0f;

                return ang;
            };

            if (g::pCmd->mousedx > 0 || g::pCmd->mousedx < 0)
                g::pCmd->sidemove = g::pCmd->mousedx < 0.f ? -450.f : 450.f;
            else
            {
                auto vel = g::pLocalEntity->GetVelocity();
                float y_vel = redian_to_degrees(atan2(vel.y, vel.x));
                float diff_ang = normalize_ang(g::pCmd->viewangles.y - y_vel);
                g::pCmd->sidemove = (diff_ang > 0.0) ? -450.f : 450.f;
                g::pCmd->viewangles.y = normalize_ang(g::pCmd->viewangles.y - diff_ang);
            }
        }
    }
    else
    {
        if (g_Settings.Miscellaneous.bGroundStrafe && !(g::pCmd->buttons & IN_JUMP) && (g::pCmd->sidemove != 0  || g::pCmd->forwardmove != 0 ))
        {
            if (g::pCmd->command_number % 2)
                g::pCmd->viewangles.y += 26.86f;
            else
                g::pCmd->viewangles.y -= 26.86f;
        }
    }
    VM_END
}