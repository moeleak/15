#include <thread>
#include <intrin.h>

#include "Hooks.h"
#include "Utils/Utils.h"
#include "Utils/Xorstr.h"
#include "Features/Features.h"
#include "SDK/SDK.h"
#include "SDK/Imgui/imgui.h"
#include "SDK/Imgui/imgui_impl_dx9.h"
#include "SDK/Imgui/imgui_internal.h"

Configs  g_Configs;
Hooks    g_Hooks;
Settings g_Settings;

ESP      g_ESP;
Chams    g_Chams;
Glow     g_Glow;
Backtrack g_Backtrack;
EventLogger g_EventLogger;

extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); // Im the shit

void Hooks::Init()
{
    VM_BEGIN("HOOK");
    // Get window handle
    while (!(g_Hooks.hCSGOWindow = FindWindowA(XORSTR("Valve001"), nullptr)))
    {
        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(50ms);
    }

    interfaces::Init();                         // Get interfaces

    Utils::Log(XORSTR("Hooking in progress..."));


    // D3D Device pointer
    const uintptr_t d3dDevice = **reinterpret_cast<uintptr_t**>(Utils::FindSignature(XORSTR("shaderapidx9.dll"), XORSTR("A1 ? ? ? ? 50 8B 08 FF 51 0C")) + 1);

    if (g_Hooks.hCSGOWindow)        // Hook WNDProc to capture mouse / keyboard input
        g_Hooks.pOriginalWNDProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(g_Hooks.hCSGOWindow, GWLP_WNDPROC,
            reinterpret_cast<LONG_PTR>(g_Hooks.WndProc)));

    // Functions without VMT
    g_Hooks.pStreamPresent = Utils::FindSignature(XORSTR("gameoverlayrenderer.dll"), XORSTR("FF 15 ? ? ? ? 8B F8 85 DB")) + 2;
    g_Hooks.pStreamReset = Utils::FindSignature(XORSTR("gameoverlayrenderer.dll"), XORSTR("FF 15 ? ? ? ? 8B F8 85 FF 78 18")) + 2;

    g_Hooks.oStreamPresent = **reinterpret_cast<decltype(&g_Hooks.oStreamPresent)*>(g_Hooks.pStreamPresent);
    g_Hooks.oStreamReset = **reinterpret_cast<decltype(&g_Hooks.oStreamReset)*>(g_Hooks.pStreamReset);

    **reinterpret_cast<void***>(g_Hooks.pStreamPresent) = reinterpret_cast<void*>(&g_Hooks.StreamPresent);
    **reinterpret_cast<void***>(g_Hooks.pStreamReset) = reinterpret_cast<void*>(&g_Hooks.StreamReset);

    // VMTHooks
    g_Hooks.pD3DDevice9Hook = std::make_unique<VMTHook>(reinterpret_cast<void*>(d3dDevice));
    g_Hooks.pClientModeHook = std::make_unique<VMTHook>(g_pClientMode);
    g_Hooks.pClientHook = std::make_unique<VMTHook>(g_pClientDll);
    g_Hooks.pRenderViewHook = std::make_unique<VMTHook>(g_pRenderView);
    g_Hooks.pSteamGameCoordinator = std::make_unique<VMTHook>(g_pSteamGameCoordinator);
    g_Hooks.pMatRenderHook = std::make_unique<VMTHook>(g_pMdlRender);
    g_Hooks.pVPanelHook = std::make_unique<VMTHook>(g_pVPanel);
    g_Hooks.pMaterialSystemHook = std::make_unique<VMTHook>(g_pMaterialSystem);
    /*auto sv_cheats = g_pCVar->FindVar("sv_cheats");
    if(sv_cheats)
        g_Hooks.pConVarsHook = std::make_unique<VMTHook>(sv_cheats);*/

    // Hook the table functions
    g_Hooks.pClientHook->Hook(vtable_indexes::framestagenotify, Hooks::FrameStageNotify);
    g_Hooks.pClientHook->Hook(vtable_indexes::dispatchusermessage, Hooks::DispatchUserMessage);
    g_Hooks.pD3DDevice9Hook->Hook(vtable_indexes::reset, Hooks::Reset);
    g_Hooks.pD3DDevice9Hook->Hook(vtable_indexes::endscene, Hooks::EndScene);
    g_Hooks.pD3DDevice9Hook->Hook(vtable_indexes::present, Hooks::Present);
    g_Hooks.pClientModeHook->Hook(vtable_indexes::createmove, Hooks::CreateMove);
    g_Hooks.pClientModeHook->Hook(vtable_indexes::overridemouseinput, Hooks::OverrideMouseInput);
    g_Hooks.pClientModeHook->Hook(vtable_indexes::overrideview, Hooks::OverrideView);
    g_Hooks.pRenderViewHook->Hook(vtable_indexes::sceneend, Hooks::SceneEnd);
    g_Hooks.pMatRenderHook->Hook(vtable_indexes::drawmodelexecute, Hooks::DrawModelExecute);
    g_Hooks.pVPanelHook->Hook(vtable_indexes::painttraverse, Hooks::PaintTraverse);
    g_Hooks.pMaterialSystemHook->Hook(vtable_indexes::overrideconfig, Hooks::OverrideConfig);
    //g_Hooks.pConVarsHook->Hook(vtable_indexes::getbool, Hooks::SvCheats_GetBool);   

    //g_Hooks.oGetBool = g_Hooks.pConVarsHook->GetOriginal<SvCheatsGetBool_t>(vtable_indexes::getbool);

    //For Steam
    g_Hooks.pSteamGameCoordinator->Hook(vtable_indexes::sendmsg, Hooks::SendMsg);
    g_Hooks.pSteamGameCoordinator->Hook(vtable_indexes::retrievemsg, Hooks::RetrieveMsg);

    // Create event listener
    const std::vector<const char*> vecEventNames = 
    { XORSTR("item_purchase"),
      XORSTR("player_hurt"),
      XORSTR("bomb_beginplant"),
      XORSTR("bomb_begindefuse"),
      XORSTR("bullet_impact")
    };
    g_Hooks.pEventListener = std::make_unique<EventListener>(vecEventNames);

    Utils::Log(XORSTR("Hooking completed!"));

    //if (!InventoryChanger::SendClientHello());

    //g_pCVar->ConsoleColorPrintf(Color(255, 255, 255, 255), "[15] Injected successfully.");
    VM_END
}

