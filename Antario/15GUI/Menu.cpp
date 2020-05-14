#include "Menu.h"
#include "../Settings.h"
#include "../Utils/GlobalVars.h"
#include "../Utils/Xorstr.h"
#include "../Hooks.h"
#include "../SDK/Imgui/imgui.h"
#include "../SDK/Imgui/imgui_impl_dx9.h"
#include "Fonts/UnifrakturCook.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))


ImFont* B1gFonts::Default = nullptr;
ImFont* B1gFonts::Tab = nullptr;


MenuMain::MenuMain()
{
}

MenuMain::~MenuMain()
{
}

void MenuMain::Initialize(IDirect3DDevice9* pDevice)
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplDX9_Init(pDevice);

    InstallTheme();
}

void MenuMain::InstallTheme()
{
    ImGuiIO& io = ImGui::GetIO();

    //B1gFonts::Default = io.Fonts->AddFontFromFileTTF(XORSTR("c:/windows/fonts/SegoeUI.ttf"), 18.0f, NULL, io.Fonts->GetGlyphRangesDefault());
    B1gFonts::Default = io.Fonts->AddFontFromMemoryCompressedBase85TTF(UnifrakturCook_compressed_data_base85, 18.0f);
    B1gFonts::Tab = io.Fonts->AddFontFromFileTTF(XORSTR("c:/windows/fonts/SegoeUI.ttf"), 32.0f, NULL, io.Fonts->GetGlyphRangesDefault());

    auto style = ImGui::GetStyle();
    ImGui::StyleColorsDark();
    style.WindowRounding = 20.f;
    style.WindowPadding = ImVec2(8, 8);
    style.FramePadding = ImVec2(5, 2);
    style.ItemInnerSpacing = ImVec2(4, 4);
    style.DisplayWindowPadding = ImVec2(805, 472);
    style.DisplaySafeAreaPadding = ImVec2(4, 4);
    style.FrameRounding = 0.f;
    style.ScrollbarSize = 2.f;
    style.ScrollbarRounding = 0.f;
    style.GrabRounding = 0.f;
    style.GrabMinSize = 8.f;
    style.AntiAliasedFill = false;
    style.ChildRounding = 0.0f;

    g_Settings.Color.ColorPicker.push_back(ColorP("Menu - Text", g_Settings.Color.flColMenuText));
    g_Settings.Color.ColorPicker.push_back(ColorP("Menu - Background", g_Settings.Color.flColMenuBackground));
    g_Settings.Color.ColorPicker.push_back(ColorP("Menu - Border", g_Settings.Color.flColMenuBorder));
    g_Settings.Color.ColorPicker.push_back(ColorP("Menu - Theme", g_Settings.Color.flColMenuTheme));
    g_Settings.Color.ColorPicker.push_back(ColorP("Glow - Team", g_Settings.Color.flGlowTeam));
    g_Settings.Color.ColorPicker.push_back(ColorP("Glow - Enemy", g_Settings.Color.flGlowEnemy));
    g_Settings.Color.ColorPicker.push_back(ColorP("Chams - LocalPlayer", g_Settings.Color.flChamsLocalPlayer));
    g_Settings.Color.ColorPicker.push_back(ColorP("Chams - Visible - Team", g_Settings.Color.flChamsVisibleTeam));
    g_Settings.Color.ColorPicker.push_back(ColorP("Chams - Visible - Enemy", g_Settings.Color.flChamsVisibleEnemy));
    g_Settings.Color.ColorPicker.push_back(ColorP("Chams - Occluded - Team", g_Settings.Color.flChamsOccludedTeam));
    g_Settings.Color.ColorPicker.push_back(ColorP("Chams - Occluded - Enemy", g_Settings.Color.flChamsOccludedEnemy));
    g_Settings.Color.ColorPicker.push_back(ColorP("Visual - Box - Team", g_Settings.Color.flBoxTeam));
    g_Settings.Color.ColorPicker.push_back(ColorP("Visual - Box - Enemy", g_Settings.Color.flBoxEnemy));
    g_Settings.Color.ColorPicker.push_back(ColorP("Visual - Text - Team", g_Settings.Color.flTextTeam));
    g_Settings.Color.ColorPicker.push_back(ColorP("Visual - Text - Enemy", g_Settings.Color.flTextEnemy));
    g_Settings.Color.ColorPicker.push_back(ColorP("Visual - Skeleton - Team", g_Settings.Color.flSkeletonTeam));
    g_Settings.Color.ColorPicker.push_back(ColorP("Visual - Skeleton - Enemy", g_Settings.Color.flSkeletonEnemy));
    g_Settings.Color.ColorPicker.push_back(ColorP("NadePrediction - Line", g_Settings.Color.flNadePrediction));
}

void MenuMain::Render()
{
    if (!g_Settings.Cheat.bMenuOpened)
        return;

    RenderMainWindow();
    RenderTab();
}


