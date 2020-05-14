#pragma once
#include <Windows.h>

typedef void*   (*CreateInterfaceFn)        (const char *pName, int *pReturnCode);
typedef void(*pfnDemoCustomDataCallback)(unsigned char *pData, size_t iSize);

enum class EClassIds : int
{
    CAI_BaseNPC = 0,
    CAK47,
    CBaseAnimating,
    CBaseAnimatingOverlay,
    CBaseAttributableItem,
    CBaseButton,
    CBaseCombatCharacter,
    CBaseCombatWeapon,
    CBaseCSGrenade,
    CBaseCSGrenadeProjectile,
    CBaseDoor,
    CBaseEntity,
    CBaseFlex,
    CBaseGrenade,
    CBaseParticleEntity,
    CBasePlayer,
    CBasePropDoor,
    CBaseTeamObjectiveResource,
    CBaseTempEntity,
    CBaseToggle,
    CBaseTrigger,
    CBaseViewModel,
    CBaseVPhysicsTrigger,
    CBaseWeaponWorldModel,
    CBeam,
    CBeamSpotlight,
    CBoneFollower,
    CBreakableProp,
    CBreakableSurface,
    CC4,
    CCascadeLight,
    CChicken,
    CColorCorrection,
    CColorCorrectionVolume,
    CCSGameRulesProxy,
    CCSPlayer,
    CCSPlayerResource,
    CCSRagdoll,
    CCSTeam,
    CDEagle,
    CDecoyGrenade,
    CDecoyProjectile,
    CDynamicLight,
    CDynamicProp,
    CEconEntity,
    CEconWearable,
    CEmbers,
    CEntityDissolve,
    CEntityFlame,
    CEntityFreezing,
    CEntityParticleTrail,
    CEnvAmbientLight,
    CEnvDetailController,
    CEnvDOFController,
    CEnvParticleScript,
    CEnvProjectedTexture,
    CEnvQuadraticBeam,
    CEnvScreenEffect,
    CEnvScreenOverlay,
    CEnvTonemapController,
    CEnvWind,
    CFEPlayerDecal,
    CFireCrackerBlast,
    CFireSmoke,
    CFireTrail,
    CFish,
    CFlashbang,
    CFogController,
    CFootstepControl,
    CFunc_Dust,
    CFunc_LOD,
    CFuncAreaPortalWindow,
    CFuncBrush,
    CFuncConveyor,
    CFuncLadder,
    CFuncMonitor,
    CFuncMoveLinear,
    CFuncOccluder,
    CFuncReflectiveGlass,
    CFuncRotating,
    CFuncSmokeVolume,
    CFuncTrackTrain,
    CGameRulesProxy,
    CHandleTest,
    CHEGrenade,
    CHostage,
    CHostageCarriableProp,
    CIncendiaryGrenade,
    CInferno,
    CInfoLadderDismount,
    CInfoOverlayAccessor,
    CItem_Healthshot,
    CItemDogtags,
    CKnife,
    CKnifeGG,
    CLightGlow,
    CMaterialModifyControl,
    CMolotovGrenade,
    CMolotovProjectile,
    CMovieDisplay,
    CParticleFire,
    CParticlePerformanceMonitor,
    CParticleSystem,
    CPhysBox,
    CPhysBoxMultiplayer,
    CPhysicsProp,
    CPhysicsPropMultiplayer,
    CPhysMagnet,
    CPlantedC4,
    CPlasma,
    CPlayerResource,
    CPointCamera,
    CPointCommentaryNode,
    CPointWorldText,
    CPoseController,
    CPostProcessController,
    CPrecipitation,
    CPrecipitationBlocker,
    CPredictedViewModel,
    CProp_Hallucination,
    CPropDoorRotating,
    CPropJeep,
    CPropVehicleDriveable,
    CRagdollManager,
    CRagdollProp,
    CRagdollPropAttached,
    CRopeKeyframe,
    CSCAR17,
    CSceneEntity,
    CSensorGrenade,
    CSensorGrenadeProjectile,
    CShadowControl,
    CSlideshowDisplay,
    CSmokeGrenade,
    CSmokeGrenadeProjectile,
    CSmokeStack,
    CSpatialEntity,
    CSpotlightEnd,
    CSprite,
    CSpriteOriented,
    CSpriteTrail,
    CStatueProp,
    CSteamJet,
    CSun,
    CSunlightShadowControl,
    CTeam,
    CTeamplayRoundBasedRulesProxy,
    CTEArmorRicochet,
    CTEBaseBeam,
    CTEBeamEntPoint,
    CTEBeamEnts,
    CTEBeamFollow,
    CTEBeamLaser,
    CTEBeamPoints,
    CTEBeamRing,
    CTEBeamRingPoint,
    CTEBeamSpline,
    CTEBloodSprite,
    CTEBloodStream,
    CTEBreakModel,
    CTEBSPDecal,
    CTEBubbles,
    CTEBubbleTrail,
    CTEClientProjectile,
    CTEDecal,
    CTEDust,
    CTEDynamicLight,
    CTEEffectDispatch,
    CTEEnergySplash,
    CTEExplosion,
    CTEFireBullets,
    CTEFizz,
    CTEFootprintDecal,
    CTEFoundryHelpers,
    CTEGaussExplosion,
    CTEGlowSprite,
    CTEImpact,
    CTEKillPlayerAttachments,
    CTELargeFunnel,
    CTEMetalSparks,
    CTEMuzzleFlash,
    CTEParticleSystem,
    CTEPhysicsProp,
    CTEPlantBomb,
    CTEPlayerAnimEvent,
    CTEPlayerDecal,
    CTEProjectedDecal,
    CTERadioIcon,
    CTEShatterSurface,
    CTEShowLine,
    CTesla,
    CTESmoke,
    CTESparks,
    CTESprite,
    CTESpriteSpray,
    CTest_ProxyToggle_Networkable,
    CTestTraceline,
    CTEWorldDecal,
    CTriggerPlayerMovement,
    CTriggerSoundOperator,
    CVGuiScreen,
    CVoteController,
    CWaterBullet,
    CWaterLODControl,
    CWeaponAug,
    CWeaponAWP,
    CWeaponBaseItem,
    CWeaponBizon,
    CWeaponCSBase,
    CWeaponCSBaseGun,
    CWeaponCycler,
    CWeaponElite,
    CWeaponFamas,
    CWeaponFiveSeven,
    CWeaponG3SG1,
    CWeaponGalil,
    CWeaponGalilAR,
    CWeaponGlock,
    CWeaponHKP2000,
    CWeaponM249,
    CWeaponM3,
    CWeaponM4A1,
    CWeaponMAC10,
    CWeaponMag7,
    CWeaponMP5Navy,
    CWeaponMP7,
    CWeaponMP9,
    CWeaponNegev,
    CWeaponNOVA,
    CWeaponP228,
    CWeaponP250,
    CWeaponP90,
    CWeaponSawedoff,
    CWeaponSCAR20,
    CWeaponScout,
    CWeaponSG550,
    CWeaponSG552,
    CWeaponSG556,
    CWeaponSSG08,
    CWeaponTaser,
    CWeaponTec9,
    CWeaponTMP,
    CWeaponUMP45,
    CWeaponUSP,
    CWeaponXM1014,
    CWorld,
    DustTrail,
    MovieExplosion,
    ParticleSmokeGrenade,
    RocketTrail,
    SmokeTrail,
    SporeExplosion,
    SporeTrail
};