void Hooks::Restore()
{
    VM_BEGIN("UNHOOK");
    Utils::Log(XORSTR("Enabling mouse pointer."));
    g_pEngine->ExecuteClientCmd(XORSTR("cl_mouseenable 1")); //Renable the mouse after exit

    Utils::Log(XORSTR("Unhooking in progress..."));
    {   // Unhook every function we hooked and restore original one
        g_Hooks.pD3DDevice9Hook->Unhook(vtable_indexes::reset);
        g_Hooks.pD3DDevice9Hook->Unhook(vtable_indexes::endscene);
        g_Hooks.pD3DDevice9Hook->Unhook(vtable_indexes::present);
        g_Hooks.pClientHook->Unhook(vtable_indexes::framestagenotify);
        g_Hooks.pClientModeHook->Unhook(vtable_indexes::createmove);
        g_Hooks.pClientModeHook->Unhook(vtable_indexes::overridemouseinput);
        g_Hooks.pClientModeHook->Unhook(vtable_indexes::overrideview);
        g_Hooks.pRenderViewHook->Unhook(vtable_indexes::sceneend);
        g_Hooks.pSteamGameCoordinator->Unhook(vtable_indexes::sendmsg);
        g_Hooks.pSteamGameCoordinator->Unhook(vtable_indexes::retrievemsg);
        g_Hooks.pVPanelHook->Unhook(vtable_indexes::painttraverse);
        g_Hooks.pMatRenderHook->Unhook(vtable_indexes::drawmodelexecute);
        if (g_Hooks.pNetChannelHook)
            g_Hooks.pNetChannelHook->Unhook(vtable_indexes::senddatagram);

        g_Hooks.pMaterialSystemHook->Unhook(vtable_indexes::overrideconfig);

        **reinterpret_cast<void***>(g_Hooks.pStreamPresent) = g_Hooks.oStreamPresent;
        **reinterpret_cast<void***>(g_Hooks.pStreamReset) = g_Hooks.oStreamReset;

        SetWindowLongPtr(g_Hooks.hCSGOWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(g_Hooks.pOriginalWNDProc));
    }
    Utils::Log(XORSTR("Unhooking succeded!"));

    // Destroy fonts and all textures we created
    g_Render.InvalidateDeviceObjects();
    ImGui_ImplDX9_InvalidateDeviceObjects();
    g_Fonts.DeleteDeviceObjects();
    VM_END
}

