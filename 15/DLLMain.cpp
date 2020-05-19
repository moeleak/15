#include <thread>
#include "Hooks.h"
#include "Utils/Utils.h"
#include "Utils/Xorstr.h"
#include "Utils/VMProtect.h"
#include "Utils/HideMoudle.h"
#include "Settings.h"
#include "Utils/DrawManager.h"

DWORD WINAPI OnDllAttach(PVOID base)
{
#ifndef _DEBUG
    VMProtectBegin("ONDLLATTACH");
#endif

#ifdef _DEBUG       // Create console only in debug mode
    AllocConsole();                                 // Alloc memory and create console    
    freopen_s((FILE**)stdin,  XORSTR("CONIN$"), XORSTR("r"),  stdin);   // ----------------------------------------------
    freopen_s((FILE**)stdout, XORSTR("CONOUT$"), XORSTR("w"), stdout);  //  Make iostream library use our console handle
                                                                        // ----------------------------------------------
    SetConsoleTitleA(XORSTR("Project 15 - Debug console"));   // Set console name to a custom one
#endif

    // HideModule(GetModuleHandle(L"15.dll"));
    
    Utils::Log(XORSTR("Console Allocated!"));
    Hooks::Init();

    g_pCVar->FindVar(XORSTR("developer"))->SetValue(1);
    g_pCVar->FindVar(XORSTR("con_filter_enable"))->SetValue(2);
    g_pCVar->FindVar(XORSTR("con_filter_text"))->SetValue("[15]");

    while (g_Settings.Cheat.bCheatActive)
    {
        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);
#ifndef _DEBUG
    VMProtectEnd();
#endif
}

VOID WINAPI OnDllDetach()
{
#ifdef _DEBUG
    fclose((FILE*)stdin);
    fclose((FILE*)stdout);
    FreeConsole();  // Free allocated memory and remove console
#endif
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    VM_BEGIN("DLLMAIN")
    if (dwReason == DLL_PROCESS_ATTACH) 
    {
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, NULL, OnDllAttach, hModule, NULL, nullptr);
    }
    else if (dwReason == DLL_PROCESS_DETACH) 
    {
        if (!lpReserved)
        {
            Hooks::Restore();
            using namespace std::literals::chrono_literals;
            std::this_thread::sleep_for(1s);
        }

        OnDllDetach();
    }
    return TRUE;
    VM_END
}


