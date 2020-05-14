#pragma once
#include <cstdint>

enum EAccountType
{
	k_EAccountTypeInvalid = 0,
	k_EAccountTypeIndividual = 1,		// single user account
	k_EAccountTypeMultiseat = 2,		// multiseat (e.g. cybercafe) account
	k_EAccountTypeGameServer = 3,		// game server account
	k_EAccountTypeAnonGameServer = 4,	// anonymous game server account
	k_EAccountTypePending = 5,			// pending
	k_EAccountTypeContentServer = 6,	// content server
	k_EAccountTypeClan = 7,
	k_EAccountTypeChat = 8,
	k_EAccountTypeConsoleUser = 9,		// Fake SteamID for local PSN account on PS3 or Live account on 360, etc.
	k_EAccountTypeAnonUser = 10,

	// Max of 16 items in this field
	k_EAccountTypeMax
};

enum EGCResults
{
	k_EGCResultOK = 0,
	k_EGCResultNoMessage = 1,			// There is no message in the queue
	k_EGCResultBufferTooSmall = 2,		// The buffer is too small for the requested message
	k_EGCResultNotLoggedOn = 3,			// The client is not logged onto Steam
	k_EGCResultInvalidMessage = 4,		// Something was wrong with the message being sent with SendMessage
};

enum EUniverse
{
	k_EUniverseInvalid = 0,
	k_EUniversePublic = 1,
	k_EUniverseBeta = 2,
	k_EUniverseInternal = 3,
	k_EUniverseDev = 4,
	k_EUniverseRC = 5,

	k_EUniverseMax
};

class ISteamUser;
class ISteamGameServer;
class ISteamFriends;
class ISteamUtils;
class ISteamMatchmaking;
class ISteamContentServer;
class ISteamMatchmakingServers;
class ISteamUserStats;
class ISteamApps;
class ISteamNetworking;
class ISteamRemoteStorage;
class ISteamScreenshots;
class ISteamMusic;
class ISteamMusicRemote;
class ISteamGameServerStats;
class ISteamHTTP;
class ISteamUnifiedMessages;
class ISteamController;
class ISteamUGC;
class ISteamAppList;
class ISteamHTMLSurface;
class ISteamInventory;
class ISteamVideo;
class ISteamGameCoordinator;
class HSteamUser;
class CGameID;

typedef uint32_t SteamPipeHandle;
typedef uint32_t SteamUserHandle;
typedef uint64_t SteamAPICallHandle;

typedef void(__cdecl *SteamAPIWarningMessageHook_t)(int, const char *);
typedef void(*SteamAPI_PostAPIResultInProcess_t)(SteamAPICallHandle callHandle, void*, uint32_t unCallbackSize, int iCallbackNum);
typedef uint32_t(*SteamAPI_CheckCallbackRegistered_t)(int iCallbackNum);

/*const unsigned int k_unSteamAccountIDMask = 0xFFFFFFFF;
const unsigned int k_unSteamAccountInstanceMask = 0x000FFFFF;
// we allow 3 simultaneous user account instances right now, 1= desktop, 2 = console, 4 = web, 0 = all
const unsigned int k_unSteamUserDesktopInstance = 1;
const unsigned int k_unSteamUserConsoleInstance = 2;
const unsigned int k_unSteamUserWebInstance = 4;*/

typedef uint32_t AccountID_t;

class CSteamID
{
protected:
	union SteamID_t
	{
		struct SteamIDComponent_t
		{
			unsigned int m_unAccountID : 32;
			unsigned int m_unAccountInstance : 20;
			unsigned int m_EAccountType : 4;
			int m_EUniverse : 8;
		} m_comp;
		unsigned long long m_unAll64Bits;
	} m_steamid;
public:
	CSteamID(unsigned long int uiID, int nUniverse, int nAccountType)
	{
		m_steamid.m_comp.m_unAccountID = uiID;
		m_steamid.m_comp.m_EUniverse = nUniverse;
		m_steamid.m_comp.m_EAccountType = nAccountType;

		if (nAccountType == 7 || nAccountType == 3)
			m_steamid.m_comp.m_unAccountInstance = 0;
		else
			m_steamid.m_comp.m_unAccountInstance = 1;
	}

	uint32_t GetAccountID() const { return m_steamid.m_comp.m_unAccountID; }
	unsigned long long GetFullAccID() const { return m_steamid.m_unAll64Bits; }
public:
	enum Type
	{
		k_EAccountTypeInvalid = 0,
		k_EAccountTypeIndividual = 1,		// single user account
		k_EAccountTypeMultiseat = 2,		// multiseat (e.g. cybercafe) account
		k_EAccountTypeGameServer = 3,		// game server account
		k_EAccountTypeAnonGameServer = 4,	// anonymous game server account
		k_EAccountTypePending = 5,			// pending
		k_EAccountTypeContentServer = 6,	// content server
		k_EAccountTypeClan = 7,
		k_EAccountTypeChat = 8,
		k_EAccountTypeConsoleUser = 9,		// Fake SteamID for local PSN account on PS3 or Live account on 360, etc.
		k_EAccountTypeAnonUser = 10,

		// Max of 16 items in this field
		k_EAccountTypeMax
	};
};

/*class ISteamUser
{
public:
	// returns the CSteamID of the account currently logged into the Steam client
	// a CSteamID is a unique identifier for an account, and used to differentiate users in all parts of the Steamworks API
	CSteamID GetSteamID()
	{
		return Utils::CallVFunc<1337, CSteamID>(this);
	}
};*/