void MovementFix(CUserCmd *usercmd, QAngle &wish_angle)
{
    Vector view_fwd, view_right, view_up, cmd_fwd, cmd_right, cmd_up;
    auto viewangles = usercmd->viewangles;
    Math::NormalizeAngles(viewangles);

    Math::AngleVectors(wish_angle, &view_fwd, &view_right, &view_up);
    Math::AngleVectors(viewangles, &cmd_fwd, &cmd_right, &cmd_up);

    const float v8 = sqrtf((view_fwd.x * view_fwd.x) + (view_fwd.y * view_fwd.y));
    const float v10 = sqrtf((view_right.x * view_right.x) + (view_right.y * view_right.y));
    const float v12 = sqrtf(view_up.z * view_up.z);

    const Vector norm_view_fwd((1.f / v8) * view_fwd.x, (1.f / v8) * view_fwd.y, 0.f);
    const Vector norm_view_right((1.f / v10) * view_right.x, (1.f / v10) * view_right.y, 0.f);
    const Vector norm_view_up(0.f, 0.f, (1.f / v12) * view_up.z);

    const float v14 = sqrtf((cmd_fwd.x * cmd_fwd.x) + (cmd_fwd.y * cmd_fwd.y));
    const float v16 = sqrtf((cmd_right.x * cmd_right.x) + (cmd_right.y * cmd_right.y));
    const float v18 = sqrtf(cmd_up.z * cmd_up.z);

    const Vector norm_cmd_fwd((1.f / v14) * cmd_fwd.x, (1.f / v14) * cmd_fwd.y, 0.f);
    const Vector norm_cmd_right((1.f / v16) * cmd_right.x, (1.f / v16) * cmd_right.y, 0.f);
    const Vector norm_cmd_up(0.f, 0.f, (1.f / v18) * cmd_up.z);

    const float v22 = norm_view_fwd.x * usercmd->forwardmove;
    const float v26 = norm_view_fwd.y * usercmd->forwardmove;
    const float v28 = norm_view_fwd.z * usercmd->forwardmove;
    const float v24 = norm_view_right.x * usercmd->sidemove;
    const float v23 = norm_view_right.y * usercmd->sidemove;
    const float v25 = norm_view_right.z * usercmd->sidemove;
    const float v30 = norm_view_up.x * usercmd->upmove;
    const float v27 = norm_view_up.z * usercmd->upmove;
    const float v29 = norm_view_up.y * usercmd->upmove;

    usercmd->forwardmove = ((((norm_cmd_fwd.x * v24) + (norm_cmd_fwd.y * v23)) + (norm_cmd_fwd.z * v25))
        + (((norm_cmd_fwd.x * v22) + (norm_cmd_fwd.y * v26)) + (norm_cmd_fwd.z * v28)))
        + (((norm_cmd_fwd.y * v30) + (norm_cmd_fwd.x * v29)) + (norm_cmd_fwd.z * v27));
    usercmd->sidemove = ((((norm_cmd_right.x * v24) + (norm_cmd_right.y * v23)) + (norm_cmd_right.z * v25))
        + (((norm_cmd_right.x * v22) + (norm_cmd_right.y * v26)) + (norm_cmd_right.z * v28)))
        + (((norm_cmd_right.x * v29) + (norm_cmd_right.y * v30)) + (norm_cmd_right.z * v27));
    usercmd->upmove = ((((norm_cmd_up.x * v23) + (norm_cmd_up.y * v24)) + (norm_cmd_up.z * v25))
        + (((norm_cmd_up.x * v26) + (norm_cmd_up.y * v22)) + (norm_cmd_up.z * v28)))
        + (((norm_cmd_up.x * v30) + (norm_cmd_up.y * v29)) + (norm_cmd_up.z * v27));

    usercmd->forwardmove = std::clamp(usercmd->forwardmove, -450.f, 450.f);
    usercmd->sidemove = std::clamp(usercmd->sidemove, -450.f, 450.f);
    usercmd->upmove = std::clamp(usercmd->upmove, -320.f, 320.f);
    Math::ClampAngles(usercmd->viewangles);
}

