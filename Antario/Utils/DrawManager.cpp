#include "DrawManager.h"
#include <assert.h>

// Init out global vars
Fonts       g_Fonts;
DrawManager g_Render;

struct Vertex
{
    float x, y, z, rhw;
    DWORD color;
};


DrawManager::DrawManager()
{
    this->pDevice         = nullptr;
    g_Fonts.pFontVerdana6 = nullptr;
    g_Fonts.pFontVerdana10 = nullptr;
}


void DrawManager::InitDeviceObjects(LPDIRECT3DDEVICE9 pDevice)
{
    // Save the device internally
    this->pDevice = pDevice;

    // Get viewport for our center coords
    this->pDevice->GetViewport(&pViewPort);

    // Create new fonts
    g_Fonts.pFontVerdana6  = std::make_unique<CD3DFont>(L"Verdana", 6, FW_NORMAL );
    g_Fonts.pFontVerdana10 = std::make_unique<CD3DFont>(L"Verdana", 10, FW_MEDIUM);

    // Init font device objects
    g_Fonts.InitDeviceObjects(pDevice);
}


void DrawManager::InvalidateDeviceObjects()
{
    // Remove a pointer to game device
    this->pDevice = nullptr;
    
    g_Fonts.InvalidateDeviceObjects();
}


void DrawManager::RestoreDeviceObjects(LPDIRECT3DDEVICE9 pDevice)
{
    this->pDevice = pDevice;
    this->pDevice->GetViewport(&pViewPort); 

    g_Fonts.InitDeviceObjects(pDevice);
}


void DrawManager::Line(Vector2D vecPos1, Vector2D vecPos2, Color color) const
{
    this->Line(vecPos1.x, vecPos1.y, vecPos2.x, vecPos2.y, color);
}


void DrawManager::Line(float posx1, float posy1, float posx2, float posy2, Color color) const
{
    D3DCOLOR dwColor = COL2DWORD(color);
    Vertex vert[2] = 
    {  
        { posx1, posy1, 0.0f, 1.0f, dwColor },
        { posx2, posy2, 0.0f, 1.0f, dwColor } 
    };
    
    this->pDevice->SetTexture(0, nullptr);
    this->pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, &vert, sizeof(Vertex));
}


void DrawManager::Rect(Vector2D vecPos1, Vector2D vecPos2, Color color) const
{
    this->Rect(vecPos1.x, vecPos1.y, vecPos2.x, vecPos2.y, color);
}


void DrawManager::Rect(float posx1, float posy1, float posx2, float posy2, Color color) const
{
    D3DCOLOR dwColor = COL2DWORD(color);
    Vertex vert[5] =
    {   // Draw lines between declared points, needs primitive count as number of lines (4 here)
        { posx1, posy1, 0.0f, 1.0f, dwColor },  // Top left corner
        { posx2, posy1, 0.0f, 1.0f, dwColor },  // Top right corner
        { posx2, posy2, 0.0f, 1.0f, dwColor },  // Bottom right corner
        { posx1, posy2, 0.0f, 1.0f, dwColor },  // Bottom left corner
        { posx1, posy1, 0.0f, 1.0f, dwColor }   // Back to top left to finish drawing
    };

    this->pDevice->SetTexture(0, nullptr);
    this->pDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);  /* Disabled, cause corners of the rectangle get antialiased */
    this->pDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, &vert, sizeof(Vertex));
    this->pDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);   /* And enable again for the rest of the drawing */
}

void DrawManager::RectFilled(Vector2D vecPos1, Vector2D vecPos2, Color color) const
{
    this->RectFilled(vecPos1.x, vecPos1.y, vecPos2.x, vecPos2.y, color);
}

void DrawManager::RectFilled(float posx1, float posy1, float posx2, float posy2, Color color) const
{
    D3DCOLOR dwColor = COL2DWORD(color);
    Vertex vert[4] =
    {
        { posx1, posy1, 0.0f, 1.0f, dwColor },
        { posx2, posy1, 0.0f, 1.0f, dwColor },
        { posx1, posy2, 0.0f, 1.0f, dwColor },
        { posx2, posy2, 0.0f, 1.0f, dwColor }
    };
    
    this->pDevice->SetTexture(0, nullptr);
    this->pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &vert, sizeof(Vertex));
}

void DrawManager::Triangle(Vector2D pos1, Vector2D pos2, Vector2D pos3, Color color) const
{
    D3DCOLOR dwColor = COL2DWORD(color);
    Vertex vert[4] =
    {
        { pos1.x, pos1.y, 0.0f, 1.0f, dwColor },
        { pos2.x, pos2.y, 0.0f, 1.0f, dwColor },
        { pos3.x, pos3.y, 0.0f, 1.0f, dwColor },
        { pos1.x, pos1.y, 0.0f, 1.0f, dwColor }
    };

    this->pDevice->SetTexture(0, nullptr);
    this->pDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 3, &vert, sizeof(Vertex));
}


void DrawManager::TriangleFilled(Vector2D pos1, Vector2D pos2, Vector2D pos3, Color color) const
{
    D3DCOLOR dwColor = COL2DWORD(color);
    Vertex vert[3] = 
    {  
        { pos1.x, pos1.y, 0.0f, 1.0f, dwColor },
        { pos2.x, pos2.y, 0.0f, 1.0f, dwColor },
        { pos3.x, pos3.y, 0.0f, 1.0f, dwColor }
    };

    this->pDevice->SetTexture(0, nullptr);
    this->pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 1, &vert, sizeof(Vertex));
}


