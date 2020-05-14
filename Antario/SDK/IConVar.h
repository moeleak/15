#pragma once

#include <cstdint>
#include "ICvar.h"
#include "..\Utils\Utils.h"

#pragma region fuckvalve (cutlvector)

inline int UtlMemory_CalcNewAllocationCount(int nAllocationCount, int nGrowSize, int nNewSize, int nBytesItem) {
    if (nGrowSize)
        nAllocationCount = ((1 + ((nNewSize - 1) / nGrowSize)) * nGrowSize);
    else {
        if (!nAllocationCount)
            nAllocationCount = (31 + nBytesItem) / nBytesItem;

        while (nAllocationCount < nNewSize)
            nAllocationCount *= 2;
    }

    return nAllocationCount;
}

template< class T, class I = int >
class CUtlMemory
{
public:
    T & operator[](I i)
    {
        return m_pMemory[i];
    }

    T* Base()
    {
        return m_pMemory;
    }

    int NumAllocated() const
    {
        return m_nAllocationCount;
    }

    void Grow(int num = 1)
    {
        if (IsExternallyAllocated())
            return;

        int nAllocationRequested = m_nAllocationCount + num;
        int nNewAllocationCount = UtlMemory_CalcNewAllocationCount(m_nAllocationCount, m_nGrowSize, nAllocationRequested, sizeof(T));

        if ((int)(I)nNewAllocationCount < nAllocationRequested)
        {
            if ((int)(I)nNewAllocationCount == 0 && (int)(I)(nNewAllocationCount - 1) >= nAllocationRequested)
                --nNewAllocationCount;
            else
            {
                if ((int)(I)nAllocationRequested != nAllocationRequested)
                    return;

                while ((int)(I)nNewAllocationCount < nAllocationRequested)
                    nNewAllocationCount = (nNewAllocationCount + nAllocationRequested) / 2;
            }
        }

        m_nAllocationCount = nNewAllocationCount;

        if (m_pMemory)
            m_pMemory = (T*)g_pMemAlloc->Realloc(m_pMemory, m_nAllocationCount * sizeof(T));
        else
            m_pMemory = (T*)g_pMemAlloc->Alloc(m_nAllocationCount * sizeof(T));
    }

    bool IsExternallyAllocated() const
    {
        return m_nGrowSize < 0;
    }

protected:
    T * m_pMemory;
    int m_nAllocationCount;
    int m_nGrowSize;
};

template <class T>
inline T* Construct(T* pMemory)
{
    return ::new(pMemory) T;
}

template <class T>
inline void Destruct(T* pMemory)
{
    pMemory->~T();
}

template< class T, class A = CUtlMemory<T> >
class CUtlVector {
    typedef A CAllocator;
public:
    T & operator[](int i)
    {
        return m_Memory[i];
    }

    T& Element(int i)
    {
        return m_Memory[i];
    }

    T* Base()
    {
        return m_Memory.Base();
    }

    int Count() const
    {
        return m_Size;
    }

    void RemoveAll()
    {
        for (int i = m_Size; --i >= 0; )
            Destruct(&Element(i));

        m_Size = 0;
    }

    int AddToTail()
    {
        return InsertBefore(m_Size);
    }

    void SetSize(int size)
    {
        m_Size = size;
    }

    int InsertBefore(int elem)
    {
        GrowVector();
        ShiftElementsRight(elem);
        Construct(&Element(elem));

        return elem;
    }

protected:
    void GrowVector(int num = 1)
    {
        if (m_Size + num > m_Memory.NumAllocated())
            m_Memory.Grow(m_Size + num - m_Memory.NumAllocated());

        m_Size += num;
        ResetDbgInfo();
    }

    void ShiftElementsRight(int elem, int num = 1)
    {
        int numToMove = m_Size - elem - num;
        if ((numToMove > 0) && (num > 0))
            memmove(&Element(elem + num), &Element(elem), numToMove * sizeof(T));
    }

    CAllocator m_Memory;
    int m_Size;