bool __fastcall Hooks::CreateMove(IClientMode* thisptr, void* edx, float sample_frametime, CUserCmd* pCmd)
{
    VM_BEGIN("HOOKS_CREATEMOVE")
    // Call original createmove before we start screwing with it
    static auto oCreateMove = g_Hooks.pClientModeHook->GetOriginal<CreateMove_t>(vtable_indexes::createmove);
    oCreateMove(thisptr, edx, sample_frametime, pCmd);

    if (!pCmd || !pCmd->command_number)
        return oCreateMove;

    bool bSendPacket = true;

    // Get globals
    g::pCmd = pCmd;
    g::pLocalEntity = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
    if (!g::pLocalEntity)
        return false;

    for (auto i = 1; i <= 64; i++)
    {
        if (i == (short)ItemDefinitionIndex::WEAPON_FLASHBANG || i == (short)ItemDefinitionIndex::WEAPON_SMOKEGRENADE || i == (short)ItemDefinitionIndex::WEAPON_HEGRENADE
            || i == (short)ItemDefinitionIndex::WEAPON_MOLOTOV || i == (short)ItemDefinitionIndex::WEAPON_DECOY || i == (short)ItemDefinitionIndex::WEAPON_INCGRENADE
            || i == (short)ItemDefinitionIndex::WEAPON_TASER || i == (short)ItemDefinitionIndex::WEAPON_C4)
            continue;

        g::iWeaponId = i;
    }

    // run shit outside enginepred
    Bunnyhop::CreateMove(pCmd);
    NadePrediction::CreateMove(pCmd);

    if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
    {
        FakeLatency::UpdateIncomingSequences();
        if (g_Settings.Miscellaneous.bFakeLatency)
        {
            if (!g_Hooks.pNetChannelHook)
            {
                uintptr_t ClientState = *(uintptr_t*)g_pClientState;
                if (ClientState)
                {
                    INetChannel* NetChannel = *(INetChannel**)ClientState + 0x9C;
                    if (NetChannel)
                    {
                        g_Hooks.pNetChannelHook = std::make_unique<VMTHook>(reinterpret_cast<DWORD**>(g_pClientState->m_NetChannel));
                        g_Hooks.pNetChannelHook->Hook(vtable_indexes::senddatagram, Hooks::SendDatagram);
                        g_Hooks.oSendDatagram = g_Hooks.pNetChannelHook->GetOriginal<SendDatagram_t>(vtable_indexes::senddatagram);
                    }
                }
            }
        }
    }

    engine_prediction::RunEnginePred();
    {
        g_Backtrack.CreateMove();
    }
    engine_prediction::EndEnginePred();

    
    MovementFix(pCmd, pCmd->viewangles);
    
    if (!bSendPacket)
        g::RealAngle = pCmd->viewangles;
    
    uintptr_t* framePointer;
    __asm mov framePointer, ebp;
    *reinterpret_cast< bool* > (*framePointer - 0x1C) = bSendPacket;

    return false;
    VM_END
}


void  __fastcall Hooks::OverrideMouseInput(IClientMode* ecx, void* edx, float* x, float* y)
{
    VM_BEGIN("HOOKS_OVERRIDEMOUSEINPUT");
    static auto oOverrideMouseInput = g_Hooks.pClientModeHook->GetOriginal<OverrideMouseInput_t>(vtable_indexes::overridemouseinput);
    oOverrideMouseInput(ecx, edx, x, y);
    VM_END
}

void __fastcall Hooks::OverrideView(IClientMode* ecx, void* edx, CViewSetup* setup)
{
    VM_BEGIN("HOOKS_OVERRIDEVIEW");
    static auto oOverrideView = g_Hooks.pRenderViewHook->GetOriginal<OverrideView_t>(vtable_indexes::overrideview);

    setup->fov += g_Settings.Miscellaneous.flFov;

    oOverrideView(ecx, edx, setup);

    if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
    {
        NadePrediction::OverrideView(setup);
        ThirdPerson::OverrideView();
    }
    VM_END
}


void __fastcall Hooks::SceneEnd(void* ecx, void* edx)
{
    VM_BEGIN("HOOKS_SCENEEND")
    static auto oSceneEnd = g_Hooks.pRenderViewHook->GetOriginal<SceneEnd_t>(vtable_indexes::sceneend);
    
    if (!g::pLocalEntity || !g_pEngine->IsInGame() || !g_pEngine->IsConnected())
        return oSceneEnd(ecx, edx);

    oSceneEnd(ecx, edx);

    g_Glow.SceneEnd();
    NightMode::SceneEnd();
    g_Backtrack.SceneEnd();
    VM_END
}

void __stdcall Hooks::DrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
    VM_BEGIN("HOOKS_DRAWMODELEXECUTE")
    static auto oDrawModelExecute = g_Hooks.pMatRenderHook->GetOriginal<DrawModelExecute_t>(vtable_indexes::drawmodelexecute);
    
    g_Glow.context = ctx;
    g_Chams.DrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);

    oDrawModelExecute(g_pMdlRender, ctx, state, pInfo, pCustomBoneToWorld);
    g_pMdlRender->ForcedMaterialOverride(nullptr);
    VM_END
}

