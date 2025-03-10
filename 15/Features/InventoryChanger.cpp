#define CAST(cast, address, add) reinterpret_cast<cast>((uint32_t)address + (uint32_t)add)

#include "../Utils/Utils.h"
#include "../Utils/GlobalVars.h"
#include "InventoryChanger.h"

#include <intrin.h>

bool InventoryChanger::SendClientHello()
{
    CMsgClientHello Message;

    Message.set_client_session_need(1);
    Message.clear_socache_have_versions();

    void* ptr = malloc(Message.ByteSize() + 8);

    if (!ptr)
        return false;

    ((uint32_t*)ptr)[0] = k_EMsgGCClientHello | ((DWORD)1 << 31);
    ((uint32_t*)ptr)[1] = 0;

    Message.SerializeToArray((void*)((DWORD)ptr + 8), Message.ByteSize());

    bool result = g_pSteamGameCoordinator->SendMsg(k_EMsgGCClientHello | ((DWORD)1 << 31), ptr, Message.ByteSize() + 8) == k_EGCResultOK;

    free(ptr);

    return result;
}

bool InventoryChanger::PreSendMessage(uint32_t &unMsgType, void* pubData, uint32_t &cubData)
{
    uint32_t MessageType = unMsgType & 0x7FFFFFFF;

    if (MessageType == k_EMsgGCAdjustItemEquippedState)
    {
        CMsgAdjustItemEquippedState Message;

        try
        {
            if (!Message.ParsePartialFromArray((void*)((DWORD)pubData + 8), cubData - 8))
                return true;
        }
        catch (...)
        {
            return true;
        }

        if (!Message.has_item_id() || !Message.has_new_class() || !Message.has_new_slot())
            return true;

        ItemDefinitionIndex ItemIndex = (ItemDefinitionIndex)((uint32_t)Message.item_id() - 20000);

        if (ItemIndex < ItemDefinitionIndex::WEAPON_DEAGLE || ItemIndex > ItemDefinitionIndex::GLOVE_SPECIALIST)
            return true;

        return false;
    }

    return true;
}

