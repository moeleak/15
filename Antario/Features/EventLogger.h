#pragma once

#include "../Utils/GlobalVars.h"
#include "../Settings.h"
#include "../SDK/IGameEvent.h"
#include "../Utils/Utils.h"
#include "../SDK/IVEngineClient.h"
#include "../SDK/PlayerInfo.h"
#include "../SDK/Definitions.h"

class EventLogger
{
private:
    char* HitgroupToName(int hitgroup)
    {
        switch (hitgroup)
        {
        case (int)HitBox::HITBOX_HEAD:
        case (int)HitBox::HITBOX_NECK:
            return "head";
        case (int)HitBox::HITBOX_LEFT_FOOT:
        case (int)HitBox::HITBOX_RIGHT_FOOT:
            return "leg";
        case (int)HitBox::HITBOX_BELLY:
            return "belly";
        case (int)HitBox::HITBOX_THORAX:
            return "thorax";
        default:
            return "body";
        }
    }

    
public:
    void Run(IGameEvent* event)
    {
        if (!strcmp(event->GetName(), "player_hurt"))
        {
            int iAttacker = g_pEngine->GetPlayerForUserID(event->GetInt("attacker"));
            if (!iAttacker)
                return;
            int iVictim = g_pEngine->GetPlayerForUserID(event->GetInt("userid"));
            if (!iVictim)
                return;
            if (iAttacker == g_pEngine->GetLocalPlayer() && iVictim != g_pEngine->GetLocalPlayer())
            {
                auto pVictim = g_pEntityList->GetClientEntity(iVictim);

                PlayerInfo_t playerinfo;
                if (!g_pEngine->GetPlayerInfo(iVictim, &playerinfo))
                    return;

                std::stringstream stream;
                stream << "[15] Hit " << playerinfo.szName << " in the " << HitgroupToName(event->GetInt("hitgroup")) << " for " << event->GetInt("dmg_health") << " damage ( " << event->GetInt("health") << " health remaining )\n";
                std::string string = stream.str();
                Utils::PrintMessageToGameConsole(Color::Blue(), string.c_str());
            }
        }
        else if (!strcmp(event->GetName(), "item_purchase"))
        {
            auto userid = event->GetInt("userid");
            if (!userid)
                return;

            auto userid_id = g_pEngine->GetPlayerForUserID(userid);
            if (!userid_id)
                return;

            PlayerInfo_t playerinfo;
            if (!g_pEngine->GetPlayerInfo(userid_id, &playerinfo))
                return;

            auto pLocalPlayer = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
            if (!pLocalPlayer)
                return;
            if (userid == g_pEngine->GetLocalPlayer() || event->GetInt("team") == pLocalPlayer->GetTeam())
                return;

            std::stringstream stream;

            stream << "[15] " << playerinfo.szName << " bought " << event->GetString("weapon") << "\n";
            std::string string = stream.str();

            Utils::PrintMessageToGameConsole(Color::Blue(), string.c_str());
        }
        else if (!strcmp(event->GetName(), "bomb_beginplant"))
        {
            /*auto userid = event->GetInt("userid");
            if (!userid)
                return;

            auto userid_id = g_pEngine->GetPlayerForUserID(userid);

            PlayerInfo_t playerinfo;
            if (!g_pEngine->GetPlayerInfo(userid_id, &playerinfo))
                return;

            auto ent = g_pEntityList->GetClientEntity(userid);
            if (!ent)
                return;

            auto bombpos = ent->GetOrigin();

            if (bombpos.Length() < 0.1f)
                return;

            bool bIsASite = (g_pPlayerResource->GetBombsiteA() - bombpos).Length() < (g_pPlayerResource->GetBombsiteB() - bombpos).Length();

            std::stringstream stream;
            stream << playerinfo.szName << " is beginning to plant the boom at " << bIsASite ? " A site \n" : " B site \n";
            std::string string = stream.str();
            Utils::PrintMessageToGameConsole(string.c_str());*/
        }
        else if (!strcmp(event->GetName(), "bomb_begindefuse"))
        {
            auto userid = event->GetInt("userid");
            if (!userid)
                return;

            auto userid_id = g_pEngine->GetPlayerForUserID(userid);

            PlayerInfo_t playerinfo;
            if (!g_pEngine->GetPlayerInfo(userid_id, &playerinfo))
                return;

            std::stringstream stream;
            stream << "[15] " << playerinfo.szName << " is beginning to defuse the boom " << event->GetBool("haskit") ? " with the kit \n" : " without the kit \n";
            std::string string = stream.str();
            Utils::PrintMessageToGameConsole(Color::Blue(), string.c_str());
        }
    }

};

extern EventLogger g_EventLogger;