void __stdcall Hooks::FrameStageNotify(ClientFrameStage_t stage)
{
    static auto oFrameStageNotify = g_Hooks.pClientHook->GetOriginal<FrameStageNotify_t>(vtable_indexes::framestagenotify);
    
    if (!g_pClientState)
    {
        if (g_pClientState->m_NetChannel)
        {
            if (g_Hooks.pNetChannelHook)
            {
                g_pGlobalVars->Real_m_nInSequencenumber = 0;
                g_Hooks.pNetChannelHook->Clear();
                //g_Hooks.pNetChannelHook = nullptr;
                g_Hooks.oSendDatagram = NULL;
            }
        }
    }
    else
    {
        if (!g_pClientState->m_NetChannel)
        {
            if (g_Hooks.pNetChannelHook)
            {
                g_pGlobalVars->Real_m_nInSequencenumber = 0;
                g_Hooks.pNetChannelHook->Clear();
                //g_Hooks.pNetChannelHook = nullptr;
                g_Hooks.oSendDatagram = NULL;
            }
        }
    }

    if (g_pEngine->IsInGame() && g_pEngine->IsConnected())
    {
        if(g_Settings.LegitBot.bBacktrack)
            g_Backtrack.FrameUpdatePostEntityThink(stage);

        ThirdPerson::FrameStageNotify(stage);
    }
    oFrameStageNotify(g_pClientDll, stage);
}

static bool find_word(const std::string &text, const std::string &word)
{
    size_t pos = 0;

    while ((pos = text.substr(pos).find(word)) != std::string::npos)
    {
        if (!(isalpha(text[pos - 1])) || !(isalpha(text[pos + word.size() + 1])))
            return true;
    }
    return false;
}

static std::vector<std::string> blacklisted_words =
{
    "vac inc",
    "ow inc",
    "ow",
    "vac"
};


bool __fastcall Hooks::DispatchUserMessage(void* ecx, void* edx, int type, unsigned int a3, unsigned int length, const void *msg_data)
{
    VM_BEGIN("HOOKS_DISPATCHUSERMESSAGE");
    static auto oDispatchUserMessage = g_Hooks.pClientHook->GetOriginal<DispatchUserMessage_t>(vtable_indexes::dispatchusermessage);
    
   /*switch (type)
    {
    case CS_UM_SayText2:
        {
            1ata.SetOffset(1);

            int ent_idx = data.ReadByte();
            data.Skip(3);
            std::string msg_name = data.ReadString();
            std::string playerName = data.ReadString();
            std::string msg = data.ReadString();
            std::string location = data.ReadString();
            std::string unk0 = data.ReadString();
            bool textallchat = data.ReadBool();
            Utils::Log("msg_type:%d", type);
            Utils::Log("ent_idx:%d", ent_idx);
            Utils::Log("msg_name:%s", msg_name.c_str());
            Utils::Log("playername:%s", playerName.c_str());
            Utils::Log("msg:%s", msg.c_str());
            Utils::Log("location:%s", msg.c_str());
            Utils::Log("textallchat:%s", textallchat ? "true" : "false");
        }
        break;

    case CS_UM_VoteStart:

        bf_read data = bf_read(reinterpret_cast<unsigned int>(msg_data));

        int ent_idx = data.ReadByte();
        int vote_type = data.ReadByte();
        int team = data.ReadByte();

        Utils::Log("[ VoteStart ]vote_type: %i", vote_type);
        Utils::Log("[ VoteStart ]ent_idx: %i", ent_idx);
        Utils::Log("[ VoteStart ]team: %i", team);

        break;
    }*/

    return oDispatchUserMessage(ecx, type, a3, length, msg_data);
    VM_END
}

EGCResults __fastcall Hooks::SendMsg(void* ecx, void* edx, uint32_t unMsgType, const void* pubData, uint32_t cubData)
{
    VM_BEGIN("HOOKS_SENDMSG")
    static auto SendMsg = g_Hooks.pSteamGameCoordinator->GetOriginal<SendMessage_t>(vtable_indexes::sendmsg);
    EGCResults status = SendMsg(ecx, edx, unMsgType, pubData, cubData);

    uint32_t messageType = unMsgType & 0x7FFFFFFF;
    void* pubDataMutable = const_cast<void*>(pubData);

    bool sendMessage = true;

    sendMessage = InventoryChanger::PreSendMessage(unMsgType, pubDataMutable, cubData);

    if (!sendMessage)
        return k_EGCResultOK;

    if (status != k_EGCResultOK)
        return status;

    Utils::Log(XORSTR("[<-] Message sent from GC [%d]!"), messageType);

    return status;
    VM_END
}