void MenuMain::RenderTab()
{   
    auto& style = ImGui::GetStyle();
    int w, h;
    g_pEngine->GetScreenSize(w, h);

    const char* tabs[] = 
    {
        "Aimbot",
        "Visuals",
        "Misc",
        "Color",
        "About",
    };

    ImGui::PushFont(B1gFonts::Tab);
    ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
    ImGui::SetNextWindowSize(ImVec2(125, (float)h));
    ImGui::Begin(XORSTR("##TabArea"), &g_Settings.Cheat.bMenuOpened, ImVec2(125, (float)h), 0.98f, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
    {
        for (int i = 0; i < IM_ARRAYSIZE(tabs); i++)
        {
            int distance = i == tabPageCnt ? 0 : i > tabPageCnt ? i - tabPageCnt : tabPageCnt - i;

            if (ImGui::Button(tabs[i], ImVec2(125, 75)))
                tabPageCnt = i;

            i < IM_ARRAYSIZE(tabs) - 1;
        }
    }
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2((float)129, (float)h));
    ImGui::Begin(XORSTR("##TabAreaColor"), &g_Settings.Cheat.bMenuOpened, ImVec2(129, (float)h), 0.98f, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
    {
        ImGui::Text("");
    }
    ImGui::End();
    ImGui::PopFont();

    style.ItemSpacing = ImVec2(8, 4);
    style.WindowPadding = ImVec2(8, 8);
}


void MenuMain::RenderMainWindow()
{
    ImGui::Begin(XORSTR("Project 15"), &g_Settings.Cheat.bMenuOpened, ImVec2(975, 450), 0.98f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
    {

        switch (tabPageCnt)
        {
        case 0:
            RenderAimbotTab();
            break;
        case 1:
            RenderVisualsTab();
            break;
        case 2:
            RenderMiscTab();
            break;
        case 3:
            RenderColorTab();
            break;
        case 4:
            RenderAboutTab();
            break;
        }

    }
    ImGui::End();
}

void MenuMain::RenderAimbotTab()
{
    ImGui::Text(XORSTR("Aimbot"));
    ImGui::BeginChild(XORSTR("##Aimbot"), ImVec2(0, 0), true);
    {
        ImGui::Checkbox(XORSTR("Backtrack"), &g_Settings.LegitBot.bBacktrack);
    }
    ImGui::EndChild();
}

void MenuMain::RenderVisualsTab()
{
    static const char* strChamsType[] = { "Texture", "Flat", "Glass", "Gloss" };
    ImGui::Text(XORSTR("Visuals"));
    ImGui::BeginChild(XORSTR("Visuals"), ImVec2(0, 0), true);
    {
        ImGui::Text(XORSTR("Filter"));
        ImGui::Separator();
        ImGui::Columns(2, nullptr, false);
        {
            ImGui::Checkbox(XORSTR("Friends"), &g_Settings.Visuals.Filter.bFriends);
        } 
        ImGui::Separator();
        ImGui::Columns(2, nullptr, false);
        {
            ImGui::Text(XORSTR("Type"));
            ImGui::Checkbox(XORSTR("Glows"), &g_Settings.Visuals.Type.bGlows);
            ImGui::Checkbox(XORSTR("Draw-Skeleton"), &g_Settings.Visuals.Type.bShowSkeleton);
            ImGui::Checkbox(XORSTR("Draw-Boxes"), &g_Settings.Visuals.Type.bShowBoxes);
            ImGui::Checkbox(XORSTR("Draw-Names"), &g_Settings.Visuals.Type.bShowNames);
            ImGui::Checkbox(XORSTR("Draw-HP"), &g_Settings.Visuals.Type.bShowHealth);
            ImGui::Checkbox(XORSTR("Draw-Armor"), &g_Settings.Visuals.Type.bShowArmor);
            ImGui::Checkbox(XORSTR("Draw-Weapons"), &g_Settings.Visuals.Type.bShowWeapons);
            ImGui::Checkbox(XORSTR("Info-Scope"), &g_Settings.Visuals.Type.bShowIfIsScoped);
            ImGui::Checkbox(XORSTR("Info-Money"), &g_Settings.Visuals.Type.bShowMoneny);
        }
        ImGui::NextColumn();
        {
            ImGui::Text(XORSTR("Others"));
            ImGui::Checkbox(XORSTR("Nade Prediction"), &g_Settings.Visuals.Others.bNadePrediction);
            ImGui::Checkbox(XORSTR("Night Mode"), &g_Settings.Visuals.Others.bNightMode);
            ImGui::Checkbox(XORSTR("Grey World"), &g_Settings.Visuals.Others.bDrawGrey);
            ImGui::Checkbox(XORSTR("Full Bright"), &g_Settings.Visuals.Others.bFullbright);
            ImGui::Checkbox(XORSTR("Pixel World"), &g_Settings.Visuals.Others.bShowLowResImage);
            ImGui::Checkbox(XORSTR("MLG Mode"), &g_Settings.Visuals.Others.bShowMipLevels);
        }
        ImGui::NextColumn();
        {
            ImGui::Text(XORSTR("Chams"));
            ImGui::Checkbox(XORSTR("Enabled"), &g_Settings.Visuals.Chams.bEnabled);
            ImGui::Checkbox(XORSTR("Player-XQZ"), &g_Settings.Visuals.Chams.bPlayerIgnorez);
            ImGui::Checkbox(XORSTR("Player-Local"), &g_Settings.Visuals.Chams.bLocalPlayer);
            ImGui::Checkbox(XORSTR("Arms"), &g_Settings.Visuals.Chams.bArms);
            ImGui::Text(XORSTR("Chams Types"));
            ImGui::Combo(XORSTR("Arms"), reinterpret_cast<int*>(&g_Settings.Visuals.Chams.nArmsType), "Texture\0Flat\0Glass\0Gloss");
            ImGui::Combo(XORSTR("Player"), reinterpret_cast<int*>(&g_Settings.Visuals.Chams.nPlayerType), "Texture\0Flat\0Glass\0Gloss");
            ImGui::Combo(XORSTR("LocalPlayer"), reinterpret_cast<int*>(&g_Settings.Visuals.Chams.nLocalPlayerType), "Texture\0Flat\0Glass\0Gloss");
        }
    }
    ImGui::EndChild();
}

void MenuMain::RenderMiscTab()
{
    static char strFileName[10];

    ImGui::Text(XORSTR("Misc"));
    ImGui::BeginChild(XORSTR("Misc"), ImVec2(0, 0), true);
    {
        ImGui::Checkbox(XORSTR("Auto Bunnyhop"), &g_Settings.Miscellaneous.bAutobhop);
        ImGui::Checkbox(XORSTR("Auto Strafe"), &g_Settings.Miscellaneous.bAutoStrafe);
        ImGui::Checkbox(XORSTR("Ground Strafe"), &g_Settings.Miscellaneous.bGroundStrafe);
        ImGui::Checkbox(XORSTR("Fake Latency"), &g_Settings.Miscellaneous.bFakeLatency);
        ImGui::SliderFloat(XORSTR("##Latency"), &g_Settings.Miscellaneous.flFakeLatency, 100.f, 1000.f);
        ImGui::Checkbox(XORSTR("Stream Proof"), &g_Settings.Miscellaneous.bStreamProof);
        ImGui::Checkbox(XORSTR("ThirdPerson"), &g_Settings.Miscellaneous.bThirdPerson);
        ImGui::SliderFloat(XORSTR("Fov"), &g_Settings.Miscellaneous.flFov, 0.f, 50.f);
        if (ImGui::Button(XORSTR("Close The Cheat")))
        {
            g_Settings.Cheat.bMenuOpened = false;
            g_Settings.Cheat.bCheatActive = false;
        }
        
        static char clantagchanger[127] = "";

        ImGui::Text(XORSTR("ClanTag"));
        ImGui::InputText(XORSTR("##CLANTAGINPUT"), clantagchanger, 127);
        if (ImGui::Button(XORSTR("Apply")))
            Utils::SetClanTag(clantagchanger);

        ImGui::Columns(2, nullptr, false);
        {
            ImGui::Text(XORSTR("Configs"));
            ImGui::Separator();
            ImGui::InputText(XORSTR("##filename"), strFileName, 10);
            if (ImGui::Button(XORSTR("Save")))
            {
                g_Configs.SaveSettings(strFileName);
            } ImGui::SameLine();
            if (ImGui::Button(XORSTR("Load")))
            {
                g_Configs.LoadSettings(strFileName);
            }
        }

    }
    ImGui::EndChild();
}

void MenuMain::RenderColorTab()
{
    ImGui::BeginChild(XORSTR("##color"), ImVec2(962, 437), true);
    { 
        static int selectedItem = 0;

        ImVec2 lastCursor = ImGui::GetCursorPos();

        auto& style = ImGui::GetStyle();
    
        style.WindowPadding = ImVec2(8, 8);
        style.ItemSpacing = ImVec2(4, 4);

        ImGui::ListBoxHeader(XORSTR("##Colors - Visuals"), ImVec2(400, 418));
        
        for (int i = 0; i < g_Settings.Color.ColorPicker.size(); i++)
        {
            bool selected = i == selectedItem;

            if (ImGui::Selectable(g_Settings.Color.ColorPicker[i].Name, selected))
                selectedItem = i;
        }
        ImGui::ListBoxFooter();

        float nc = lastCursor.x + 260;
        ImGui::SetCursorPos(ImVec2(nc, lastCursor.y));

        ColorP col = g_Settings.Color.ColorPicker[selectedItem];
        float r = col.Ccolor[0];
        float g = col.Ccolor[1];
        float b = col.Ccolor[2];

        bool wip;

        ImGui::SetCursorPosX(nc);


        col.Ccolor[0] = r;
        col.Ccolor[1] = g;
        col.Ccolor[2] = b;

        ImGui::SameLine();

        ImVec2 curPos = ImGui::GetCursorPos();
        ImVec2 curWindowPos = ImGui::GetWindowPos();
        curPos.x += curWindowPos.x;
        curPos.y += curWindowPos.y;

        ImGui::PushItemWidth(400);
        ImGui::ColorPicker4(XORSTR("##clr"), col.Ccolor);
    }
    ImGui::EndChild();
}

void MenuMain::RenderAboutTab()
{
    ImGui::Text(XORSTR("b1g hake in the game!"));
}

