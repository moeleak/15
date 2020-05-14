#include <functional>
#include <ShlObj.h>
#include <fstream>

#include "Settings.h"
#include "Hooks.h"

void Configs::SaveSettings(const std::string& strFileName)
{
    VM_BEGIN("SAVE_SETTINGS")
    std::ofstream o(strFileName + ".cfg");

    config[XORSTR("Legitbox")][XORSTR("Backtrack")][XORSTR("Enabled")] = g_Settings.LegitBot.bBacktrack;

    config[XORSTR("Visuals")][XORSTR("Type")][XORSTR("Glows")][XORSTR("Enabled")] = g_Settings.Visuals.Type.bGlows;
    config[XORSTR("Visuals")][XORSTR("Type")][XORSTR("Box")][XORSTR("Enabled")] = g_Settings.Visuals.Type.bShowBoxes;
    config[XORSTR("Visuals")][XORSTR("Type")][XORSTR("Health")][XORSTR("Enabled")] = g_Settings.Visuals.Type.bShowHealth;
    config[XORSTR("Visuals")][XORSTR("Type")][XORSTR("Name")][XORSTR("Enabled")] = g_Settings.Visuals.Type.bShowNames;
    config[XORSTR("Visuals")][XORSTR("Type")][XORSTR("Money")][XORSTR("Enabled")] = g_Settings.Visuals.Type.bShowMoneny;
    config[XORSTR("Visuals")][XORSTR("Type")][XORSTR("Weapons")][XORSTR("Enabled")] = g_Settings.Visuals.Type.bShowWeapons;
    config[XORSTR("Visuals")][XORSTR("Type")][XORSTR("Scope")][XORSTR("Enabled")] = g_Settings.Visuals.Type.bShowIfIsScoped;
    config[XORSTR("Visuals")][XORSTR("Type")][XORSTR("Skeleton")][XORSTR("Enabled")] = g_Settings.Visuals.Type.bShowSkeleton;
    config[XORSTR("Visuals")][XORSTR("Filter")][XORSTR("Team")][XORSTR("Enabled")] = g_Settings.Visuals.Filter.bFriends;
    config[XORSTR("Visuals")][XORSTR("Others")][XORSTR("NadePrediction")][XORSTR("Enabled")] = g_Settings.Visuals.Others.bNadePrediction;
    config[XORSTR("Visuals")][XORSTR("Others")][XORSTR("NightMode")][XORSTR("Enabled")] = g_Settings.Visuals.Others.bNightMode;
    config[XORSTR("Visuals")][XORSTR("Others")][XORSTR("GreyWorld")][XORSTR("Enabled")] = g_Settings.Visuals.Others.bDrawGrey;
    config[XORSTR("Visuals")][XORSTR("Others")][XORSTR("FullBright")][XORSTR("Enabled")] = g_Settings.Visuals.Others.bFullbright;
    config[XORSTR("Visuals")][XORSTR("Others")][XORSTR("PixelWorld")][XORSTR("Enabled")] = g_Settings.Visuals.Others.bShowLowResImage;
    config[XORSTR("Visuals")][XORSTR("Others")][XORSTR("MLGWorld")][XORSTR("Enabled")] = g_Settings.Visuals.Others.bShowMipLevels;

    config[XORSTR("Visuals")][XORSTR("Chams")][XORSTR("Enabled")] = g_Settings.Visuals.Chams.bEnabled;
    config[XORSTR("Visuals")][XORSTR("Chams")][XORSTR("PlayerIgnorez")] = g_Settings.Visuals.Chams.bPlayerIgnorez;
    config[XORSTR("Visuals")][XORSTR("Chams")][XORSTR("LocalPlayer")] = g_Settings.Visuals.Chams.bLocalPlayer;
    config[XORSTR("Visuals")][XORSTR("Chams")][XORSTR("Arms")] = g_Settings.Visuals.Chams.bArms;
    config[XORSTR("Visuals")][XORSTR("Chams")][XORSTR("Arms")][XORSTR("Type")] = g_Settings.Visuals.Chams.nArmsType;
    config[XORSTR("Visuals")][XORSTR("Chams")][XORSTR("Player")][XORSTR("Type")] = g_Settings.Visuals.Chams.nPlayerType;
    config[XORSTR("Visuals")][XORSTR("Chams")][XORSTR("LocalPlayer")][XORSTR("Type")] = g_Settings.Visuals.Chams.nLocalPlayerType;

    config[XORSTR("Misc")][XORSTR("AutoBunnyhop")][XORSTR("Enabled")] = g_Settings.Miscellaneous.bAutobhop;
    config[XORSTR("Misc")][XORSTR("AutoStrafe")][XORSTR("Enabled")] = g_Settings.Miscellaneous.bAutoStrafe;
    config[XORSTR("Misc")][XORSTR("GroundStrafe")][XORSTR("Enabled")] = g_Settings.Miscellaneous.bGroundStrafe;
    config[XORSTR("Misc")][XORSTR("StreamProof")][XORSTR("Enabled")] = g_Settings.Miscellaneous.bStreamProof;
    config[XORSTR("Misc")][XORSTR("FakeLatency")][XORSTR("Enabled")] = g_Settings.Miscellaneous.bFakeLatency;
    config[XORSTR("Misc")][XORSTR("FakeLatency")][XORSTR("Value")] = g_Settings.Miscellaneous.flFakeLatency;
    config[XORSTR("Misc")][XORSTR("Fov")][XORSTR("Value")] = g_Settings.Miscellaneous.flFov;

    SaveColor(config[XORSTR("Colors"), XORSTR("NadePrediction")], g_Settings.Color.flNadePrediction);
    SaveColor(config[XORSTR("Colors"), XORSTR("BoxTeam")], g_Settings.Color.flBoxTeam);
    SaveColor(config[XORSTR("Colors"), XORSTR("BoxEnemy")], g_Settings.Color.flBoxEnemy);
    SaveColor(config[XORSTR("Colors"), XORSTR("TextTeam")], g_Settings.Color.flTextTeam);
    SaveColor(config[XORSTR("Colors"), XORSTR("TextEnemy")], g_Settings.Color.flTextEnemy);
    SaveColor(config[XORSTR("Colors"), XORSTR("GlowTeam")], g_Settings.Color.flGlowTeam);
    SaveColor(config[XORSTR("Colors"), XORSTR("GlowEnemy")], g_Settings.Color.flGlowEnemy);
    SaveColor(config[XORSTR("Colors"), XORSTR("SkeletonTeam")], g_Settings.Color.flSkeletonTeam);
    SaveColor(config[XORSTR("Colors"), XORSTR("SkeletonEnemy")], g_Settings.Color.flSkeletonEnemy);
    SaveColor(config[XORSTR("Colors"), XORSTR("Chams"), XORSTR("VisibleTeam")], g_Settings.Color.flChamsVisibleTeam);
    SaveColor(config[XORSTR("Colors"), XORSTR("Chams"), XORSTR("VisibleEnemy")], g_Settings.Color.flChamsVisibleEnemy);
    SaveColor(config[XORSTR("Colors"), XORSTR("Chams"), XORSTR("OccludedTeam")], g_Settings.Color.flChamsOccludedTeam);
    SaveColor(config[XORSTR("Colors"), XORSTR("Chams"), XORSTR("OccludedEnemy")], g_Settings.Color.flChamsOccludedEnemy);

    o << std::setw(4) << config << std::endl;
    VM_END
}