EGCResults __fastcall Hooks::RetrieveMsg(void* ecx, void* edx, uint32_t *punMsgType, void *pubDest, uint32_t cubDest, uint32_t *pcubMsgSize)
{
    VM_BEGIN("HOOKS_RETRIEVEMSG")
    static auto RetrieveMessage = g_Hooks.pSteamGameCoordinator->GetOriginal<RetrieveMessage_t>(vtable_indexes::retrievemsg);
    EGCResults status = RetrieveMessage(ecx, edx, punMsgType, pubDest, cubDest, pcubMsgSize);

    uint32_t messageType = *punMsgType & 0x7FFFFFFF;

    if (status != k_EGCResultOK)
        return status;

    InventoryChanger::PostRetrieveMessage(punMsgType, pubDest, cubDest, pcubMsgSize);


    Utils::Log(XORSTR("[->] Message got from GC [%u]!"), messageType);

    return status;
    VM_END
}

HRESULT __stdcall Hooks::Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    VM_BEGIN("HOOKS_RESET")
    static auto oReset = g_Hooks.pD3DDevice9Hook->GetOriginal<Reset_t>(vtable_indexes::reset);

    if (g_Settings.Miscellaneous.bStreamProof)
        return oReset(pDevice, pPresentationParameters);


    if (g_Hooks.bInitializedDrawManager)
    {
        Utils::Log(XORSTR("Reseting draw manager."));

        g_Render.InvalidateDeviceObjects();
        ImGui_ImplDX9_InvalidateDeviceObjects();

        HRESULT hr = oReset(pDevice, pPresentationParameters);

        g_Render.RestoreDeviceObjects(pDevice);
        ImGui_ImplDX9_CreateDeviceObjects();

        Utils::Log(XORSTR("DrawManager reset succeded."));
        return hr;
    }


    return oReset(pDevice, pPresentationParameters);
    VM_END
}

HRESULT __stdcall Hooks::StreamReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    VM_BEGIN("HOOKS_STREAMRESET")
    if (!g_Settings.Miscellaneous.bStreamProof)
        return g_Hooks.oStreamReset(pDevice, pPresentationParameters);

    if (g_Hooks.bInitializedDrawManager)
    {
        Utils::Log(XORSTR("Reseting draw manager."));

        g_Render.InvalidateDeviceObjects();
        ImGui_ImplDX9_InvalidateDeviceObjects();

        HRESULT hr = g_Hooks.oStreamReset(pDevice, pPresentationParameters);

        g_Render.RestoreDeviceObjects(pDevice);
        ImGui_ImplDX9_CreateDeviceObjects();

        Utils::Log(XORSTR("DrawManager reset succeded."));
        return hr;
    }


    return g_Hooks.oStreamReset(pDevice, pPresentationParameters);
    VM_END
}


HRESULT __stdcall Hooks::EndScene(IDirect3DDevice9* pDevice)
{
    VM_BEGIN("HOOKS_ENDSCENE")
    static auto oEndScene = g_Hooks.pD3DDevice9Hook->GetOriginal<Endscene_t>(vtable_indexes::endscene);


    return oEndScene(pDevice);
    VM_END
}


HRESULT __stdcall Hooks::Present(IDirect3DDevice9* pDevice, const RECT* pSourceRect,
    const RECT*       pDestRect,
    HWND              hDestWindowOverride, const RGNDATA* pDirtyRegion)
{

    VM_BEGIN("HOOKS_PRESENT")
    static auto oPresent = g_Hooks.pD3DDevice9Hook->GetOriginal<Present_t>(vtable_indexes::present);

    if (g_Settings.Miscellaneous.bStreamProof)
        return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);

    if (!g_Hooks.bInitializedDrawManager)
    {
        Utils::Log(XORSTR("Initializing Draw manager"));

        g_Render.InitDeviceObjects(pDevice);
        g_Hooks.nMenu.Initialize(pDevice);

        g_Hooks.bInitializedDrawManager = true;
        Utils::Log(XORSTR("Draw manager initialized"));
    }
    else
    {
        g_Render.SetupRenderStates();
        g_Hooks.MouseEnableExecute(); // Handles in-game cursor
        ImGui::GetIO().MouseDrawCursor = g_Settings.Cheat.bMenuOpened;

        ImGui_ImplDX9_NewFrame();

        //std::string szWatermark = "Antario";
        //g_Render.String(8, 8, CD3DFONT_DROPSHADOW, Color(250, 150, 200, 180), g_Fonts.pFontSegoeUI8.get(),
          //              szWatermark.c_str());

        if (g_Settings.Cheat.bMenuOpened)
        {
            g_Hooks.nMenu.Render();
        }

        // Put your draw calls here
        g_ESP.Render();
        NadePrediction::Render();
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    }

    return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
    VM_END
}

