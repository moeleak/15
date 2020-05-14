#pragma once
#include "../Utils/DrawManager.h"
#include "../SDK/Imgui/imgui.h"

namespace B1gFonts
{
    extern ImFont* Default;
    extern ImFont* Tab;
}

class MenuMain
{
public:
    MenuMain();
    ~MenuMain();
public:
    void Initialize(IDirect3DDevice9* pDevice);
    void InstallTheme();
public:
    void Render();
private:
    void RenderTab();
private:
    // Windows
    void RenderMainWindow();
private:
    // Tabs
    void RenderAimbotTab();
    void RenderVisualsTab();
    void RenderMiscTab();
    void RenderColorTab();
    void RenderAboutTab();
protected:
    unsigned int tabPageCnt;
};

