// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: network_connection.proto

#include "network_connection.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// This is a temporary google only hack
#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
#include "third_party/protobuf/version.h"
#endif
// @@protoc_insertion_point(includes)
namespace protobuf_network_5fconnection_2eproto {
const ::google::protobuf::EnumDescriptor* file_level_enum_descriptors[1];
const ::google::protobuf::uint32 TableStruct::offsets[1] = {};
static const ::google::protobuf::internal::MigrationSchema* schemas = NULL;
static const ::google::protobuf::Message* const* file_default_instances = NULL;

void protobuf_AssignDescriptors() {
  AddDescriptors();
  ::google::protobuf::MessageFactory* factory = NULL;
  AssignDescriptors(
      "network_connection.proto", schemas, file_default_instances, TableStruct::offsets, factory,
      NULL, file_level_enum_descriptors, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_PROTOBUF_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
}

void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n\030network_connection.proto\032 google/proto"
      "buf/descriptor.proto*\327%\n\033ENetworkDisconn"
      "ectionReason\022\036\n\032NETWORK_DISCONNECT_INVAL"
      "ID\020\000\022\037\n\033NETWORK_DISCONNECT_SHUTDOWN\020\001\022F\n"
      "%NETWORK_DISCONNECT_DISCONNECT_BY_USER\020\002"
      "\032\033\242\324\030\027#GameUI_Disconnect_User\022J\n\'NETWORK"
      "_DISCONNECT_DISCONNECT_BY_SERVER\020\003\032\035\242\324\030\031"
      "#GameUI_Disconnect_Server\022B\n\027NETWORK_DIS"
      "CONNECT_LOST\020\004\032%\242\324\030!#GameUI_Disconnect_C"
      "onnectionLost\022J\n\033NETWORK_DISCONNECT_OVER"
      "FLOW\020\005\032)\242\324\030%#GameUI_Disconnect_Connectio"
      "nOverflow\022I\n\037NETWORK_DISCONNECT_STEAM_BA"
      "NNED\020\006\032$\242\324\030 #GameUI_Disconnect_SteamIDBa"
      "nned\022G\n\036NETWORK_DISCONNECT_STEAM_INUSE\020\007"
      "\032#\242\324\030\037#GameUI_Disconnect_SteamIDInUse\022G\n"
      "\037NETWORK_DISCONNECT_STEAM_TICKET\020\010\032\"\242\324\030\036"
      "#GameUI_Disconnect_SteamTicket\022E\n\036NETWOR"
      "K_DISCONNECT_STEAM_LOGON\020\t\032!\242\324\030\035#GameUI_"
      "Disconnect_SteamLogon\022M\n&NETWORK_DISCONN"
      "ECT_STEAM_AUTHCANCELLED\020\n\032!\242\324\030\035#GameUI_D"
      "isconnect_SteamLogon\022O\n(NETWORK_DISCONNE"
      "CT_STEAM_AUTHALREADYUSED\020\013\032!\242\324\030\035#GameUI_"
      "Disconnect_SteamLogon\022K\n$NETWORK_DISCONN"
      "ECT_STEAM_AUTHINVALID\020\014\032!\242\324\030\035#GameUI_Dis"
      "connect_SteamLogon\022I\n$NETWORK_DISCONNECT"
      "_STEAM_VACBANSTATE\020\r\032\037\242\324\030\033#GameUI_Discon"
      "nect_SteamVAC\022S\n,NETWORK_DISCONNECT_STEA"
      "M_LOGGED_IN_ELSEWHERE\020\016\032!\242\324\030\035#GameUI_Dis"
      "connect_SteamInUse\022T\n+NETWORK_DISCONNECT"
      "_STEAM_VAC_CHECK_TIMEDOUT\020\017\032#\242\324\030\037#GameUI"
      "_Disconnect_SteamTimeOut\022I\n NETWORK_DISC"
      "ONNECT_STEAM_DROPPED\020\020\032#\242\324\030\037#GameUI_Disc"
      "onnect_SteamDropped\022M\n\"NETWORK_DISCONNEC"
      "T_STEAM_OWNERSHIP\020\021\032%\242\324\030!#GameUI_Disconn"
      "ect_SteamOwnership\022U\n&NETWORK_DISCONNECT"
      "_SERVERINFO_OVERFLOW\020\022\032)\242\324\030%#GameUI_Disc"
      "onnect_ServerInfoOverflow\022K\n#NETWORK_DIS"
      "CONNECT_TICKMSG_OVERFLOW\020\023\032\"\242\324\030\036#GameUI_"
      "Disconnect_TickMessage\022Y\n*NETWORK_DISCON"
      "NECT_STRINGTABLEMSG_OVERFLOW\020\024\032)\242\324\030%#Gam"
      "eUI_Disconnect_StringTableMessage\022S\n\'NET"
      "WORK_DISCONNECT_DELTAENTMSG_OVERFLOW\020\025\032&"
      "\242\324\030\"#GameUI_Disconnect_DeltaEntMessage\022Q"
      "\n&NETWORK_DISCONNECT_TEMPENTMSG_OVERFLOW"
      "\020\026\032%\242\324\030!#GameUI_Disconnect_TempEntMessag"
      "e\022O\n%NETWORK_DISCONNECT_SOUNDSMSG_OVERFL"
      "OW\020\027\032$\242\324\030 #GameUI_Disconnect_SoundsMessa"
      "ge\022P\n#NETWORK_DISCONNECT_SNAPSHOTOVERFLO"
      "W\020\030\032\'\242\324\030##GameUI_Disconnect_SnapshotOver"
      "flow\022J\n NETWORK_DISCONNECT_SNAPSHOTERROR"
      "\020\031\032$\242\324\030 #GameUI_Disconnect_SnapshotError"
      "\022P\n#NETWORK_DISCONNECT_RELIABLEOVERFLOW\020"
      "\032\032\'\242\324\030##GameUI_Disconnect_ReliableOverfl"
      "ow\022N\n\037NETWORK_DISCONNECT_BADDELTATICK\020\033\032"
      ")\242\324\030%#GameUI_Disconnect_BadClientDeltaTi"
      "ck\022H\n\037NETWORK_DISCONNECT_NOMORESPLITS\020\034\032"
      "#\242\324\030\037#GameUI_Disconnect_NoMoreSplits\022@\n\033"
      "NETWORK_DISCONNECT_TIMEDOUT\020\035\032\037\242\324\030\033#Game"
      "UI_Disconnect_TimedOut\022H\n\037NETWORK_DISCON"
      "NECT_DISCONNECTED\020\036\032#\242\324\030\037#GameUI_Disconn"
      "ect_Disconnected\022H\n\037NETWORK_DISCONNECT_L"
      "EAVINGSPLIT\020\037\032#\242\324\030\037#GameUI_Disconnect_Le"
      "avingSplit\022X\n\'NETWORK_DISCONNECT_DIFFERE"
      "NTCLASSTABLES\020 \032+\242\324\030\'#GameUI_Disconnect_"
      "DifferentClassTables\022P\n#NETWORK_DISCONNE"
      "CT_BADRELAYPASSWORD\020!\032\'\242\324\030##GameUI_Disco"
      "nnect_BadRelayPassword\022X\n\'NETWORK_DISCON"
      "NECT_BADSPECTATORPASSWORD\020\"\032+\242\324\030\'#GameUI"
      "_Disconnect_BadSpectatorPassword\022L\n!NETW"
      "ORK_DISCONNECT_HLTVRESTRICTED\020#\032%\242\324\030!#Ga"
      "meUI_Disconnect_HLTVRestricted\022H\n\037NETWOR"
      "K_DISCONNECT_NOSPECTATORS\020$\032#\242\324\030\037#GameUI"
      "_Disconnect_NoSpectators\022N\n\"NETWORK_DISC"
      "ONNECT_HLTVUNAVAILABLE\020%\032&\242\324\030\"#GameUI_Di"
      "sconnect_HLTVUnavailable\022@\n\033NETWORK_DISC"
      "ONNECT_HLTVSTOP\020&\032\037\242\324\030\033#GameUI_Disconnec"
      "t_HLTVStop\022<\n\031NETWORK_DISCONNECT_KICKED\020"
      "\'\032\035\242\324\030\031#GameUI_Disconnect_Kicked\022@\n\033NETW"
      "ORK_DISCONNECT_BANADDED\020(\032\037\242\324\030\033#GameUI_D"
      "isconnect_BanAdded\022H\n\037NETWORK_DISCONNECT"
      "_KICKBANADDED\020)\032#\242\324\030\037#GameUI_Disconnect_"
      "KickBanAdded\022D\n\035NETWORK_DISCONNECT_HLTVD"
      "IRECT\020*\032!\242\324\030\035#GameUI_Disconnect_HLTVDire"
      "ct\022\\\n)NETWORK_DISCONNECT_PURESERVER_CLIE"
      "NTEXTRA\020+\032-\242\324\030)#GameUI_Disconnect_PureSe"
      "rver_ClientExtra\022V\n&NETWORK_DISCONNECT_P"
      "URESERVER_MISMATCH\020,\032*\242\324\030&#GameUI_Discon"
      "nect_PureServer_Mismatch\022>\n\032NETWORK_DISC"
      "ONNECT_USERCMD\020-\032\036\242\324\030\032#GameUI_Disconnect"
      "_UserCmd\022N\n#NETWORK_DISCONNECT_REJECTED_"
      "BY_GAME\020.\032%\242\324\030!#GameUI_Disconnect_Reject"
      "edByGame\022T\n&NETWORK_DISCONNECT_MESSAGE_P"
      "ARSE_ERROR\020/\032(\242\324\030$#GameUI_Disconnect_Mes"
      "sageParseError\022X\n(NETWORK_DISCONNECT_INV"
      "ALID_MESSAGE_ERROR\0200\032*\242\324\030&#GameUI_Discon"
      "nect_InvalidMessageError\022T\n&NETWORK_DISC"
      "ONNECT_BAD_SERVER_PASSWORD\0201\032(\242\324\030$#GameU"
      "I_Disconnect_BadServerPassword\022b\n-NETWOR"
      "K_DISCONNECT_DIRECT_CONNECT_RESERVATION\020"
      "2\032/\242\324\030+#GameUI_Disconnect_DirectConnectR"
      "eservation\022S\n%NETWORK_DISCONNECT_CONNECT"
      "ION_FAILURE\0203\032(\242\324\030$#GameUI_Disconnect_Co"
      "nnectionFailure\022Y\n)NETWORK_DISCONNECT_NO"
      "_PEER_GROUP_HANDLERS\0204\032*\242\324\030&#GameUI_Disc"
      "onnect_NoPeerGroupHandlers\022H\n\037NETWORK_DI"
      "SCONNECT_RECONNECTION\0205\032#\242\324\030\037#GameUI_Dis"
      "connect_Reconnection\022S\n%NETWORK_DISCONNE"
      "CT_CONNECTION_CLOSING\0206\032(\242\324\030$#GameUI_Dis"
      "connect_ConnectionClosing\022]\n+NETWORK_DIS"
      "CONNECT_NO_GOTV_RELAYS_AVAILABLE\0207\032,\242\324\030("
      "#GameUI_Disconnect_NoGOTVRelaysAvailable"
      "\022O\n#NETWORK_DISCONNECT_SESSION_MIGRATED\020"
      "8\032&\242\324\030\"#GameUI_Disconnect_SessionMigrate"
      "d\022b\n,NETWORK_DISCONNECT_VERYLARGETRANSFE"
      "ROVERFLOW\0209\0320\242\324\030,#GameUI_Disconnect_Very"
      "LargeTransferOverflow\022N\n\"NETWORK_DISCONN"
      "ECT_SENDNETOVERFLOW\020:\032&\242\324\030\"#GameUI_Disco"
      "nnect_SendNetOverflow\022l\n3NETWORK_DISCONN"
      "ECT_PLAYER_REMOVED_FROM_HOST_SESSION\020;\0323"
      "\242\324\030/#GameUI_Disconnect_PlayerRemovedFrom"
      "HostSession\022I\n\035NETWORK_DISCONNECT_SERVER"
      "_DOS\020=\032&\242\324\030\"#GameUI_Disconnect_TooManyCo"
      "mmands:E\n\030network_connection_token\022!.goo"
      "gle.protobuf.EnumValueOptions\030\304\212\003 \001(\tB\003\200"
      "\001\000"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 4962);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "network_connection.proto", &protobuf_RegisterTypes);
  ::protobuf_google_2fprotobuf_2fdescriptor_2eproto::AddDescriptors();
}

void AddDescriptors() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;
}  // namespace protobuf_network_5fconnection_2eproto
const ::google::protobuf::EnumDescriptor* ENetworkDisconnectionReason_descriptor() {
  protobuf_network_5fconnection_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_network_5fconnection_2eproto::file_level_enum_descriptors[0];
}
bool ENetworkDisconnectionReason_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
    case 40:
    case 41:
    case 42:
    case 43:
    case 44:
    case 45:
    case 46:
    case 47:
    case 48:
    case 49:
    case 50:
    case 51:
    case 52:
    case 53:
    case 54:
    case 55:
    case 56:
    case 57:
    case 58:
    case 59:
    case 61:
      return true;
    default:
      return false;
  }
}

const ::std::string network_connection_token_default("");
::google::protobuf::internal::ExtensionIdentifier< ::google::protobuf::EnumValueOptions,
    ::google::protobuf::internal::StringTypeTraits, 9, false >
  network_connection_token(kNetworkConnectionTokenFieldNumber, network_connection_token_default);

// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