enum class MoveType_t
{
    MOVETYPE_NONE = 0,
    MOVETYPE_ISOMETRIC,
    MOVETYPE_WALK,
    MOVETYPE_STEP,
    MOVETYPE_FLY,
    MOVETYPE_FLYGRAVITY,
    MOVETYPE_VPHYSICS,
    MOVETYPE_PUSH,
    MOVETYPE_NOCLIP,
    MOVETYPE_LADDER,
    MOVETYPE_OBSERVER,
    MOVETYPE_CUSTOM,
    MOVETYPE_LAST = MOVETYPE_CUSTOM,
    MOVETYPE_MAX_BITS = 4
};

enum EntityFlags : int
{
    FL_ONGROUND = (1 << 0),
    FL_DUCKING = (1 << 1),
    FL_WATERJUMP = (1 << 2),
    FL_ONTRAIN = (1 << 3),
    FL_INRAIN = (1 << 4),
    FL_FROZEN = (1 << 5),
    FL_ATCONTROLS = (1 << 6),
    FL_CLIENT = (1 << 7),
    FL_FAKECLIENT = (1 << 8)
};

enum class ItemDefinitionIndex : int
{
    WEAPON_DEAGLE = 1,
    WEAPON_ELITE = 2,
    WEAPON_FIVESEVEN = 3,
    WEAPON_GLOCK = 4,
    WEAPON_AK47 = 7,
    WEAPON_AUG = 8,
    WEAPON_AWP = 9,
    WEAPON_FAMAS = 10,
    WEAPON_G3SG1 = 11,
    WEAPON_GALILAR = 13,
    WEAPON_M249 = 14,
    WEAPON_M4A1 = 16,
    WEAPON_MAC10 = 17,
    WEAPON_P90 = 19,
    WEAPON_UMP45 = 24,
    WEAPON_XM1014 = 25,
    WEAPON_BIZON = 26,
    WEAPON_MAG7 = 27,
    WEAPON_NEGEV = 28,
    WEAPON_SAWEDOFF = 29,
    WEAPON_TEC9 = 30,
    WEAPON_TASER = 31,
    WEAPON_HKP2000 = 32,
    WEAPON_MP7 = 33,
    WEAPON_MP9 = 34,
    WEAPON_NOVA = 35,
    WEAPON_P250 = 36,
    WEAPON_SCAR20 = 38,
    WEAPON_SG556 = 39,
    WEAPON_SSG08 = 40,
    WEAPON_KNIFE = 42,
    WEAPON_FLASHBANG = 43,
    WEAPON_HEGRENADE = 44,
    WEAPON_SMOKEGRENADE = 45,
    WEAPON_MOLOTOV = 46,
    WEAPON_DECOY = 47,
    WEAPON_INCGRENADE = 48,
    WEAPON_C4 = 49,
    WEAPON_KNIFE_T = 59,
    WEAPON_M4A1_SILENCER = 60,
    WEAPON_USP_SILENCER = 61,
    WEAPON_CZ75A = 63,
    WEAPON_REVOLVER = 64,
    WEAPON_KNIFE_BAYONET = 500,
    WEAPON_KNIFE_FLIP = 505,
    WEAPON_KNIFE_GUT = 506,
    WEAPON_KNIFE_KARAMBIT = 507,
    WEAPON_KNIFE_M9_BAYONET = 508,
    WEAPON_KNIFE_TACTICAL = 509,
    WEAPON_KNIFE_FALCHION = 512,
    WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
    WEAPON_KNIFE_BUTTERFLY = 515,
    WEAPON_KNIFE_PUSH = 516,
    GLOVE_STUDDED_BLOODHOUND = 5027,
    GLOVE_T_SIDE = 5028,
    GLOVE_CT_SIDE = 5029,
    GLOVE_SPORTY = 5030,
    GLOVE_SLICK = 5031,
    GLOVE_LEATHER_WRAP = 5032,
    GLOVE_MOTORCYCLE = 5033,
    GLOVE_SPECIALIST = 5034
};

