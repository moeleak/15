#include "Interfaces.h"
#include "Utils.h"

#include "../SDK/SDK.h"

// Initializing global interfaces

IBaseClientDLL*     g_pClientDll        = nullptr;
IClientMode*        g_pClientMode       = nullptr;
IClientEntityList*  g_pEntityList       = nullptr;
IVEngineClient*     g_pEngine           = nullptr;
CPrediction*        g_pPrediction       = nullptr;
IGameMovement*      g_pMovement         = nullptr;
IMoveHelper*        g_pMoveHelper       = nullptr;
CGlobalVarsBase*    g_pGlobalVars       = nullptr;
IGameEventManager2* g_pEventManager     = nullptr;
IMemAlloc*          g_pMemAlloc         = nullptr;
ICVar*              g_pCVar             = nullptr;
CInput*             g_pInput            = nullptr;
ISurface*           g_pSurface          = nullptr;
IVPanel*            g_pVPanel           = nullptr;
IVModelInfo*        g_pModelInfo        = nullptr;
IMaterialSystem*    g_pMaterialSystem   = nullptr;
C_CSPlayerResource* g_pPlayerResource   = nullptr;
IEngineTrace*       g_pEngineTrace      = nullptr;
IViewRender*        g_pViewRender       = nullptr;
IVRenderView*       g_pRenderView       = nullptr;
IVModelRender*      g_pMdlRender        = nullptr;
CClientState*       g_pClientState      = nullptr;
IWeaponSystem*      g_pWeaponSystem     = nullptr;
IPhysicsSurfaceProps* g_pPhysSurface    = nullptr;


ISteamClient*           g_pSteamClient              = nullptr;
ISteamHTTP*             g_pSteamHTTP                = nullptr;
ISteamUser*             g_pSteamUser                = nullptr;
ISteamFriends*          g_pSteamFriends             = nullptr;
ISteamInventory*        g_pSteamInventory           = nullptr;
ISteamGameCoordinator*  g_pSteamGameCoordinator     = nullptr;

SteamUserHandle         g_hSteamUser = 0;
SteamPipeHandle         g_hSteamPipe = 0;

