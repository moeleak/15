#pragma once
#include <memory>
#include "D3DFont.h"
#include "../SDK/Color.h"
#include "../SDK/Vector.h"
#include "../Utils/Utils.h"

#define GET_D3DCOLOR_ALPHA(x) (( x >> 24) & 255)
#define COL2DWORD(x) (D3DCOLOR_ARGB(x.alpha, x.red, x.green, x.blue))

enum GradientType;

class DrawManager
{
public: // Function members
    // Basic non-drawing functions

    DrawManager();

    void InitDeviceObjects      (LPDIRECT3DDEVICE9 pDevice);
    void RestoreDeviceObjects   (LPDIRECT3DDEVICE9 pDevice);
    void InvalidateDeviceObjects();
    void SetupRenderStates      ();


    // Drawing functions

    void Line   (Vector2D vecPos1, Vector2D vecPos2, Color color)                   const;
    void Line   (float posx1, float posy1, float posx2, float posy2, Color color) const;
    void Rect   (Vector2D vecPos1, Vector2D vecPos2, Color color) const;
    void Rect   (float posx1, float posy1, float posx2, float posy2, Color color) const;
    void RectFilled          (Vector2D vecPos1, Vector2D vecPos2, Color color) const;
    void RectFilled          (float posx1, float posy1, float posx2, float posy2, Color color) const;
    void Triangle            (Vector2D pos1, Vector2D pos2, Vector2D pos3, Color color) const;
    void TriangleFilled      (Vector2D pos1, Vector2D pos2, Vector2D pos3, Color color) const;
    void RectFilledGradient  (Vector2D vecPos1, Vector2D vecPos2, Color col1, Color col2, GradientType type) const;
    void RectFilledMultiColor(const Vector2D& vecPos1, const Vector2D& vecPos2, Color col1, Color col2, Color col3, Color col4) const;
    void Circle              (Vector2D pos, float r, float points, Color color) const;


    void String (Vector2D vecPos, DWORD dwFlags, Color color, CD3DFont * pFont, const char * szText, ...) const;
    void String (float posx, float posy, DWORD dwFlags, Color color, CD3DFont* pFont, const char* szText, ...) const;

    void String (float posx, float posy, float scaledx, float scaledy, DWORD dwFlags, Color color, CD3DFont* pFont, const char* szText, ...) const;

    int GetTextWitdh(char * text, CD3DFont * pFont) const;
    int GetTextHeight(char * text, CD3DFont * pFont) const;

    // Helpers
    Vector2D GetScreenCenter();

private: // Variable members
    LPDIRECT3DDEVICE9 pDevice;
    D3DVIEWPORT9      pViewPort;
};
extern DrawManager g_Render;


struct Fonts
{
public:
    void DeleteDeviceObjects()
    {
        Utils::Log("Deleting device objects...");
        HRESULT hr;
        hr = pFontVerdana6->DeleteDeviceObjects();
        if (FAILED(hr)) { Utils::Log("Deleting proceture failed for font Verdana 6"); }
        hr = pFontVerdana10->DeleteDeviceObjects();
        if (FAILED(hr)) { Utils::Log("Deleting proceture failed for font Verdana 10"); }
    };

    void InvalidateDeviceObjects()
    {
        HRESULT hr;
        hr = pFontVerdana6->InvalidateDeviceObjects();
        if (FAILED(hr)) { Utils::Log("Invalidating failed for font Verdana 8"); }
        hr = pFontVerdana10->InvalidateDeviceObjects();
        if (FAILED(hr)) { Utils::Log("Invalidating failed for font Verdana 10"); }
    };

    void InitDeviceObjects(LPDIRECT3DDEVICE9 pDevice)
    {
        HRESULT hr;
        hr = pFontVerdana6->InitDeviceObjects(pDevice);
        if (FAILED(hr)) { Utils::Log(hr); }
        hr = pFontVerdana6->RestoreDeviceObjects();
        if (FAILED(hr)) { Utils::Log(hr); }

        hr = pFontVerdana10->InitDeviceObjects(pDevice);
        if (FAILED(hr)) { Utils::Log(hr); }
        hr = pFontVerdana10->RestoreDeviceObjects();
        if (FAILED(hr)) { Utils::Log(hr); }
    };

    // Fonts
    std::unique_ptr<CD3DFont> pFontVerdana6;
    std::unique_ptr<CD3DFont> pFontVerdana10;
};
extern Fonts g_Fonts;


enum GradientType
{
    GRADIENT_VERTICAL,
    GRADIENT_HORIZONTAL
};