enum class CSWeaponType : int
{
    WEAPONTYPE_KNIFE = 0,
    WEAPONTYPE_PISTOL,
    WEAPONTYPE_SUBMACHINEGUN,
    WEAPONTYPE_RIFLE,
    WEAPONTYPE_SHOTGUN,
    WEAPONTYPE_SNIPER_RIFLE,
    WEAPONTYPE_MACHINEGUN,
    WEAPONTYPE_C4,
    WEAPONTYPE_PLACEHOLDER,
    WEAPONTYPE_GRENADE,
    WEAPONTYPE_UNKNOWN
};

enum class HitBox : int
{
    HITBOX_HEAD,
    HITBOX_NECK,
    HITBOX_PELVIS,
    HITBOX_BELLY,
    HITBOX_THORAX,
    HITBOX_LOWER_CHEST,
    HITBOX_UPPER_CHEST,
    HITBOX_RIGHT_THIGH,
    HITBOX_LEFT_THIGH,
    HITBOX_RIGHT_CALF,
    HITBOX_LEFT_CALF,
    HITBOX_RIGHT_FOOT,
    HITBOX_LEFT_FOOT,
    HITBOX_RIGHT_HAND,
    HITBOX_LEFT_HAND,
    HITBOX_RIGHT_UPPER_ARM,
    HITBOX_RIGHT_FOREARM,
    HITBOX_LEFT_UPPER_ARM,
    HITBOX_LEFT_FOREARM,
    HITBOX_MAX
};

enum class Medals : int
{
    DREAMHACK_STEELSERIES_CHAMPION_2013 = 875,
    DREAMHACK_STEELSERIES_FINALIST_2013,
    DREAMHACK_STEELSERIES_SEMIFINALIST_2013,
    DREAMHACK_STEELSERIES_QUARTERFINALISTFINALIST_2013,