class ISteamUser
{
public:
	// returns the HSteamUser this interface represents
	virtual HSteamUser GetHSteamUser() = 0;

	// steam account management functions
	virtual bool BLoggedOn() = 0;
	virtual CSteamID GetSteamID() = 0;

	// notify of connection to game server
	virtual int InitiateGameConnection(void *pBlob, int cbMaxBlob, CSteamID steamID, CGameID gameID, uint32_t unIPServer, uint16_t usPortServer, bool bSecure, void *pvSteam2GetEncryptionKey, int cbSteam2GetEncryptionKey) = 0;
	// notify of disconnect
	virtual void TerminateGameConnection(uint32_t unIPServer, uint16_t usPortServer) = 0;

	// game info
	virtual void TrackAppUsageEvent(CGameID gameID, int eAppUsageEvent, const char *pchExtraInfo = "") = 0;

	// legacy authentication support - need to be called if the game server rejects the user with a 'bad ticket' error
	virtual void RefreshSteam2Login() = 0;
};

class ISteamClient
{
public:
	virtual SteamPipeHandle CreateSteamPipe() = 0;
	virtual bool BReleaseSteamPipe(SteamPipeHandle hSteamPipe) = 0;
	virtual SteamUserHandle ConnectToGlobalUser(SteamPipeHandle hSteamPipe) = 0;
	virtual SteamUserHandle CreateLocalUser(SteamPipeHandle *phSteamPipe, EAccountType eAccountType) = 0;
	virtual void ReleaseUser(SteamPipeHandle hSteamPipe, SteamUserHandle hUser) = 0;
	virtual ISteamUser *GetISteamUser(SteamUserHandle hSteamUser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamGameServer *GetISteamGameServer(SteamUserHandle hSteamUser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual void SetLocalIPBinding(uint32_t unIP, uint16_t usPort) = 0;
	virtual ISteamFriends *GetISteamFriends(SteamUserHandle hSteamUser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamUtils *GetISteamUtils(SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamMatchmaking *GetISteamMatchmaking(SteamUserHandle hSteamUser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamMatchmakingServers *GetISteamMatchmakingServers(SteamUserHandle hSteamUser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual void *GetISteamGenericInterface(SteamUserHandle hSteamUser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamUserStats *GetISteamUserStats(SteamUserHandle hSteamUser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamGameServerStats *GetISteamGameServerStats(SteamUserHandle hSteamuser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamApps *GetISteamApps(SteamUserHandle hSteamUser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamNetworking *GetISteamNetworking(SteamUserHandle hSteamUser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamRemoteStorage *GetISteamRemoteStorage(SteamUserHandle hSteamuser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamScreenshots *GetISteamScreenshots(SteamUserHandle hSteamuser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual void RunFrame() = 0;
	virtual uint32_t GetIPCCallCount() = 0;
	virtual void SetWarningMessageHook(SteamAPIWarningMessageHook_t pFunction) = 0;
	virtual bool ShutdownIfAllPipesClosed() = 0;
	virtual ISteamHTTP *GetISteamHTTP(SteamUserHandle hSteamuser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamUnifiedMessages *GetISteamUnifiedMessages(SteamUserHandle hSteamuser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamController *GetISteamController(SteamUserHandle hSteamUser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamUGC *GetISteamUGC(SteamUserHandle hSteamUser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamAppList *GetISteamAppList(SteamUserHandle hSteamUser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamMusic *GetISteamMusic(SteamUserHandle hSteamuser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamMusicRemote *GetISteamMusicRemote(SteamUserHandle hSteamuser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamHTMLSurface *GetISteamHTMLSurface(SteamUserHandle hSteamuser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual void Set_SteamAPI_CPostAPIResultInProcess(SteamAPI_PostAPIResultInProcess_t func) = 0;
	virtual void Remove_SteamAPI_CPostAPIResultInProcess(SteamAPI_PostAPIResultInProcess_t func) = 0;
	virtual void Set_SteamAPI_CCheckCallbackRegisteredInProcess(SteamAPI_CheckCallbackRegistered_t func) = 0;
	virtual ISteamInventory *GetISteamInventory(SteamUserHandle hSteamuser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
	virtual ISteamVideo *GetISteamVideo(SteamUserHandle hSteamuser, SteamPipeHandle hSteamPipe, const char *pchVersion) = 0;
};

class ISteamGameCoordinator
{
public:

	// sends a message to the Game Coordinator
	virtual EGCResults SendMsg(uint32_t unMsgType, const void *pubData, uint32_t cubData) = 0;

	// returns true if there is a message waiting from the game coordinator
	virtual bool IsMessageAvailable(uint32_t *pcubMsgSize) = 0;

	// fills the provided buffer with the first message in the queue and returns k_EGCResultOK or 
	// returns k_EGCResultNoMessage if there is no message waiting. pcubMsgSize is filled with the message size.
	// If the provided buffer is not large enough to fit the entire message, k_EGCResultBufferTooSmall is returned
	// and the message remains at the head of the queue.
	virtual EGCResults RetrieveMessage(uint32_t *punMsgType, void *pubDest, uint32_t cubDest, uint32_t *pcubMsgSize) = 0;

};

extern SteamUserHandle			g_hSteamUser;
extern SteamPipeHandle			g_hSteamPipe;

extern ISteamClient* g_pSteamClient;
extern ISteamHTTP* g_pSteamHTTP;
extern ISteamUser* g_pSteamUser;
extern ISteamFriends* g_pSteamFriends;
extern ISteamInventory* g_pSteamInventory;
extern ISteamGameCoordinator* g_pSteamGameCoordinator;