namespace interfaces
{
    template<typename T>
    T* CaptureInterface(const char* szModuleName, const char* szInterfaceVersion)
    {
        HMODULE moduleHandle = GetModuleHandleA(szModuleName);
        if (moduleHandle)   /* In case of not finding module handle, throw an error. */
        {
            CreateInterfaceFn pfnFactory = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(moduleHandle, XORSTR("CreateInterface")));
            return reinterpret_cast<T*>(pfnFactory(szInterfaceVersion, nullptr));
        }
        Utils::Log(XORSTR("Error getting interface %s"), szInterfaceVersion);
        return nullptr;
    }


    void Init()
    {
#ifndef _DEBUG
        VMProtectBegin("INTERFACES");
#endif
        g_pClientDll        = CaptureInterface<IBaseClientDLL>(XORSTR("client_panorama.dll"), XORSTR("VClient018"));
        g_pEngine           = CaptureInterface<IVEngineClient>(XORSTR("engine.dll"), XORSTR("VEngineClient014"));
        g_pClientMode       = **reinterpret_cast<IClientMode***>((*reinterpret_cast<uintptr_t**>(g_pClientDll))[10] + 0x5u);
        g_pGlobalVars       = **reinterpret_cast<CGlobalVarsBase***>((*reinterpret_cast<uintptr_t**>(g_pClientDll))[0] + 0x1Bu);
        g_pInput            = *reinterpret_cast<CInput**>((*reinterpret_cast<uintptr_t**>(g_pClientDll))[15] + 0x1);
        g_pClientState      = **reinterpret_cast<CClientState***>((*reinterpret_cast<uintptr_t**>(g_pEngine))[12] + 0x10);
        g_pEntityList       = CaptureInterface<IClientEntityList>(XORSTR("client_panorama.dll"), XORSTR("VClientEntityList003"));
        g_pPrediction       = CaptureInterface<CPrediction>(XORSTR("client_panorama.dll"), XORSTR("VClientPrediction001"));
        g_pMovement         = CaptureInterface<IGameMovement>(XORSTR("client_panorama.dll"), XORSTR("GameMovement001"));
        g_pSurface          = CaptureInterface<ISurface>(XORSTR("vguimatsurface.dll"), XORSTR("VGUI_Surface031"));
        g_pVPanel           = CaptureInterface<IVPanel>(XORSTR("vgui2.dll"), XORSTR("VGUI_Panel009"));
        g_pModelInfo        = CaptureInterface<IVModelInfo>(XORSTR("engine.dll"), XORSTR("VModelInfoClient004"));
        g_pEngineTrace      = CaptureInterface<IEngineTrace>(XORSTR("engine.dll"), XORSTR("EngineTraceClient004"));
        g_pMoveHelper       = **reinterpret_cast<IMoveHelper***>((Utils::FindSignature(XORSTR("client_panorama.dll"), XORSTR("8B 0D ? ? ? ? 8B 46 08 68")) + 0x2));
        g_pEventManager     = CaptureInterface<IGameEventManager2>(XORSTR("engine.dll"), XORSTR("GAMEEVENTSMANAGER002"));
        g_pMemAlloc         = **reinterpret_cast<IMemAlloc***>(GetProcAddress(GetModuleHandleA(XORSTR("tier0.dll")), XORSTR("g_pMemAlloc")));
        g_pCVar             = CaptureInterface<ICVar>(XORSTR("vstdlib.dll"), XORSTR("VEngineCvar007"));
        g_pRenderView       = CaptureInterface<IVRenderView>(XORSTR("engine.dll"), XORSTR("VEngineRenderView014"));
        g_pViewRender       = **reinterpret_cast<IViewRender***>(Utils::FindSignature(XORSTR("client_panorama.dll"), XORSTR("A1 ? ? ? ? B9 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? FF 10")) + 1);
        g_pMdlRender        = CaptureInterface<IVModelRender>(XORSTR("engine.dll"), XORSTR("VEngineModel016"));
        g_pMaterialSystem   = CaptureInterface<IMaterialSystem>(XORSTR("materialsystem.dll"), XORSTR("VMaterialSystem080"));
        g_pWeaponSystem     = *reinterpret_cast<IWeaponSystem**>(Utils::FindSignature(XORSTR("client_panorama.dll"), XORSTR("FF 50 04 83 BE ? ? ? ? ? 74 0A")) - 4);
        g_pPhysSurface      = CaptureInterface<IPhysicsSurfaceProps>(XORSTR("vphysics.dll"), XORSTR("VPhysicsSurfaceProps"));

        g_hSteamUser = ((SteamUserHandle(__cdecl*)(void))GetProcAddress(GetModuleHandleA(XORSTR("steam_api.dll")), XORSTR("SteamAPI_GetHSteamUser")))();
        g_hSteamPipe = ((SteamPipeHandle(__cdecl*)(void))GetProcAddress(GetModuleHandleA(XORSTR("steam_api.dll")), XORSTR("SteamAPI_GetHSteamPipe")))();

        g_pSteamClient              = ((ISteamClient*(__cdecl*)(void))GetProcAddress(GetModuleHandleA(XORSTR("steam_api.dll")), XORSTR("SteamClient")))();
        g_pSteamHTTP                = g_pSteamClient->GetISteamHTTP(g_hSteamUser, g_hSteamPipe, XORSTR("STEAMHTTP_INTERFACE_VERSION002"));
        g_pSteamUser                = g_pSteamClient->GetISteamUser(g_hSteamUser, g_hSteamPipe, XORSTR("SteamUser019"));
        g_pSteamFriends             = g_pSteamClient->GetISteamFriends(g_hSteamUser, g_hSteamPipe, XORSTR("SteamFriends015"));
        g_pSteamInventory           = g_pSteamClient->GetISteamInventory(g_hSteamUser, g_hSteamPipe, XORSTR("STEAMINVENTORY_INTERFACE_V002"));
        g_pSteamGameCoordinator     = (ISteamGameCoordinator*)g_pSteamClient->GetISteamGenericInterface(g_hSteamUser, g_hSteamPipe, XORSTR("SteamGameCoordinator001"));

        g_pPlayerResource           = *reinterpret_cast<C_CSPlayerResource**>(*(uintptr_t**)(Utils::FindSignature(XORSTR("client_panorama.dll"), XORSTR("A1 ? ? ? ? 57 85 C0 74 08")) + 1));
#ifndef _DEBUG
        VMProtectEnd();
#endif 
    }
}