    EMS_ONE_KATOWICE_CHAMPION_2014,
    EMS_ONE_KATOWICE_FINALIST_2014,
    EMS_ONE_KATOWICE_SEMIFINALIST_2014,
    EMS_ONE_KATOWICE_QUARTERFINALIST_2014,

    ESL_ONE_COLOGNE_CHAMPION_2014,
    ESL_ONE_COLOGNE_FINALIST_2014,
    ESL_ONE_COLOGNE_SEMIFINALIST_2014,
    ESL_ONE_COLOGNE_QUARTERFINALIST_2014,

    DREAMHACK_WINTER_CHAMPION_2014 = 890,
    DREAMHACK_WINTER_FINALIST_2014,
    DREAMHACK_WINTER_SEMIFINALIST_2014,
    DREAMHACK_WINTER_QUARTERFINALIST_2014,

    ESL_ONE_KATOWICE_CHAMPION_2015 = 897,
    ESL_ONE_KATOWICE_FINALIST_2015,
    ESL_ONE_KATOWICE_SEMIFINALIST_2015,
    ESL_ONE_KATOWICE_QUARTERFINALIST_2015,

    ESL_ONE_COLOGNE_CHAMPION_2015 = 904,
    ESL_ONE_COLOGNE_FINALIST_2015,
    ESL_ONE_COLOGNE_SEMIFINALIST_2015,
    ESL_ONE_COLOGNE_QUARTERFINALST_2015,

    DREAMHACK_CLUJNAPOCA_CHAMPION_2015 = 917,
    DREAMAHCK_CLUJNAPOCA_FINALIST_2015,
    DREAMAHCK_CLUJNAPOCA_SEMIFINALIST_2015,
    DREAMAHCK_CLUJNAPOCA_QUARTERFINALIST_2015,

    MLG_COLUMBUS_CHAMPION_2016 = 927,
    MLG_COLUMBUS_FINALIST_2016,
    MLG_COLUMBUS_SEMIFINALIST_2016,
    MLG_COLUMBUS_QUARTERFINALIST_2016,

    ESL_ONE_COLOGNE_CHAMPION_2016,
    ESL_ONE_COLOGNE_FINALIST_2016,
    ESL_ONE_COLOGNE_SEMIFINALIST_2016,
    ESL_ONE_COLOGNE_QUARTERFINALIST_2016,

    ELEAGUE_ATLANTA_CHAMPION_2017 = 941,
    ELEAGUE_ATLANTA_FINALIST_2017,
    ELEAGUE_ATLANTA_SEMIFINALST_2017,
    ELEAGUE_ATLANTA_QUATERFINALST_2017,

    PGL_KRAKOW_CHAMPION_2017 = 948,
    PGL_KRAKOW_FINALIST_2017,
    PGL_KRAKOW_SEMIFINALIST_2017,
    PGL_KRAKOW_QUARTERFINALIST_2017,

    ELEAGUE_BOSTON_CHAMPION_2018 = 955,
    ELEAGUE_BOSTON_FINALIST_2018,
    ELEAGUE_BOSTON_SEMIFINALIST_2018,
    ELEAGUE_BOSTON_QUATERFINALIST_2018,

    PAYBACK_CHALLENGE_COIN_OPERATION = 1002,
    PAYBACK_CHALLENGE_COIN_SILVER,
    PAYBACK_CHALLENGE_COIN_GOLD,

    BRAVO_CHALLENGE_COIN_OPERATION = 1013,
    BRAVO_CHALLENGE_COIN_SILVER,
    BRAVO_CHALLENGE_COIN_GOLD,

    PHOENIX_CHALLENGE_COIN_OPERATION = 1024,
    PHOENIX_CHALLENGE_COIN_SILVER,
    PHOENIX_CHALLENGE_COIN_GOLD,

    BREAKOUT_CHALLENGE_COIN_OPERATION = 1028,
    BREAKOUT_CHALLENGE_COIN_SILVER,
    BREAKOUT_CHALLENGE_COIN_GOLD,

    VANGUARD_CHALLENGE_COIN_OPERATION = 1316,
    VANGUARD_CHALLENGE_COIN_SILVER,
    VANGUARD_CHALLENGE_COIN_GOLD,