HRESULT __stdcall Hooks::StreamPresent(IDirect3DDevice9* pDevice, const RECT* pSourceRect,
    const RECT* pDestRect,
    HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
{
    VM_BEGIN("HOOKS_STREAMPRESENT")
    if (!g_Settings.Miscellaneous.bStreamProof)
        return g_Hooks.oStreamPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);

    if (!g_Hooks.bInitializedDrawManager)
    {
        Utils::Log(XORSTR("Initializing Draw manager"));

        g_Render.InitDeviceObjects(pDevice);
        g_Hooks.nMenu.Initialize(pDevice);

        g_Hooks.bInitializedDrawManager = true;
        Utils::Log(XORSTR("Draw manager initialized"));
    }
    else
    {
        g_Render.SetupRenderStates();
        g_Hooks.MouseEnableExecute(); // Handles in-game cursor
        ImGui::GetIO().MouseDrawCursor = g_Settings.Cheat.bMenuOpened;

        ImGui_ImplDX9_NewFrame();

        //std::string szWatermark = "Antario";
        //g_Render.String(8, 8, CD3DFONT_DROPSHADOW, Color(250, 150, 200, 180), g_Fonts.pFontSegoeUI8.get(),
        //              szWatermark.c_str());

        if (g_Settings.Cheat.bMenuOpened)
        {
            g_Hooks.nMenu.Render();
        }

        // Put your draw calls here
        NadePrediction::Render();
        g_ESP.Render();
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    }

    return g_Hooks.oStreamPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}


LRESULT ImGui_ImplDX9_WndProcHandler(HWND, UINT msg, WPARAM wParam, LPARAM lParam)
{
    ImGuiIO& io = ImGui::GetIO();
    switch (msg) {
    case WM_LBUTTONDOWN:
        io.MouseDown[0] = true;
        return true;
    case WM_LBUTTONUP:
        io.MouseDown[0] = false;
        return true;
    case WM_RBUTTONDOWN:
        io.MouseDown[1] = true;
        return true;
    case WM_RBUTTONUP:
        io.MouseDown[1] = false;
        return true;
    case WM_MBUTTONDOWN:
        io.MouseDown[2] = true;
        return true;
    case WM_MBUTTONUP:
        io.MouseDown[2] = false;
        return true;
    case WM_XBUTTONDOWN:
        if ((GET_KEYSTATE_WPARAM(wParam) & MK_XBUTTON1) == MK_XBUTTON1)
            io.MouseDown[3] = true;
        else if ((GET_KEYSTATE_WPARAM(wParam) & MK_XBUTTON2) == MK_XBUTTON2)
            io.MouseDown[4] = true;
        return true;
    case WM_XBUTTONUP:
        if ((GET_KEYSTATE_WPARAM(wParam) & MK_XBUTTON1) == MK_XBUTTON1)
            io.MouseDown[3] = false;
        else if ((GET_KEYSTATE_WPARAM(wParam) & MK_XBUTTON2) == MK_XBUTTON2)
            io.MouseDown[4] = false;
        return true;
    case WM_MOUSEWHEEL:
        io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
        return true;
    case WM_MOUSEMOVE:
        io.MousePos.x = (signed short)(lParam);
        io.MousePos.y = (signed short)(lParam >> 16);
        return true;
    case WM_KEYDOWN:
        if (wParam < 256)
            io.KeysDown[wParam] = 1;
        return true;
    case WM_KEYUP:
        if (wParam < 256)
            io.KeysDown[wParam] = 0;
        return true;
    case WM_CHAR:
        // You can also use ToAscii()+GetKeyboardState() to retrieve characters.
        if (wParam > 0 && wParam < 0x10000)
            io.AddInputCharacter((unsigned short)wParam);
        return true;
    }
    return 0;
}


