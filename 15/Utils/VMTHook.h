#pragma once

#define NOMINMAX
#include <Windows.h>

#include <cstdint>
#include <stdexcept>

namespace detail
{
    class protectGuard
    {
    public:
        protectGuard(void* base, size_t len, std::uint32_t flags)
        {
            _base = base;
            _length = len;
            if (!VirtualProtect(base, len, flags, (PDWORD)&_old))
                throw std::runtime_error("Failed to protect region.");
        }
        ~protectGuard()
        {
            VirtualProtect(_base, _length, _old, (PDWORD)&_old);
        }

    private:
        void*         _base;
        size_t        _length;
        std::uint32_t _old;
    };
}

class VMTHook
{
public:
    uintptr_t* search_free_data_page(const char* moduleName, const std::size_t vmt_size);
    VMTHook(void* base, const char* moduleName = nullptr);
    ~VMTHook();

    bool Setup(void* base, const char* moduleName = nullptr);

    template<typename T>
    void Hook(int index, T fun)
    {
        assert(index >= 0 && index <= (int)vftbl_len);
        new_vftb1[index + 1] = reinterpret_cast<std::uintptr_t>(fun);
    }

    void Unhook(int index)
    {
        new_vftb1[index] = old_vftbl[index];
    }

    void UnhookAll()
    {
        try
        {
            if (old_vftbl != nullptr)
            {
                auto guard = detail::protectGuard { class_base, sizeof(std::uintptr_t), PAGE_READWRITE };
                *(std::uintptr_t**)class_base = old_vftbl;
                old_vftbl = nullptr;
            }
        }
        catch (...) { }
    }

    template<typename T>
    T GetOriginal(int index)
    {
        return (T)old_vftbl[index];
    }

    void Clear()
    {
        class_base = nullptr;
    }

private:
    static inline std::size_t estimate_vftbl_length(std::uintptr_t* vftbl_start);

    void*           class_base;
    std::size_t     vftbl_len;
    std::uintptr_t* new_vftb1;
    std::uintptr_t* old_vftbl;
    LPCVOID         search_base = nullptr;
    bool wasAllocated = false;
};