    WILDFIRE_CHALLENGE_COIN_OPERATION = 1316,
    WILDFIRE_CHALLENGE_COIN_SILVER,
    WILDFIRE_CHALLENGE_COIN_GOLD,

    HYDRA_CHALLENGE_COIN_OPERATION = 4353,
    HYDRA_CHALLENGE_COIN_SILVER,
    HYDRA_CHALLENGE_COIN_GOLD,
    HYDRA_CHALLENGE_COIN_DIAMOND,

    SERVICE_MEDAL_2015_1 = 1331,
    SERVICE_MEDAL_2015_2,

    SERVICE_MEDAL_2016_1 = 1339,
    SERVICE_MEDAL_2016_2,
    SERVICE_MEDAL_2016_3,
    SERVICE_MEDAL_2016_4,
    SERVICE_MEDAL_2016_5,
    SERVICE_MEDAL_2016_6,

    SERVICE_MEDAL_2017_1 = 1357,
    SERVICE_MEDAL_2017_2,
    SERVICE_MEDAL_2017_3,
    SERVICE_MEDAL_2017_4,
    SERVICE_MEDAL_2017_5,
    SERVICE_MEDAL_2017_6,
    SERVICE_MEDAL_2017_7,

    SERVICE_MEDAL_2018_1 = 1367,
    SERVICE_MEDAL_2018_2,
    SERVICE_MEDAL_2018_3,
    SERVICE_MEDAL_2018_4,
    SERVICE_MEDAL_2018_5,
    SERVICE_MEDAL_2018_6,

    DUST2_PIN = 6001,
    GARDIAN_ELITE_PIN,
    MIRAGE_PIN,
    INFERNO_PIN,
    ITALY_PIN,
    VICTORY_PIN,
    MILITIA_PIN,
    NUKE_PIN,
    TRAIN_PIN,
    TACTICS_PIN,
    GUARDIAN2_PIN,
    BRAVO_PIN,
    PHOENIX_PIN,
    OFFICE_PIN,
    COBBLESTONE_PIN,
    OVERPASS_PIN,
    BLOODHOUND_PIN,
    CACHE_PIN,
    VALERIA_PHOENIX_PIN,
    CHROMA_PIN,
    GUARDIAN3_PIN,
    CANALS_PIN,
    WELCOME_TO_TEH_CLUTCH_PIN,
    DEATH_SENTENCE_PIN,
    INFERNO2_PIN,
    WILDFIRE_PIN,
    EASY_PEASY_PIN,
    ACES_HIGH_PIN,
    HYDRA_PIN,
    HOWL_PIN,
    BRIGADIER_GENERAL_PIN

};