void InventoryChanger::PostRetrieveMessage(uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize)
{
    uint32_t MessageType = *punMsgType & 0x7FFFFFFF;

    if (MessageType == k_EMsgGCClientWelcome)
    {
        CMsgClientWelcome Message;

        try
        {
            if (!Message.ParsePartialFromArray((void*)((DWORD)pubDest + 8), *pcubMsgSize - 8))
                return;
        }
        catch (...)
        {
            return;
        }

        if (Message.outofdate_subscribed_caches_size() <= 0)
            return;

        CMsgSOCacheSubscribed* Cache = Message.mutable_outofdate_subscribed_caches(0);

        for (int i = 0; i < Cache->objects_size(); i++)
        {
            CMsgSOCacheSubscribed::SubscribedType* Object = Cache->mutable_objects(i);

            if (!Object->has_type_id())
                continue;

            switch (Object->type_id())
            {
            case 1: // Inventory
            {
                uint64_t index = 0;

                for (int j = 0; j < Object->object_data_size(); j++)
                {
                    auto ObjectData = Object->mutable_object_data(j);

                    CSOEconItem Item;

                    if (!Item.ParseFromArray((void*)const_cast<char*>(ObjectData->data()), ObjectData->size()))
                        continue;

                    uint64_t id = Item.id();

                    if (index < id)
                        index = id;

                    try
                    {
                        if (Item.equipped_state_size() <= 0)
                            continue;
                    }
                    catch (...) { return; }

                    for (int s = 0; s < Item.equipped_state_size(); s++)
                    {
                        auto EquippedState = Item.mutable_equipped_state(s);

                        EquippedState->set_new_class(0);
                        EquippedState->set_new_slot(0);
                    }

                    ObjectData->resize(Item.ByteSize());
                    Item.SerializeToArray((void*)const_cast<char*>(ObjectData->data()), Item.ByteSize());
                }

            }
            break;
            }
        }

        if ((uint32_t)Message.ByteSize() <= cubDest - 8)
        {
            Message.SerializeToArray((void*)((DWORD)pubDest + 8), Message.ByteSize());

            *pcubMsgSize = Message.ByteSize() + 8;
        }
        else
        {
            void* thisPtr = nullptr;
            __asm mov thisPtr, ebx;

            auto oldEBP = *reinterpret_cast<void**>((uint32_t)_AddressOfReturnAddress() - 4);

            if (g_pMemAlloc)
            {
                auto memPtr = *CAST(void**, thisPtr, 0x14);
                auto memPtrSize = *CAST(uint32_t*, thisPtr, 0x18);
                auto newSize = (memPtrSize - cubDest) + Message.ByteSize() + 8;

                auto memAlloc = *(IMemAlloc**)g_pMemAlloc;
                auto memory = memAlloc->Realloc(memPtr, newSize + 4);

                *CAST(void**, thisPtr, 0x14) = memory;
                *CAST(uint32_t*, thisPtr, 0x18) = newSize;
                *CAST(void**, oldEBP, -0x14) = memory;

                Message.SerializeToArray(CAST(void*, memory, 24), Message.ByteSize());

                *pcubMsgSize = Message.ByteSize() + 8;
            }
        }
    }

    if (MessageType == k_EMsgGCCStrike15_v2_MatchmakingGC2ClientHello)
    {
        CMsgGCCStrike15_v2_MatchmakingGC2ClientHello Message;

        try
        {
            if (!Message.ParseFromArray((void*)((DWORD)pubDest + 8), *pcubMsgSize - 8))
                return;
        }
        catch (...)
        {
            return;
        }

        //绑定手机号后可成为优先账号
        //但并没有什么卵用 打mm的时候应该会搜索优先场 #待测试

        Message.set_player_level(40);
        Message.set_player_cur_xp(0);

        if ((uint32_t)Message.ByteSize() <= cubDest - 8)
        {
            Message.SerializeToArray((void*)((DWORD)pubDest + 8), Message.ByteSize());

            *pcubMsgSize = Message.ByteSize() + 8;
        }
        else
        {
            void* thisPtr = nullptr;
            __asm mov thisPtr, ebx;

            auto oldEBP = *reinterpret_cast<void**>((uint32_t)_AddressOfReturnAddress() - 4);

            if (g_pMemAlloc)
            {
                auto memPtr = *CAST(void**, thisPtr, 0x14);
                auto memPtrSize = *CAST(uint32_t*, thisPtr, 0x18);
                auto newSize = (memPtrSize - cubDest) + Message.ByteSize() + 8;

                auto memAlloc = *(IMemAlloc**)g_pMemAlloc;
                auto memory = memAlloc->Realloc(memPtr, newSize + 4);

                *CAST(void**, thisPtr, 0x14) = memory;
                *CAST(uint32_t*, thisPtr, 0x18) = newSize;
                *CAST(void**, oldEBP, -0x14) = memory;

                Message.SerializeToArray(CAST(void*, memory, 24), Message.ByteSize());

                *pcubMsgSize = Message.ByteSize() + 8;
            }
        }
    }

    if (MessageType == k_EMsgGCCStrike15_v2_MatchmakingGC2ClientReserve)
    {
        CMsgGCCStrike15_v2_MatchmakingGC2ClientReserve msg;

        try
        {
            if (!msg.ParsePartialFromArray((void*)((DWORD)pubDest + 8), *pcubMsgSize - 8))
                return;
        }
        catch (...)
        {
            return;
        }

        if (!msg.has_map())
            return;

        Utils::PrintMessageToGameConsole(Color::White(), msg.map().c_str());
    }

}

void InventoryChanger::Medals(CMsgSOCacheSubscribed::SubscribedType* pInventoryCacheObject, uint64_t& index, int itemIndex, bool active)
{
    if (itemIndex <= 0)
        return;

    index++;

    CSOEconItem Medal;
    Medal.set_id(index);
    Medal.set_account_id(g_pSteamUser->GetSteamID().GetAccountID());
    Medal.set_def_index(itemIndex);
    Medal.set_inventory(1);
    Medal.set_origin(9);
    Medal.set_rarity(6);
    Medal.set_quantity(1);
    Medal.set_quality(4);
    Medal.set_level(1);

    CSOEconItemAttribute* TimeAcquiredAttribute = Medal.add_attribute();
    uint32_t TimeAcquiredAttributeValue = 0;

    TimeAcquiredAttribute->set_def_index(222);

    TimeAcquiredAttribute->set_value_bytes(&TimeAcquiredAttributeValue, 4);

    if (active)
    {
        CSOEconItemEquipped* EquippedState = Medal.add_equipped_state();
        EquippedState->set_new_class(0);
        EquippedState->set_new_slot(55);
    }

    pInventoryCacheObject->add_object_data(Medal.SerializeAsString());
}