void Configs::LoadSettings(const std::string& strFileName)
{
    VM_BEGIN("LOAD_SETTINGS")
    std::ifstream i(strFileName + ".cfg");

    if (!i.good())
        return;

    i >> config;
    GetVal(config[XORSTR("Legitbox")][XORSTR("Backtrack")][XORSTR("Enabled")], &g_Settings.LegitBot.bBacktrack);

    GetVal(config[XORSTR("Visuals")][XORSTR("Type")][XORSTR("Glows")][XORSTR("Enabled")], &g_Settings.Visuals.Type.bGlows);
    GetVal(config[XORSTR("Visuals")][XORSTR("Type")][XORSTR("Box")][XORSTR("Enabled")], &g_Settings.Visuals.Type.bShowBoxes);
    GetVal(config[XORSTR("Visuals")][XORSTR("Type")][XORSTR("Health")][XORSTR("Enabled")], &g_Settings.Visuals.Type.bShowHealth);
    GetVal(config[XORSTR("Visuals")][XORSTR("Type")][XORSTR("Name")][XORSTR("Enabled")], &g_Settings.Visuals.Type.bShowNames);
    GetVal(config[XORSTR("Visuals")][XORSTR("Type")][XORSTR("Money")][XORSTR("Enabled")], &g_Settings.Visuals.Type.bShowMoneny);
    GetVal(config[XORSTR("Visuals")][XORSTR("Type")][XORSTR("Weapons")][XORSTR("Enabled")], &g_Settings.Visuals.Type.bShowWeapons);
    GetVal(config[XORSTR("Visuals")][XORSTR("Type")][XORSTR("Scope")][XORSTR("Enabled")], &g_Settings.Visuals.Type.bShowIfIsScoped);
    GetVal(config[XORSTR("Visuals")][XORSTR("Type")][XORSTR("Skeleton")][XORSTR("Enabled")], &g_Settings.Visuals.Type.bShowSkeleton);
    GetVal(config[XORSTR("Visuals")][XORSTR("Filter")][XORSTR("Team")][XORSTR("Enabled")], &g_Settings.Visuals.Filter.bFriends);
    GetVal(config[XORSTR("Visuals")][XORSTR("Others")][XORSTR("NadePrediction")][XORSTR("Enabled")], &g_Settings.Visuals.Others.bNadePrediction);
    GetVal(config[XORSTR("Visuals")][XORSTR("Others")][XORSTR("NightMode")][XORSTR("Enabled")], &g_Settings.Visuals.Others.bNightMode);
    GetVal(config[XORSTR("Visuals")][XORSTR("Others")][XORSTR("GreyWorld")][XORSTR("Enabled")], &g_Settings.Visuals.Others.bDrawGrey);
    GetVal(config[XORSTR("Visuals")][XORSTR("Others")][XORSTR("FullBright")][XORSTR("Enabled")], &g_Settings.Visuals.Others.bFullbright);
    GetVal(config[XORSTR("Visuals")][XORSTR("Others")][XORSTR("PixelWorld")][XORSTR("Enabled")], &g_Settings.Visuals.Others.bShowLowResImage);
    GetVal(config[XORSTR("Visuals")][XORSTR("Others")][XORSTR("MLGWorld")][XORSTR("Enabled")], &g_Settings.Visuals.Others.bShowMipLevels);


    GetVal(config[XORSTR("Visuals")][XORSTR("Chams")][XORSTR("Enabled")], &g_Settings.Visuals.Chams.bEnabled);
    GetVal(config[XORSTR("Visuals")][XORSTR("Chams")][XORSTR("PlayerIgnorez")], &g_Settings.Visuals.Chams.bPlayerIgnorez);
    GetVal(config[XORSTR("Visuals")][XORSTR("Chams")][XORSTR("LocalPlayer")], &g_Settings.Visuals.Chams.bLocalPlayer);
    GetVal(config[XORSTR("Visuals")][XORSTR("Chams")][XORSTR("Arms")], &g_Settings.Visuals.Chams.bArms);
    GetVal(config[XORSTR("Visuals")][XORSTR("Chams")][XORSTR("Arms")][XORSTR("Type")], &g_Settings.Visuals.Chams.nArmsType);
    GetVal(config[XORSTR("Visuals")][XORSTR("Chams")][XORSTR("Player")][XORSTR("Type")], &g_Settings.Visuals.Chams.nPlayerType);
    GetVal(config[XORSTR("Visuals")][XORSTR("Chams")][XORSTR("LocalPlayer")][XORSTR("Type")], &g_Settings.Visuals.Chams.nLocalPlayerType);


    GetVal(config[XORSTR("Misc")][XORSTR("AutoBunnyhop")][XORSTR("Enabled")], &g_Settings.Miscellaneous.bAutobhop);
    GetVal(config[XORSTR("Misc")][XORSTR("AutoStrafe")][XORSTR("Enabled")], &g_Settings.Miscellaneous.bAutoStrafe);
    GetVal(config[XORSTR("Misc")][XORSTR("GroundStrafe")][XORSTR("Enabled")], &g_Settings.Miscellaneous.bGroundStrafe);
    GetVal(config[XORSTR("Misc")][XORSTR("StreamProof")][XORSTR("Enabled")], &g_Settings.Miscellaneous.bStreamProof);
    GetVal(config[XORSTR("Misc")][XORSTR("FakeLatency")][XORSTR("Enabled")], &g_Settings.Miscellaneous.bFakeLatency);
    GetVal(config[XORSTR("Misc")][XORSTR("FakeLatency")][XORSTR("Value")], &g_Settings.Miscellaneous.flFakeLatency);
    GetVal(config[XORSTR("Misc")][XORSTR("Fov")][XORSTR("Value")], &g_Settings.Miscellaneous.flFov);

    GetVal(config[XORSTR("Colors"), XORSTR("NadePrediction")], g_Settings.Color.flNadePrediction);
    GetVal(config[XORSTR("Colors"), XORSTR("BoxTeam")], g_Settings.Color.flBoxTeam);
    GetVal(config[XORSTR("Colors"), XORSTR("BoxEnemy")], g_Settings.Color.flBoxEnemy);
    GetVal(config[XORSTR("Colors"), XORSTR("TextTeam")], g_Settings.Color.flTextTeam);
    GetVal(config[XORSTR("Colors"), XORSTR("TextEnemy")], g_Settings.Color.flTextEnemy);
    GetVal(config[XORSTR("Colors"), XORSTR("GlowTeam")], g_Settings.Color.flGlowTeam);
    GetVal(config[XORSTR("Colors"), XORSTR("GlowEnemy")], g_Settings.Color.flGlowEnemy);
    GetVal(config[XORSTR("Colors"), XORSTR("SkeletonTeam")], g_Settings.Color.flSkeletonTeam);
    GetVal(config[XORSTR("Colors"), XORSTR("SkeletonEnemy")], g_Settings.Color.flSkeletonEnemy);
    GetVal(config[XORSTR("Colors"), XORSTR("Chams"), XORSTR("VisibleTeam")], g_Settings.Color.flChamsVisibleTeam);
    GetVal(config[XORSTR("Colors"), XORSTR("Chams"), XORSTR("VisibleEnemy")], g_Settings.Color.flChamsVisibleEnemy);
    GetVal(config[XORSTR("Colors"), XORSTR("Chams"), XORSTR("OccludedTeam")], g_Settings.Color.flChamsOccludedTeam);
    GetVal(config[XORSTR("Colors"), XORSTR("Chams"), XORSTR("OccludedEnemy")], g_Settings.Color.flChamsOccludedEnemy);
    VM_END
}
