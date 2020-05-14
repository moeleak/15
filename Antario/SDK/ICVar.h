#pragma once

#include "IAppSystem.h"
#include "IConVar.h"
#include "Color.h"

class ConCommandBase;
class ConCommand;
class ConVar;

typedef int CVarDLLIdentifier_t;

class IConsoleDisplayFunc
{
public:
    virtual void ColorPrint(const uint8_t* clr, const char *pMessage) = 0;
    virtual void Print(const char *pMessage) = 0;
    virtual void DPrint(const char *pMessage) = 0;
};

class ICVar : public IAppSystem
{
public:
    ConVar* FindVar(const char *var_name)
    {
        return Utils::CallVFunc<15, ConVar*, const char*>(this, var_name);
    }

    void RegisterConCommand(ConVar* pCommandBase)
    {
        return Utils::CallVFunc<11, void, ConVar*>(this, pCommandBase);
    }

    void UnregisterConCommand(ConVar* pCommandBase)
    {
        return Utils::CallVFunc<12, void, ConVar*>(this, pCommandBase);
    }

    template <typename... Values>
    void ConsoleColorPrintf(const Color& MsgColor, const char* szMsgFormat, Values... Parameters)
    {
        typedef void(*oConsoleColorPrintf)(void*, const Color&, const char*, ...);
        return Utils::CallVFuncOld<oConsoleColorPrintf>(this, 25)(this, MsgColor, szMsgFormat, Parameters...);
    }

    void ConsolePrintf(const char* pFormat)
    {
        return Utils::CallVFunc<26, void, const char*>(this, pFormat);
    }
};

extern ICVar* g_pCVar;