#pragma once
#include <filesystem>

#include "SDK/Json/Json.h"
#include "SDK/Color.h"

struct ColorP
{
public:
    const char* Name;
    float* Ccolor;

    ColorP(const char* name, float* color)
    {
        this->Name = name;
        this->Ccolor = color;
    }
};

namespace fs = std::experimental::filesystem;

class Configs
{
public:

    void SaveSettings(const std::string& strFileName);
    void LoadSettings(const std::string& strFileName);

private:
    nlohmann::json config;
private:
    template <class T>
    void GetVal(const nlohmann::json& config, T* setting)
    {
        if (config.is_null())
            return;
        try
        {
            *setting = config;
        } catch (...)
        {
            return;
        }
    }
    void GetVal(const nlohmann::json& config, Color* setting)
    {
        if (config.is_null())
            return;

        GetVal(config["r"], &setting->red);
        GetVal(config["g"], &setting->green);
        GetVal(config["b"], &setting->blue);
        GetVal(config["a"], &setting->alpha);
    }

    void GetVal(const nlohmann::json& config, float* setting)
    {
        if (config.is_null())
            return;

        auto buffer = ImColor(setting[0], setting[1], setting[2]);

        buffer.Value.x = config["r"];
        buffer.Value.y = config["g"];
        buffer.Value.z = config["b"];

        setting[0] = buffer.Value.x;
        setting[1] = buffer.Value.y;
        setting[2] = buffer.Value.z;
    }

    void SaveColor(nlohmann::json& config, const Color& color)
    {
        config["r"] = color.red;
        config["g"] = color.red;
        config["b"] = color.red;
        config["a"] = color.red;
    }

    void SaveColor(nlohmann::json& config, const float* color)
    {
        config["r"] = color[0];
        config["g"] = color[1];
        config["b"] = color[2];
    }

};

struct Settings
{
    struct
    {
        bool  bCheatActive = true;
        bool  bMenuOpened = false;
    }Cheat;

    struct
    {
        struct wepconfigs
        {
            bool bShouldAimbot = false;
            int iHitbox = 0;
            float flFov = 0.1f;
            float flRCS_Y = 1.0f;
            float flRCS_X = 1.0f;
            float flSmooth = 1.0f;
        };

        wepconfigs WeaponConfig[33];

        bool bBacktrack = false;
        int  iBacktrackTick = 1;

    }LegitBot;

    struct
    {
        bool bEnabled = false;
        bool bAutoFire = false;
        float flHitChance = 0.0;
    }RageBot;

    struct
    {
        struct 
        {
            bool bFriends;
            bool bSmoke;
        }Filter;

        struct
        {
            bool  bGlows = false;
            bool  bShowSkeleton = false;
            bool  bShowBoxes = false;
            bool  bShowNames = false;
            bool  bShowWeapons = false;
            bool  bShowHealth = false;
            bool  bShowArmor = false;
            bool  bShowMoneny = false;
            bool  bShowIfIsScoped = false;
        }Type;

        struct
        {
            bool bNadePrediction = false;
            bool bNightMode = false;
            bool bDrawGrey = false;
            bool bFullbright = false;
            bool bShowLowResImage = false;
            bool bShowMipLevels = false;
        }Others;

        struct
        {
            bool bEnabled = false;
            bool bPlayerIgnorez = false;
            bool bLocalPlayer = false;
            bool bArms = false;

            enum Type
            {
                FLAT = 0, GLASS = 1, GLOSS = 2
            };

            Type nPlayerType = FLAT;
            Type nLocalPlayerType = FLAT;
            Type nArmsType = FLAT;

        }Chams;

    }Visuals;

    struct
    {
        bool bAutobhop = false;
        bool bAutoStrafe = false;
        bool bGroundStrafe = false;
        bool bStreamProof = false;
        bool bFakeLatency = false;
        bool bAntiVoteKick = false;
        bool bThirdPerson = false;
        float flFakeLatency = 100.f;
        float flFov = 0.0f;
    }Miscellaneous;

    struct
    {
        // Menu Colors
        float flColMenuText[3] = { 0.07058823529f, 0.07058823529f, 0.07058823529f };
        float flColMenuTheme[3] = { 0.54117647058f, 0.16470588235f, 0.8862745098f };
        float flColMenuBackground[3] = { 0.97647058823f, 0.97647058823f, 0.97647058823f };
        float flColMenuBorder[3] = { 0.14509803921f, 0.14509803921f, 0.14509803921f };

        // Hake Colors
        float flGlowTeam[3] = { 0.f, 0.f, 0.f };
        float flGlowEnemy[3] = { 0.f, 0.f, 0.f };

        float flChamsVisibleTeam[3] = { 0.f, 0.f, 0.f };
        float flChamsVisibleEnemy[3] = { 0.f, 0.f, 0.f };
        float flChamsOccludedTeam[3] = { 0.f, 0.f, 0.f };
        float flChamsOccludedEnemy[3] = { 0.f, 0.f, 0.f };
        float flChamsLocalPlayer[3] = { 1.f, 1.f, 1.f };

        float flTextTeam[3] = { 1.f, 1.f, 1.f };
        float flTextEnemy[3] = { 1.f, 1.f, 1.f };

        float flBoxTeam[3] = { 1.f, 1.f, 1.f };
        float flBoxEnemy[3] = { 1.f, 1.f, 1.f };

        float flSkeletonTeam[3] = { 1.f, 1.f, 1.f };
        float flSkeletonEnemy[3] = { 1.f, 1.f, 1.f };

        float flNadePrediction[3] = { 0.1019607843137255f, 0.407843137254902f, 0.6784313725490196f };

        std::vector<ColorP> ColorPicker;
    }Color;
};

extern Configs g_Configs;
extern Settings g_Settings;


