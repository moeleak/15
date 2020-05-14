#pragma once
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>

#include "../SDK/Recv.h"

using fnv_t = unsigned;

class FnvHash
{
    static const fnv_t FNV_PRIME = 16777619u;
    static const fnv_t OFFSET_BASIS = 2166136261u;

    template <unsigned int N>
    static constexpr fnv_t fnvHashConst(const char(&str)[N], unsigned int I = N)
    {
        return static_cast<fnv_t>(1ULL * (I == 1 ? (OFFSET_BASIS ^ str[0]) : (fnvHashConst(str, I - 1) ^ str[I - 1])) * FNV_PRIME);
    }

    static fnv_t fnvHash(const char* str)
    {
        const auto length = strlen(str) + 1;
        auto hash = OFFSET_BASIS;
        for (size_t i = 0; i < length; ++i)
        {
            hash ^= *str++;
            hash *= FNV_PRIME;
        }
        return hash;
    }

    struct Wrapper
    {
        Wrapper(const char* str) : str(str) { }
        const char* str;
    };

    fnv_t hash_value;
public:
    FnvHash(Wrapper wrapper) : hash_value(fnvHash(wrapper.str)) { }

    template <unsigned int N>
    constexpr FnvHash(const char(&str)[N]) : hash_value(fnvHashConst(str)) { }

    constexpr operator fnv_t() const { return this->hash_value; }
};

class CNetVars
{
private:
    struct StoredPropData
    {
        RecvProp* prop_ptr;
        uint16_t class_relative_offset;
    };

public:
    static CNetVars& Get()
    {
        static CNetVars instance;
        return instance;
    }

    unsigned short GetOffset(unsigned int hash) { return m_props[hash].class_relative_offset; }
    RecvProp* GetPropPtr(unsigned int hash) { return m_props[hash].prop_ptr; }

    // Prevent instruction cache pollution caused by automatic
    // inlining of Get and GetOffset every netvar usage when there
    // are a lot of netvars
    __declspec(noinline) static uint16_t GetOffsetByHash(fnv_t hash)
    {
        return Get().GetOffset(hash);
    }

private:
    CNetVars();
    void DumpRecursive(const char* base_class, RecvTable* table, uint16_t offset);

private:
    std::map<fnv_t, StoredPropData> m_props;
};

#define PNETVAR(funcname, type, netvarname) type* funcname() \
{ \
	constexpr fnv_t hash = FnvHash(netvarname); \
	static uint16_t offset = 0; \
	if(!offset) offset = CNetVars::GetOffsetByHash(hash); \
	return reinterpret_cast<type*>(uintptr_t(this) + offset); \
}

#define NETVAR(funcname, type, netvarname) type& funcname() \
{ \
	constexpr fnv_t hash = FnvHash(netvarname); \
	static uint16_t offset = 0; \
	if(!offset) offset = CNetVars::GetOffsetByHash(hash); \
	return *reinterpret_cast<type*>(uintptr_t(this) + offset); \
}

#define NETPROP(funcname, netvarname) static RecvProp* funcname() \
{ \
	constexpr fnv_t hash = FnvHash(netvarname); \
	static auto prop_ptr = CNetVars::Get().GetPropPtr(hash); \
	return prop_ptr; \
}

#define OFFSET(funcname, type, offset) type& funcname() \
{ \
	static uint16_t _offset = offset; \
	return *reinterpret_cast<type*>(uintptr_t(this) + _offset); \
}

#define VFUNC(funcname, type, index) type funcname() \
{ \
	return U::GetVFunc<type( __thiscall* )( void* )>( this, index )( this ); \
}