#pragma once

#include "Utils/DrawManager.h"
#include "Utils/Interfaces.h"
#include "SDK/SDK.h"
#include "EventListener.h"
#include "15GUI/Menu.h"
#include "Utils/VMTHook.h"

namespace vtable_indexes
{
    constexpr auto sendmsg              = 0;
    constexpr auto retrievemsg          = 2;
    constexpr auto sceneend             = 9;
    constexpr auto getbool              = 13;
    constexpr auto reset                = 16;
    constexpr auto present              = 17;
    constexpr auto overrideview         = 18;
    constexpr auto drawmodelexecute     = 21;
    constexpr auto overrideconfig       = 21;
    constexpr auto overridemouseinput   = 23;
    constexpr auto createmove           = 22;
    constexpr auto framestagenotify     = 37;
    constexpr auto dispatchusermessage  = 38;
    constexpr auto painttraverse        = 41;
    constexpr auto endscene             = 42;
    constexpr auto senddatagram         = 46;
}

class VMTHook;
class Hooks
{
public:
    // Initialization setup, called on injection
    static void Init();
    static void Restore();

    /*---------------------------------------------*/
    /*-------------Hooked functions----------------*/
    /*---------------------------------------------*/

    static EGCResults   __fastcall      SendMsg             (void* ecx, void* edx, uint32_t unMsgType, const void* pubData, uint32_t cubData);
    static EGCResults   __fastcall      RetrieveMsg         (void* ecx, void* edx, uint32_t *punMsgType, void *pubDest, uint32_t cubDest, uint32_t *pcubMsgSize);
    static bool         __fastcall      CreateMove	        (IClientMode*, void*, float, CUserCmd*);
    static bool         __fastcall      DispatchUserMessage (void* ecx, void* edx, int type, unsigned int a3, unsigned int length, const void *msg_data);
    static void         __fastcall      OverrideMouseInput  (IClientMode*, void*, float* x, float* y);
    static void         __fastcall      OverrideView        (IClientMode*, void*, CViewSetup*);
    static void         __fastcall      SceneEnd            (void* ecx, void* edx);
    static void         __fastcall      PaintTraverse       (IVPanel* ecx, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
    static int          __fastcall      SendDatagram        (INetChannel* ecx, void* edx, bf_write* pDatagram);
    static bool         __fastcall      OverrideConfig      (IMaterialSystem* this0, int edx, MaterialSystem_Config_t* config, bool forceUpdate);
    static bool         __fastcall      SvCheats_GetBool    (PVOID pConVar, int edx);
    static void         __stdcall       DrawModelExecute    (IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld);
    static void         __stdcall       FrameStageNotify    (ClientFrameStage_t);
    static HRESULT      __stdcall       Reset               (IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
    static HRESULT      __stdcall       StreamReset         (IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
    static HRESULT      __stdcall       EndScene            (IDirect3DDevice9* pDevice);
    static HRESULT      __stdcall       Present             (IDirect3DDevice9* pDevice, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion);
    static HRESULT      __stdcall       StreamPresent       (IDirect3DDevice9* pDevice, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion);
    static LRESULT      __stdcall       WndProc             (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
    /*---------------------------------------------*/
    /*-------------Hook prototypes-----------------*/
    /*---------------------------------------------*/

    typedef EGCResults  (__fastcall* RetrieveMessage_t)      (void*, void*, uint32_t *, void *, uint32_t, uint32_t *);
    typedef EGCResults  (__fastcall* SendMessage_t)          (void*, void*, uint32_t, const void*, uint32_t);
    typedef bool        (__fastcall* CreateMove_t)           (IClientMode*, void*, float, CUserCmd*);
    typedef void        (__fastcall* OverrideMouseInput_t)   (IClientMode*, void*, float*, float*);
    typedef void        (__fastcall* OverrideView_t)         (IClientMode*, void*, CViewSetup*);
    typedef void        (__fastcall* SceneEnd_t)             (void*, void*);
    typedef void        (__fastcall* PaintTraverse_t)        (IVPanel*, void*, unsigned int, bool, bool);
    typedef int         (__fastcall* SendDatagram_t)         (INetChannel*, void*, bf_write*);
    typedef void*       (__thiscall* DrawModelExecute_t)     (IVModelRender*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
    typedef void        (__thiscall* FrameStageNotify_t)     (IBaseClientDLL*, ClientFrameStage_t);
    typedef bool        (__thiscall* DispatchUserMessage_t)  (void*, int, unsigned int, unsigned int, const void *);
    typedef bool        (__thiscall* OverrideConfig_t)       (IMaterialSystem*, MaterialSystem_Config_t*, bool);
    typedef bool        (__thiscall* SvCheatsGetBool_t)      (PVOID);
    typedef long        (__stdcall*  Reset_t)                (IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
    typedef long        (__stdcall*  Endscene_t)             (IDirect3DDevice9*);
    typedef long        (__stdcall*  Present_t)              (IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);

    /*---------------------------------------------*/
    /*------------Function Hook pointers-----------*/
    /*---------------------------------------------*/

    uintptr_t pStreamPresent;
    uintptr_t pStreamReset;

    Present_t oStreamPresent;
    Reset_t oStreamReset;

    SendDatagram_t oSendDatagram;
    SvCheatsGetBool_t oGetBool;
    /*---------------------------------------------*/
    /*-------------VMT Hook pointers---------------*/
    /*---------------------------------------------*/

    std::unique_ptr<VMTHook> pD3DDevice9Hook;
    std::unique_ptr<VMTHook> pClientModeHook;
    std::unique_ptr<VMTHook> pClientHook;
    std::unique_ptr<VMTHook> pRenderViewHook;
    std::unique_ptr<VMTHook> pSteamGameCoordinator;
    std::unique_ptr<VMTHook> pMatRenderHook;
    std::unique_ptr<VMTHook> pVPanelHook;
    std::unique_ptr<VMTHook> pNetChannelHook = nullptr;
    std::unique_ptr<VMTHook> pMaterialSystemHook;
    std::unique_ptr<VMTHook> pConVarsHook;

private:
    static void MouseEnableExecute();

    MenuMain                       nMenu;
    HWND                           hCSGOWindow             = nullptr; // CSGO window handle
    bool                           bInitializedDrawManager = false;   // Check if we initialized our draw manager
    WNDPROC                        pOriginalWNDProc        = nullptr; // Original CSGO window proc
    std::unique_ptr<EventListener> pEventListener          = nullptr; // Listens to csgo events, needs to be created
};

extern Hooks g_Hooks;




/*
class VMTHook
{
public:
    VMTHook(void* ppClass)
    {
        this->ppBaseClass = static_cast<std::uintptr_t**>(ppClass);

        // loop through all valid class indexes. When it will hit invalid (not existing) it will end the loop
        while (static_cast<std::uintptr_t*>(*this->ppBaseClass)[this->indexCount])
            ++this->indexCount;

        const std::size_t kSizeTable = this->indexCount * sizeof(std::uintptr_t);


        this->pOriginalVMT = *this->ppBaseClass;
        this->pNewVMT      = std::make_unique<std::uintptr_t[]>(this->indexCount);

        // copy original vtable to our local copy of it
        std::memcpy(this->pNewVMT.get(), this->pOriginalVMT, kSizeTable);

        // replace original class with our local copy
        *this->ppBaseClass = this->pNewVMT.get();
    };
    ~VMTHook() { *this->ppBaseClass = this->pOriginalVMT; };

    template<class Type>
    Type GetOriginal(const std::size_t index)
    {
        return reinterpret_cast<Type>(this->pOriginalVMT[index]);
    };

    HRESULT Hook(const std::size_t index, void* fnNew)
    {
        if (index > this->indexCount)   // check if given index is valid
            return E_INVALIDARG;

        this->pNewVMT[index] = reinterpret_cast<std::uintptr_t>(fnNew);
        return S_OK;
    };

    HRESULT Unhook(const std::size_t index)
    {
        if (index > this->indexCount)
            return E_INVALIDARG;

        this->pNewVMT[index] = this->pOriginalVMT[index];
        return S_OK;
    };

    void Clear()
    {
        ppBaseClass = NULL;
    }

private:
    std::unique_ptr<std::uintptr_t[]> pNewVMT      = nullptr;    // Actual used vtable
    std::uintptr_t**                  ppBaseClass  = nullptr;             // Saved pointer to original class
    std::uintptr_t*                   pOriginalVMT = nullptr;             // Saved original pointer to the VMT
    std::size_t                       indexCount   = 0;                     // Count of indexes inside out f-ction
};
*/