    T* m_pElements;

    inline void ResetDbgInfo()
    {
        m_pElements = Base();
    }
};

#pragma endregion

enum CVAR_FLAGS
{
    FCVAR_NONE = 0,
    FCVAR_UNREGISTERED = (1 << 0),
    FCVAR_DEVELOPMENTONLY = (1 << 1),
    FCVAR_GAMEDLL = (1 << 2),
    FCVAR_CLIENTDLL = (1 << 3),
    FCVAR_HIDDEN = (1 << 4),
    FCVAR_PROTECTED = (1 << 5),
    FCVAR_SPONLY = (1 << 6),
    FCVAR_ARCHIVE = (1 << 7),
    FCVAR_NOTIFY = (1 << 8),
    FCVAR_USERINFO = (1 << 9),
    FCVAR_PRINTABLEONLY = (1 << 10),
    FCVAR_UNLOGGED = (1 << 11),
    FCVAR_NEVER_AS_A_STRING = (1 << 12),
    FCVAR_REPLICATED = (1 << 13),
    FCVAR_CHEAT = (1 << 14),
    FCVAR_SS = (1 << 15),
    FCVAR_DEMO = (1 << 16),
    FCVAR_DONTRECORD = (1 << 17),
    FCVAR_SS_ADDED = (1 << 18),
    FCVAR_RELEASE = (1 << 19),
    FCVAR_RELOAD_MATERIALS = (1 << 20),
    FCVAR_RELOAD_TEXTURES = (1 << 21),
    FCVAR_NOT_CONNECT = (1 << 22),
    FCVAR_MATERIAL_SYSTEM_THREAD = (1 << 23),
    FCVAR_ARCHIVE_XBOX = (1 << 24),
    FCVAR_ACCESSIBLE_FROM_THREADS = (1 << 25),
    FCVAR_SERVER_CAN_EXECUTE = (1 << 28),
    FCVAR_SERVER_CANNOT_QUERY = (1 << 29),
    FCVAR_CLIENTCMD_CAN_EXECUTE = (1 << 30),
    FCVAR_MEME_DLL = (1 << 31),
    FCVAR_MATERIAL_THREAD_MASK = (FCVAR_RELOAD_MATERIALS | FCVAR_RELOAD_TEXTURES | FCVAR_MATERIAL_SYSTEM_THREAD)
};

class ConVar
{
public:
    char pad_0x0000[0x4]; //0x0000
    ConVar* pNext; //0x0004 
    __int32 bRegistered; //0x0008 
    char* pszName; //0x000C 
    char* pszHelpString; //0x0010 
    __int32 nFlags; //0x0014 
    char pad_0x0018[0x4]; //0x0018
    ConVar* pParent; //0x001C 
    char* pszDefaultValue; //0x0020 
    char* strString; //0x0024 
    __int32 StringLength; //0x0028 
    float fValue; //0x002C 
    __int32 nValue; //0x0030 
    __int32 bHasMin; //0x0034 
    float fMinVal; //0x0038 
    __int32 bHasMax; //0x003C 
    float fMaxVal; //0x0040 
    void* fnChangeCallbacks;

    void SetValue(const char* value)
    {
        return Utils::CallVFunc<14, void, const char*>(this, value);
    }

    void SetValue(float value)
    {
        return Utils::CallVFunc<15, void, float>(this, value);
    }

    void SetValue(int value)
    {
        return Utils::CallVFunc<16, void, int>(this, value);
    }

    void SetValue(bool value)
    {
        return Utils::CallVFunc<16, void, bool>(this, value);
    }

    float GetFloat()
    {
        auto temp = *(int*)(&fValue);
        auto temp_result = (int)(temp ^ (uintptr_t)this);

        return *(float*)(&temp_result);
    }

    int GetInt()
    {
        auto temp = *(int*)(&nValue);
        auto temp_result = (int)(temp ^ (uintptr_t)this);

        return *(int*)(&temp_result);
    }

    bool GetBool()
    {
        return !GetInt();
    }

};