void DrawManager::RectFilledGradient(Vector2D vecPos1, Vector2D vecPos2, Color col1, Color col2, GradientType vertical) const
{
    D3DCOLOR dwColor  = COL2DWORD(col1);
    D3DCOLOR dwColor2 = COL2DWORD(col2);
    D3DCOLOR dwcol1, dwcol2, dwcol3, dwcol4;

    switch (vertical)
    {
        case GradientType::GRADIENT_VERTICAL:
            dwcol1 = dwColor;
            dwcol2 = dwColor;
            dwcol3 = dwColor2;
            dwcol4 = dwColor2;
            break;
        case GradientType::GRADIENT_HORIZONTAL:
            dwcol1 = dwColor;
            dwcol2 = dwColor2;
            dwcol3 = dwColor;
            dwcol4 = dwColor2;
            break;
        default:
            dwcol1 = D3DCOLOR_RGBA(255, 255, 255, 255);
            dwcol4 = dwcol3 = dwcol2 = dwcol1;
    }

    Vertex vert[4] =
    {
        { vecPos1.x, vecPos1.y, 0.0f, 1.0f, dwcol1 },
        { vecPos2.x, vecPos1.y, 0.0f, 1.0f, dwcol2 },
        { vecPos1.x, vecPos2.y, 0.0f, 1.0f, dwcol3 },
        { vecPos2.x, vecPos2.y, 0.0f, 1.0f, dwcol4 }
    };
  
    this->pDevice->SetTexture(0, nullptr);
    this->pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &vert, sizeof(Vertex));
}


void DrawManager::RectFilledMultiColor(const Vector2D& vecPos1, const Vector2D& vecPos2, Color colUprLeft, Color colUprRight, Color colBotLeft, Color colBotRight) const
{
    Vertex vert[4] =
    {
        { vecPos1.x, vecPos1.y, 0.0f, 1.0f, COL2DWORD(colUprLeft)  },
        { vecPos2.x, vecPos1.y, 0.0f, 1.0f, COL2DWORD(colUprRight) },
        { vecPos1.x, vecPos2.y, 0.0f, 1.0f, COL2DWORD(colBotLeft)  },
        { vecPos2.x, vecPos2.y, 0.0f, 1.0f, COL2DWORD(colBotRight) }
    };

    this->pDevice->SetTexture(0, nullptr);
    this->pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &vert, sizeof(Vertex));
}

void DrawManager::Circle(Vector2D pos, float r, float points, Color color) const
{
    Vertex* pVertex = new Vertex[static_cast<int>
        (points + 1)];
	for (int i = 0; i <= static_cast<int>(points); i++)
        pVertex[i] = 
        { pos.x + r * cos(D3DX_PI * (i / (points / 2.0f))),
          pos.y - r * sin(D3DX_PI * (i / (points / 2.0f))),
          0.0f,
          1.0f,
          COL2DWORD(color)
        };
	this->pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	this->pDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, static_cast<int>(points), pVertex, sizeof(Vertex));
	delete[] pVertex;
}


void DrawManager::String(Vector2D vecPos, DWORD dwFlags, Color color, CD3DFont* pFont, const char* szText, ...) const
{
    this->String(vecPos.x, vecPos.y, dwFlags, color, pFont, szText);
}


void DrawManager::String(float posx, float posy, DWORD dwFlags, Color color, CD3DFont* pFont, const char* szText, ...) const
{
    D3DCOLOR dwColor = COL2DWORD(color);
    posx = std::roundf(posx); posy = std::roundf(posy);
    pFont->DrawString(posx, posy, dwColor, szText, dwFlags);    // To make life easier
}


void DrawManager::String(float posx, float posy, float scaledx, float scaledy, DWORD dwFlags, Color color, CD3DFont* pFont, const char* szText, ...)const
{
    D3DCOLOR dwColor = COL2DWORD(color);
    posx = std::roundf(posx); posy = std::roundf(posy); scaledx = std::roundf(scaledx); scaledy = std::roundf(scaledy);
    pFont->DrawStringScaled(posx, posy, scaledx, scaledy, dwColor, szText, dwFlags);
}


int DrawManager::GetTextWitdh(char* text, CD3DFont* pFont) const
{
    SIZE sz;

    pFont->GetTextExtent(text, &sz);

    return sz.cx;
}


int DrawManager::GetTextHeight(char* text, CD3DFont* pFont) const
{
    SIZE sz;

    pFont->GetTextExtent(text, &sz);

    return sz.cy;
}


/*
*   SetupRenderStates - Sets RenderStates for our custom StateBlock
*   It's required to draw everything independent on game settings.
*/
void DrawManager::SetupRenderStates()
{
	this->pDevice->SetVertexShader(nullptr);
	this->pDevice->SetPixelShader(nullptr);
	this->pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	this->pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	this->pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	this->pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	this->pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	this->pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	this->pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
	this->pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	this->pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	this->pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	this->pDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);

	this->pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	this->pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	this->pDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	this->pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	this->pDevice->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
	this->pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	this->pDevice->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);

	this->pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
	this->pDevice->SetRenderState(D3DRS_COLORWRITEENABLE,
		D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN |
		D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
}


Vector2D DrawManager::GetScreenCenter()
{
    return Vector2D((static_cast<float>(this->pViewPort.Width)*0.5f), (static_cast<float>(this->pViewPort.Height)*0.5f));
}