LRESULT Hooks::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    VM_BEGIN("HOOKS_WNDPROC")
    // for now as a lambda, to be transfered somewhere
    // Thanks uc/WasserEsser for pointing out my mistake!
    // Working when you HOLD th button, not when you press it.
    const auto getButtonHeld = [uMsg, wParam](bool& bButton, int vKey)
    {
        if (wParam != vKey) return;

        if (uMsg == WM_KEYDOWN)
            bButton = true;
        else if (uMsg == WM_KEYUP)
            bButton = false;
    };

    const auto getButtonToggle = [uMsg, wParam](bool& bButton, int vKey)
    {
        if (wParam != vKey) return;

        if (uMsg == WM_KEYUP)
            bButton = !bButton;
    };

    g::hWnd = hWnd;

    getButtonToggle(g_Settings.Cheat.bMenuOpened, VK_INSERT);


    if (g_Hooks.bInitializedDrawManager && g_Settings.Cheat.bMenuOpened && ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;

    // Call original wndproc to make game use input again
    return CallWindowProcA(g_Hooks.pOriginalWNDProc, hWnd, uMsg, wParam, lParam);
    VM_END
}

void __fastcall Hooks::PaintTraverse(IVPanel* ecx, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
    VM_BEGIN("HOOKS_PAINTTRAVERSE")
    static auto oPaintTraverse = g_Hooks.pVPanelHook->GetOriginal<PaintTraverse_t>(vtable_indexes::painttraverse);
    oPaintTraverse(ecx, edx, vguiPanel, forceRepaint, allowForce);

    if (!g_Hooks.bInitializedDrawManager)
        return;

    VM_END
}


void Hooks::MouseEnableExecute()
{
    static bool bIsHeld = false;
    if (g_Settings.Cheat.bMenuOpened && !bIsHeld)
    {
        g_pEngine->ExecuteClientCmd(XORSTR("cl_mouseenable 0"));
        bIsHeld = true;
    }
    else if (!g_Settings.Cheat.bMenuOpened && bIsHeld)
    {
        g_pEngine->ExecuteClientCmd(XORSTR("cl_mouseenable 1"));
        bIsHeld = false;
    }
}


int __fastcall Hooks::SendDatagram(INetChannel* ecx, void* edx, bf_write* pDatagram)
{
    bf_write* data = (bf_write*)pDatagram;

    if (pDatagram)
        return g_Hooks.oSendDatagram(ecx, edx, pDatagram);

    auto instate = ecx->m_nInReliableState;
    auto in_sequencenr = ecx->m_nInSequenceNr;

    auto latency = g_Settings.Miscellaneous.flFakeLatency / 1000.f;
    auto latency_delta = latency - g_pEngine->GetNetChannelInfo()->GetLatency(0/*FLOW_OUTGOING*/);


    FakeLatency::AddLatencyToNetchan(ecx, latency_delta);

    int ret = g_Hooks.oSendDatagram(ecx, edx, pDatagram);

    ecx->m_nInReliableState = instate;
    ecx->m_nInSequenceNr = in_sequencenr;

    return ret;
}


bool __fastcall Hooks::OverrideConfig(IMaterialSystem* this0, int edx, MaterialSystem_Config_t* config, bool forceUpdate)
{
    static auto oOverrideConfig = g_Hooks.pMaterialSystemHook->GetOriginal<OverrideConfig_t>(vtable_indexes::overrideconfig);

    if (g_pEngine->IsInGame() && g_pEngine->IsConnected())
    {
        config->m_bDrawGray = g_Settings.Visuals.Others.bDrawGrey;
        config->m_nFullbright = g_Settings.Visuals.Others.bFullbright;
        config->m_bShowLowResImage = g_Settings.Visuals.Others.bShowLowResImage;
        config->m_nShowMipLevels = g_Settings.Visuals.Others.bShowMipLevels;
    }

    return oOverrideConfig(this0, config, forceUpdate);
}


/*bool __fastcall Hooks::SvCheats_GetBool(PVOID pConVar, int edx)
{
    static auto oGetBool = g_Hooks.pConVarsHook->GetOriginal<SvCheatsGetBool_t>(vtable_indexes::getbool);
    static auto CAM_THINK = Utils::FindSignature(XORSTR("client.dll"), XORSTR("85 C0 75 30 38 86"));
    if (!pConVar)
        return false;

    if (g_Settings.Miscellaneous.bThirdPerson)
    {
        if ((uintptr_t)_ReturnAddress() == CAM_THINK)
            return true;
    }

    return g_Hooks.oGetBool(pConVar);
}*/