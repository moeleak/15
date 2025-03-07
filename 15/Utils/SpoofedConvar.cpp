#include "SpoofedConvar.h"

#include "../SDK/SDK.h"


SpoofedConvar::SpoofedConvar(const char* szCVar)
{
    m_pOriginalCVar = g_pCVar->FindVar(szCVar);
    Spoof();
}

SpoofedConvar::SpoofedConvar(ConVar* pCVar) 
{
    m_pOriginalCVar = pCVar;
    Spoof();
}

SpoofedConvar::~SpoofedConvar()
{
    if (IsSpoofed())
    {
        DWORD dwOld;

        //Restore flags
        SetFlags(m_iOriginalFlags);
        //Restore value
        SetString(m_szOriginalValue);
        //Restore the name
        VirtualProtect((LPVOID)m_pOriginalCVar->pszName, 128, PAGE_READWRITE, &dwOld);
        strcpy((char*)m_pOriginalCVar->pszName, m_szOriginalName);
        VirtualProtect((LPVOID)m_pOriginalCVar->pszName, 128, dwOld, &dwOld);

        //Unregister dummy cvar
        g_pCVar->UnregisterConCommand(m_pDummyCVar);
        free(m_pDummyCVar);
        m_pDummyCVar = nullptr;
    }
}
bool SpoofedConvar::IsSpoofed()
{
    return m_pDummyCVar != nullptr;
}

void SpoofedConvar::Spoof()
{
    if (IsSpoofed() || !m_pOriginalCVar)
        return;

    //Save old name value and flags so we can restore the cvar lates if needed
    m_iOriginalFlags = m_pOriginalCVar->nFlags;
    strcpy(m_szOriginalName, m_pOriginalCVar->pszName);
    strcpy(m_szOriginalValue, m_pOriginalCVar->strString);

    sprintf_s(m_szDummyName, 128, "dummy_%s", m_szOriginalName);

    //Create the dummy cvar
    m_pDummyCVar = (ConVar*)malloc(sizeof(ConVar));
    if (!m_pDummyCVar) return;
    memcpy(m_pDummyCVar, m_pOriginalCVar, sizeof(ConVar));

    m_pDummyCVar->pNext = nullptr;
    //Register it
    g_pCVar->RegisterConCommand(m_pDummyCVar);

    //Fix "write access violation" bullshit
    DWORD dwOld;
    VirtualProtect((LPVOID)m_pOriginalCVar->pszName, 128, PAGE_READWRITE, &dwOld);

    //Rename the cvar
    strcpy((char*)m_pOriginalCVar->pszName, m_szDummyName);

    VirtualProtect((LPVOID)m_pOriginalCVar->pszName, 128, dwOld, &dwOld);

    SetFlags(FCVAR_NONE);
}

void SpoofedConvar::SetFlags(int flags)
{
    if (!IsSpoofed())
        return;
    m_pOriginalCVar->nFlags = flags;
}

int SpoofedConvar::GetFlags()
{
    if (!IsSpoofed())
        return 0;
    return m_pOriginalCVar->nFlags;
}

void SpoofedConvar::SetValue(int iValue)
{
    if (!IsSpoofed())
        return;
    m_pOriginalCVar->SetValue(iValue);
}

void SpoofedConvar::SetFloat(float flValue)
{
    if (!IsSpoofed())
        return;
    m_pOriginalCVar->SetValue(flValue);
}

void SpoofedConvar::SetString(const char* szValue)
{
    if (!IsSpoofed())
        return;
        m_pOriginalCVar->SetValue(szValue);
}

int SpoofedConvar::GetValue()
{
    if (!IsSpoofed())
        return 0;
    return m_pOriginalCVar->GetInt();
}

float SpoofedConvar::GetFloat()
{
    if (!IsSpoofed())
        return 0.f;
    return m_pOriginalCVar->GetFloat();
}

const char* SpoofedConvar::GetString()
{
    if (!IsSpoofed())
        return nullptr;
    return m_pOriginalCVar->strString;
}