class WeaponInfo_t
{
public:
    void*   pVTable;
    char*   consoleName;
    char    pad_0008[12];
    int     iMaxClip1;
    int     iMaxClip2;
    int     iDefaultClip1;
    int     iDefaultClip2;
    char    pad_0024[8];
    char*   szWorldModel;
    char*   szViewModel;
    char*   szDroppedModel;
    char    pad_0038[4];
    char*   N0000023E;
    char    pad_0040[56];
    char*   szEmptySound;
    char    pad_007C[4];
    char*   szBulletType;
    char    pad_0084[4];
    char*   szHudName;
    char*   szWeaponName;
    char    pad_0090[56];
    CSWeaponType    iWeaponType;
    int     iWeaponPrice;
    int     iKillAward;
    char*   szAnimationPrefix;
    float   flCycleTime;
    float   flCycleTimeAlt;
    float   flTimeToIdle;
    float   flIdleInterval;
    bool    bFullAuto;    
    char    pad_0x00E5[3];
    int     iDamage;
    float   flArmorRatio;
    int     iBullets;
    float   flPenetration;
    float   flFlinchVelocityModifierLarge;
    float   flFlinchVelocityModifierSmall;
    float   flRange;
    float   flRangeModifier;
    float   flThrowVelocity;
    char    pad_0x010C[12];
    bool    bHasSilencer;
    char    pad_0x0119[3];
    char*   pSilencerModel;
    int     iCrosshairMinDistance;
    int     iCrosshairDeltaDistance;
    float   flMaxPlayerSpeed;
    float   flMaxPlayerSpeedAlt;
    float   flSpread;
    float   flSpreadAlt;
    float   flInaccuracyCrouch;
    float   flInaccuracyCrouchAlt;
    float   flInaccuracyStand;
    float   flInaccuracyStandAlt;
    float   flInaccuracyJumpInitial;
    float   flInaccuracyJump;
    float   flInaccuracyJumpAlt;
    float   flInaccuracyLand;
    float   flInaccuracyLandAlt;
    float   flInaccuracyLadder;
    float   flInaccuracyLadderAlt;
    float   flInaccuracyFire;
    float   flInaccuracyFireAlt;
    float   flInaccuracyMove;
    float   flInaccuracyMoveAlt;
    float   flInaccuracyReload;
    int     iRecoilSeed;
    float   flRecoilAngle;
    float   flRecoilAngleAlt;
    float   flRecoilAngleVariance;
    float   flRecoilAngleVarianceAlt;
    float   flRecoilMagnitude;
    float   flRecoilMagnitudeAlt;
    float   flRecoilMagnitudeVariance;
    float   flRecoilMagnitudeVarianceAlt;
    float   flRecoveryTimeCrouch;
    float   flRecoveryTimeStand;
    float   flRecoveryTimeCrouchFinal;
    float   flRecoveryTimeStandFinal;
    int     iRecoveryTransitionStartBullet;
    int     iRecoveryTransitionEndBullet;
    bool    bUnzoomAfterShot;
    bool    bHideViewModelZoomed;
    char    pad_0x01B5[2];
    char    iZoomLevels[3];
    int     iZoomFOV[2];
    float   fZoomTime[3];
    char*   szWeaponClass;
    float   flAddonScale;
    char    pad_0x01DC[4];
    char*   szEjectBrassEffect;
    char*   szTracerEffect;
    int     iTracerFrequency;
    int     iTracerFrequencyAlt;
    char*   szMuzzleFlashEffect_1stPerson;
    char    pad_0x01F4[4];
    char*   szMuzzleFlashEffect_3rdPerson;
    char    pad_0x01FC[4];
    char*   szMuzzleSmokeEffect;
    float   flHeatPerShot;
    char*   szZoomInSound;
    char*   szZoomOutSound;
    float   flInaccuracyPitchShift;
    float   flInaccuracySoundThreshold;
    float   flBotAudibleRange;
    char    pad_0x0218[8];
    char*   pWrongTeamMsg;
    bool    bHasBurstMode;
    char    pad_0x0225[3];     
    bool    bIsRevolver;
    bool    bCannotShootUnderwater;
};

enum mp_break_t
{
    MULTIPLAYER_BREAK_DEFAULT,
    MULTIPLAYER_BREAK_SERVERSIDE,
    MULTIPLAYER_BREAK_CLIENTSIDE,
    MULTIPLAYER_BREAK_BOTH
};

enum propdata_interactions_t
{
    PROPINTER_PHYSGUN_WORLD_STICK,		// "onworldimpact"	"stick"
    PROPINTER_PHYSGUN_FIRST_BREAK,		// "onfirstimpact"	"break"
    PROPINTER_PHYSGUN_FIRST_PAINT,		// "onfirstimpact"	"paintsplat"
    PROPINTER_PHYSGUN_FIRST_IMPALE,		// "onfirstimpact"	"impale"
    PROPINTER_PHYSGUN_LAUNCH_SPIN_NONE,	// "onlaunch"		"spin_none"
    PROPINTER_PHYSGUN_LAUNCH_SPIN_Z,	// "onlaunch"		"spin_zaxis"
    PROPINTER_PHYSGUN_BREAK_EXPLODE,	// "onbreak"		"explode_fire"
    PROPINTER_PHYSGUN_DAMAGE_NONE,		// "damage"			"none"
    PROPINTER_FIRE_FLAMMABLE,			// "flammable"			"yes"
    PROPINTER_FIRE_EXPLOSIVE_RESIST,	// "explosive_resist"	"yes"
    PROPINTER_FIRE_IGNITE_HALFHEALTH,	// "ignite"				"halfhealth"
    PROPINTER_PHYSGUN_CREATE_FLARE,		// "onpickup"		"create_flare"
    PROPINTER_PHYSGUN_ALLOW_OVERHEAD,	// "allow_overhead"	"yes"
    PROPINTER_WORLD_BLOODSPLAT,			// "onworldimpact", "bloodsplat"
    PROPINTER_PHYSGUN_NOTIFY_CHILDREN,	// "onfirstimpact" cause attached flechettes to explode
    PROPINTER_NUM_INTERACTIONS,			// If we get more than 32 of these, we'll need a different system
};