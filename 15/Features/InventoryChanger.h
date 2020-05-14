#pragma once

#include "../SDK/Protobuf/base_gcmessages.pb.h"
#include "../SDK/Protobuf/cstrike15_gcmessages.pb.h"
#include "../SDK/Protobuf/cstrike15_usermessages.pb.h"
#include "../SDK/Protobuf/econ_gcmessages.pb.h"
#include "../SDK/Protobuf/engine_gcmessages.pb.h"
#include "../SDK/Protobuf/fatdemo.pb.h"
#include "../SDK/Protobuf/gcsdk_gcmessages.pb.h"
#include "../SDK/Protobuf/gcsystemmsgs.pb.h"
#include "../SDK/Protobuf/netmessages.pb.h"
#include "../SDK/Protobuf/network_connection.pb.h"
#include "../SDK/Protobuf/steamdatagram_messages.pb.h"
#include "../SDK/Protobuf/steammessages.pb.h"
#include "../SDK/Steam/ISteamUser.h"
#include "../SDK/IMemAlloc.h"

namespace InventoryChanger
{
    void PostRetrieveMessage(uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize);
    bool PreSendMessage(uint32_t &unMsgType, void* pubData, uint32_t &cubData);
    bool SendClientHello();

    void PostRetrieveMessage(uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize);
    void Medals(CMsgSOCacheSubscribed::SubscribedType* pInventoryCacheObject, uint64_t& index, int itemIndex, bool active = false);
};
