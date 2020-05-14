#include "NetvarManager.h"
#include "../SDK/IBaseClientDll.h"

CNetVars::CNetVars()
{
    for (auto clazz = g_pClientDll->GetAllClasses(); clazz; clazz = clazz->pNext)
        if (clazz->pRecvTable)
            DumpRecursive(clazz->pNetworkName, clazz->pRecvTable, 0);
}

void CNetVars::DumpRecursive(const char* base_class, RecvTable* table, uint16_t offset)
{
    for (auto i = 0; i < table->nProps; ++i)
    {
        auto prop_ptr = &table->pProps[i];

        //Skip trash array items
        if (!prop_ptr || isdigit(prop_ptr->pVarName[0]))
            continue;

        //We dont care about the base class, we already know that
        if (strcmp(prop_ptr->pVarName, "baseclass") == 0)
            continue;

        if (prop_ptr->RecvType == DPT_DataTable &&
            prop_ptr->pDataTable != nullptr &&
            prop_ptr->pDataTable->pNetTableName[0] == 'D') // Skip shitty tables
        {
            DumpRecursive(base_class, prop_ptr->pDataTable, offset + prop_ptr->Offset);
        }

        char hash_name[256];

        strcpy_s(hash_name, base_class);
        strcat_s(hash_name, "->");
        strcat_s(hash_name, prop_ptr->pVarName);

        // Need to cast it to prevent FnvHash using the recursive hasher
        // which would hash all 256 bytes
        auto hash = FnvHash(static_cast<const char*>(hash_name));

        m_props[hash] = { prop_ptr,  uint16_t(offset + prop_ptr->Offset) };
    }
}
