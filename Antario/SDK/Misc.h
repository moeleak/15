#pragma once

#include "Vector.h"
#include "VMatrix.h"
#include "IEngineTrace.h"

class IMaterial;
class CPhysCollide;
class CUtlBuffer;
class IClientRenderable;
class CStudioHdr;
struct virtualmodel_t;

typedef float Quaternion[4];
typedef float RadianEuler[3];

#define MAX_QPATH  260

#define BONE_CALCULATE_MASK             0x1F
#define BONE_PHYSICALLY_SIMULATED       0x01    // bone is physically simulated when physics are active
#define BONE_PHYSICS_PROCEDURAL         0x02    // procedural when physics is active
#define BONE_ALWAYS_PROCEDURAL          0x04    // bone is always procedurally animated
#define BONE_SCREEN_ALIGN_SPHERE        0x08    // bone aligns to the screen, not constrained in motion.
#define BONE_SCREEN_ALIGN_CYLINDER      0x10    // bone aligns to the screen, constrained by it's own axis.

#define BONE_USED_MASK                  0x0007FF00
#define BONE_USED_BY_ANYTHING           0x0007FF00
#define BONE_USED_BY_HITBOX             0x00000100    // bone (or child) is used by a hit box
#define BONE_USED_BY_ATTACHMENT         0x00000200    // bone (or child) is used by an attachment point
#define BONE_USED_BY_VERTEX_MASK        0x0003FC00
#define BONE_USED_BY_VERTEX_LOD0        0x00000400    // bone (or child) is used by the toplevel model via skinned vertex
#define BONE_USED_BY_VERTEX_LOD1        0x00000800    
#define BONE_USED_BY_VERTEX_LOD2        0x00001000  
#define BONE_USED_BY_VERTEX_LOD3        0x00002000
#define BONE_USED_BY_VERTEX_LOD4        0x00004000
#define BONE_USED_BY_VERTEX_LOD5        0x00008000
#define BONE_USED_BY_VERTEX_LOD6        0x00010000
#define BONE_USED_BY_VERTEX_LOD7        0x00020000
#define BONE_USED_BY_BONE_MERGE         0x00040000    // bone is available for bone merge to occur against it

#define BONE_USED_BY_VERTEX_AT_LOD(lod) ( BONE_USED_BY_VERTEX_LOD0 << (lod) )
#define BONE_USED_BY_ANYTHING_AT_LOD(lod) ( ( BONE_USED_BY_ANYTHING & ~BONE_USED_BY_VERTEX_MASK ) | BONE_USED_BY_VERTEX_AT_LOD(lod) )

#define MAX_NUM_LODS 8
#define MAXSTUDIOBONES        128        // total bones actually used

#define BONE_TYPE_MASK                  0x00F00000
#define BONE_FIXED_ALIGNMENT            0x00100000    // bone can't spin 360 degrees, all interpolation is normalized around a fixed orientation

#define BONE_HAS_SAVEFRAME_POS          0x00200000    // Vector48
#define BONE_HAS_SAVEFRAME_ROT64        0x00400000    // Quaternion64
#define BONE_HAS_SAVEFRAME_ROT32        0x00800000    // Quaternion32

enum modtype_t
{
    mod_bad = 0,
    mod_brush,
    mod_sprite,
    mod_studio
};

enum RenderableTranslucencyType_t
{
    RENDERABLE_IS_OPAQUE = 0,
    RENDERABLE_IS_TRANSLUCENT,
    RENDERABLE_IS_TWO_PASS,    // has both translucent and opaque sub-partsa
};

typedef unsigned short MDLHandle_t;

struct mstudiobone_t
{
    int                    sznameindex;
    inline char * const    pszName(void) const { return ((char *)this) + sznameindex; }
    int                    parent;
    int                    bonecontroller[6];    // bone controller index, -1 == none
    Vector                 pos;
    Quaternion             quat;
    RadianEuler            rot;
    // compression scale
    Vector                 posscale;
    Vector                 rotscale;

    matrix3x4_t            poseToBone;
    Quaternion             qAlignment;
    int                    flags;
    int                    proctype;
    int                    procindex;
    mutable int            physicsbone;
    inline void *          pProcedure() const { if (procindex == 0) return NULL; else return  (void *)(((byte *)this) + procindex); };
    int                    surfacepropidx;
    inline char * const    pszSurfaceProp(void) const { return ((char *)this) + surfacepropidx; }
    inline int             GetSurfaceProp(void) const { return surfacepropLookup; }

    int                    contents;
    int                    surfacepropLookup;
    int                    m_iPad01[7];

    mstudiobone_t() {}
private:
    // No copy constructors allowed
    mstudiobone_t(const mstudiobone_t& vOther);
};;


struct mstudiobbox_t
{
    int         bone;
    int         group;
    Vector      bbmin;
    Vector      bbmax;
    int         szhitboxnameindex;
    int32_t     m_iPad01[3];
    float       m_flRadius;
    int32_t     m_iPad02[4];

    const char* GetName()
    {
        if (!szhitboxnameindex) return nullptr;
        return (const char*)((uint8_t*)this + szhitboxnameindex);
    }
};

struct mstudiohitboxset_t
{
    int    sznameindex;
    int    numhitboxes;
    int    hitboxindex;

    const char* GetName()
    {
        if (!sznameindex) return nullptr;
        return (const char*)((uint8_t*)this + sznameindex);
    }

    mstudiobbox_t* GetHitbox(int i)
    {
        if (i > numhitboxes) return nullptr;
        return (mstudiobbox_t*)((uint8_t*)this + hitboxindex) + i;
    }
};

struct model_t
{
    void*   fnHandle;               //0x0000 
    char    szName[260];            //0x0004 
    __int32 nLoadFlags;             //0x0108 
    __int32 nServerCount;           //0x010C 
    __int32 type;                   //0x0110 
    __int32 flags;                  //0x0114 
    Vector  vecMins;                //0x0118 
    Vector  vecMaxs;                //0x0124 
    float   radius;                 //0x0130 
    char    pad[0x1C];              //0x0134
};//Size=0x0150

enum MaterialPropertyTypes_t
{
    MATERIAL_PROPERTY_NEEDS_LIGHTMAP = 0,					// bool
    MATERIAL_PROPERTY_OPACITY,								// int (enum MaterialPropertyOpacityTypes_t)
    MATERIAL_PROPERTY_REFLECTIVITY,							// vec3_t
    MATERIAL_PROPERTY_NEEDS_BUMPED_LIGHTMAPS				// bool
};

class studiohdr_t
{
public:
    __int32 id;                     //0x0000 
    __int32 version;                //0x0004 
    long    checksum;               //0x0008 
    char    szName[64];             //0x000C 
    __int32 length;                 //0x004C 
    Vector  vecEyePos;              //0x0050 
    Vector  vecIllumPos;            //0x005C 
    Vector  vecHullMin;             //0x0068 
    Vector  vecHullMax;             //0x0074 
    Vector  vecBBMin;               //0x0080 
    Vector  vecBBMax;               //0x008C 
    __int32 flags;                  //0x0098 
    __int32 numbones;               //0x009C 
    __int32 boneindex;              //0x00A0 
    __int32 numbonecontrollers;     //0x00A4 
    __int32 bonecontrollerindex;    //0x00A8 
    __int32 numhitboxsets;          //0x00AC 
    __int32 hitboxsetindex;         //0x00B0 
    __int32 numlocalanim;           //0x00B4 
    __int32 localanimindex;         //0x00B8 
    __int32 numlocalseq;            //0x00BC 
    __int32 localseqindex;          //0x00C0 
    __int32 activitylistversion;    //0x00C4 
    __int32 eventsindexed;          //0x00C8 
    __int32 numtextures;            //0x00CC 
    __int32 textureindex;           //0x00D0

    mstudiohitboxset_t* GetHitboxSet(int i)
    {
        if (i > numhitboxsets) return nullptr;
        return (mstudiohitboxset_t*)((uint8_t*)this + hitboxsetindex) + i;
    }
    mstudiobone_t* GetBone(int i)
    {
        if (i > numbones) return nullptr;
        return (mstudiobone_t*)((uint8_t*)this + boneindex) + i;
    }

};//Size=0x00D4
class IVModelInfo
{
public:
    virtual							~IVModelInfo(void) { }
    virtual const model_t			*GetModel(int modelindex) const = 0;
    // Returns index of model by name
    virtual int						GetModelIndex(const char *name) const = 0;
    // Returns name of model
    virtual const char				*GetModelName(const model_t *model) const = 0;
    virtual void					UNUSED000() {};
    virtual vcollide_t				*GetVCollide(const model_t *model) const = 0;
    virtual vcollide_t				*GetVCollide(int modelindex) const = 0;
    virtual void					GetModelBounds(const model_t *model, Vector& mins, Vector& maxs) const = 0;
    virtual	void					GetModelRenderBounds(const model_t *model, Vector& mins, Vector& maxs) const = 0;
    virtual int						GetModelFrameCount(const model_t *model) const = 0;
    virtual int						GetModelType(const model_t *model) const = 0;
    virtual void					*GetModelExtraData(const model_t *model) = 0;
    virtual bool					ModelHasMaterialProxy(const model_t *model) const = 0;
    virtual bool					IsTranslucent(model_t const* model) const = 0;
    virtual bool					IsTranslucentTwoPass(const model_t *model) const = 0;
    virtual void					Unused0() {};
    virtual RenderableTranslucencyType_t ComputeTranslucencyType(const model_t *model, int nSkin, int nBody) = 0;
    virtual int						GetModelMaterialCount(const model_t* model) const = 0;
    virtual void					GetModelMaterials(const model_t *model, int count, IMaterial** ppMaterials) = 0;
    virtual bool					IsModelVertexLit(const model_t *model) const = 0;
    virtual const char				*GetModelKeyValueText(const model_t *model) = 0;
    virtual bool					GetModelKeyValue(const model_t *model, CUtlBuffer &buf) = 0; // supports keyvalue blocks in submodels
    virtual float					GetModelRadius(const model_t *model) = 0;

    virtual const studiohdr_t		*FindModel(const studiohdr_t *pStudioHdr, void **cache, const char *modelname) const = 0;
    virtual const studiohdr_t		*FindModel(void *cache) const = 0;
    virtual	virtualmodel_t			*GetVirtualModel(const studiohdr_t *pStudioHdr) const = 0;
    virtual byte					*GetAnimBlock(const studiohdr_t *pStudioHdr, int iBlock) const = 0;
    virtual bool					HasAnimBlockBeenPreloaded(studiohdr_t const*, int) const = 0;

    // Available on Client only!!!
    virtual void					GetModelMaterialColorAndLighting(const model_t *model, Vector const& origin,
        QAngle const& angles, trace_t* pTrace,
        Vector& lighting, Vector& matColor) = 0;
    virtual void					GetIlluminationPoint(const model_t *model, IClientRenderable *pRenderable, Vector const& origin,
        QAngle const& angles, Vector* pLightingCenter) = 0;

    virtual int						GetModelContents(int modelIndex) const = 0;
    virtual void                    UNUSED() = 0;
    virtual studiohdr_t				*GetStudioModel(const model_t *mod) = 0;
    virtual int						GetModelSpriteWidth(const model_t *model) const = 0;
    virtual int						GetModelSpriteHeight(const model_t *model) const = 0;

    // Sets/gets a map-specified fade range (Client only)
    virtual void					SetLevelScreenFadeRange(float flMinSize, float flMaxSize) = 0;
    virtual void					GetLevelScreenFadeRange(float *pMinArea, float *pMaxArea) const = 0;

    // Sets/gets a map-specified per-view fade range (Client only)
    virtual void					SetViewScreenFadeRange(float flMinSize, float flMaxSize) = 0;

    // Computes fade alpha based on distance fade + screen fade (Client only)
    virtual unsigned char			ComputeLevelScreenFade(const Vector &vecAbsOrigin, float flRadius, float flFadeScale) const = 0;
    virtual unsigned char			ComputeViewScreenFade(const Vector &vecAbsOrigin, float flRadius, float flFadeScale) const = 0;

    // both Client and server
    virtual int						GetAutoplayList(const studiohdr_t *pStudioHdr, unsigned short **pAutoplayList) const = 0;

    // Gets a virtual terrain collision model (creates if necessary)
    // NOTE: This may return NULL if the terrain model cannot be virtualized
    virtual CPhysCollide			*GetCollideForVirtualTerrain(int index) = 0;
    virtual bool					IsUsingFBTexture(const model_t *model, int nSkin, int nBody, void /*IClientRenderable*/ *pClientRenderable) const = 0;
    virtual const model_t			*FindOrLoadModel(const char *name) const = 0;
    virtual MDLHandle_t				GetCacheHandle(const model_t *model) const = 0;
    // Returns planes of non-nodraw brush model surfaces
    virtual int						GetBrushModelPlaneCount(const model_t *model) const = 0;
    virtual void					GetBrushModelPlane(const model_t *model, int nIndex, cplane_t &plane, Vector *pOrigin) const = 0;
    virtual int						GetSurfacepropsForVirtualTerrain(int index) = 0;
    virtual bool					UsesEnvCubemap(const model_t *model) const = 0;
    virtual bool					UsesStaticLighting(const model_t *model) const = 0;
};
extern IVModelInfo* g_pModelInfo;

class IVModelInfoClient : public IVModelInfo
{
public:
};


struct virtualterrainparams_t
{
    // UNDONE: Add grouping here, specified in BSP file? (test grouping to see if this is necessary)
    int index;
};

struct inputdata_t;
typedef enum _fieldtypes
{
    FIELD_VOID = 0,			// No type or value
    FIELD_FLOAT,			// Any floating point value
    FIELD_STRING,			// A string ID (return from ALLOC_STRING)
    FIELD_VECTOR,			// Any vector, QAngle, or AngularImpulse
    FIELD_QUATERNION,		// A quaternion
    FIELD_INTEGER,			// Any integer or enum
    FIELD_BOOLEAN,			// boolean, implemented as an int, I may use this as a hint for compression
    FIELD_SHORT,			// 2 byte integer
    FIELD_CHARACTER,		// a byte
    FIELD_COLOR32,			// 8-bit per channel r,g,b,a (32bit color)
    FIELD_EMBEDDED,			// an embedded object with a datadesc, recursively traverse and embedded class/structure based on an additional typedescription
    FIELD_CUSTOM,			// special type that contains function pointers to it's read/write/parse functions

    FIELD_CLASSPTR,			// CBaseEntity *
    FIELD_EHANDLE,			// Entity handle
    FIELD_EDICT,			// edict_t *

    FIELD_POSITION_VECTOR,	// A world coordinate (these are fixed up across level transitions automagically)
    FIELD_TIME,				// a floating point time (these are fixed up automatically too!)
    FIELD_TICK,				// an integer tick count( fixed up similarly to time)
    FIELD_MODELNAME,		// Engine string that is a model name (needs precache)
    FIELD_SOUNDNAME,		// Engine string that is a sound name (needs precache)

    FIELD_INPUT,			// a list of inputed data fields (all derived from CMultiInputVar)
    FIELD_FUNCTION,			// A class function pointer (Think, Use, etc)

    FIELD_VMATRIX,			// a vmatrix (output coords are NOT worldspace)

                            // NOTE: Use float arrays for local transformations that don't need to be fixed up.
                            FIELD_VMATRIX_WORLDSPACE,// A VMatrix that maps some local space to world space (translation is fixed up on level transitions)
                            FIELD_MATRIX3X4_WORLDSPACE,	// matrix3x4_t that maps some local space to world space (translation is fixed up on level transitions)

                            FIELD_INTERVAL,			// a start and range floating point interval ( e.g., 3.2->3.6 == 3.2 and 0.4 )
                            FIELD_MODELINDEX,		// a model index
                            FIELD_MATERIALINDEX,	// a material index (using the material precache string table)

                            FIELD_VECTOR2D,			// 2 floats

                            FIELD_TYPECOUNT,		// MUST BE LAST
} fieldtype_t;

class ISaveRestoreOps;
class C_BaseEntity;
//
// Function prototype for all input handlers.
//
typedef void (C_BaseEntity::*inputfunc_t)(inputdata_t &data);

struct datamap_t;
class typedescription_t;

enum
{
    TD_OFFSET_NORMAL = 0,
    TD_OFFSET_PACKED = 1,

    // Must be last
    TD_OFFSET_COUNT,
};

class typedescription_t
{
public:
    int32_t fieldType; //0x0000
    char* fieldName; //0x0004
    int fieldOffset[TD_OFFSET_COUNT]; //0x0008
    int16_t fieldSize_UNKNWN; //0x0010
    int16_t flags_UNKWN; //0x0012
    char pad_0014[12]; //0x0014
    datamap_t* td; //0x0020
    char pad_0024[24]; //0x0024
}; //Size: 0x003C


   //-----------------------------------------------------------------------------
   // Purpose: stores the list of objects in the hierarchy
   //            used to iterate through an object's data descriptions
   //-----------------------------------------------------------------------------
struct datamap_t
{
    typedescription_t    *dataDesc;
    int                    dataNumFields;
    char const            *dataClassName;
    datamap_t            *baseMap;

    bool                chains_validated;
    // Have the "packed" offsets been computed
    bool                packed_offsets_computed;
    int                    packed_size;
};

class bf_write
{
public:
    bf_write();

    // nMaxBits can be used as the number of bits in the buffer. 
    // It must be <= nBytes*8. If you leave it at -1, then it's set to nBytes * 8.
    bf_write(void* pData, int nBytes, int nMaxBits = -1);

    bf_write(const char* pDebugName, void* pData, int nBytes, int nMaxBits = -1);

    // Start writing to the specified buffer.
    // nMaxBits can be used as the number of bits in the buffer. 
    // It must be <= nBytes*8. If you leave it at -1, then it's set to nBytes * 8.
    void StartWriting(void* pData, int nBytes, int iStartBit = 0, int nMaxBits = -1);

    // Restart buffer writing.
    void Reset();

    // Get the base pointer.
    unsigned char* GetBasePointer()
    {
        return (unsigned char*)m_pData;
    }

    // Enable or disable assertion on overflow. 99% of the time, it's a bug that we need to catch,
    // but there may be the occasional buffer that is allowed to overflow gracefully.
    void SetAssertOnOverflow(bool bAssert);

    // This can be set to assign a name that gets output if the buffer overflows.
    const char* GetDebugName();

    void SetDebugName(const char* pDebugName);

    // Seek to a specific position.
public:

    void SeekToBit(int bitPos);

    // Bit functions.
public:

    void WriteOneBit(int nValue);

    void WriteOneBitNoCheck(int nValue);

    void WriteOneBitAt(int iBit, int nValue);

    // Write signed or unsigned. Range is only checked in debug.
    void WriteUBitLong(unsigned int data, int numbits, bool bCheckRange = true);

    void WriteSBitLong(int data, int numbits);

    // Tell it whether or not the data is unsigned. If it's signed,
    // cast to unsigned before passing in (it will cast back inside).
    void WriteBitLong(unsigned int data, int numbits, bool bSigned);

    // Write a list of bits in.
    bool WriteBits(const void* pIn, int nBits);

    // writes an unsigned integer with variable bit length
    void WriteUBitVar(unsigned int data);

    // writes a varint encoded integer
    void WriteVarInt32(uint32_t data);

    void WriteVarInt64(uint64_t data);

    void WriteSignedVarInt32(int32_t data);

    void WriteSignedVarInt64(int64_t data);

    int ByteSizeVarInt32(uint32_t data);

    int ByteSizeVarInt64(uint64_t data);

    int ByteSizeSignedVarInt32(int32_t data);

    int ByteSizeSignedVarInt64(int64_t data);

    // Copy the bits straight out of pIn. This seeks pIn forward by nBits.
    // Returns an error if this buffer or the read buffer overflows.
    bool WriteBitsFromBuffer(class bf_read* pIn, int nBits);

    void WriteBitAngle(float fAngle, int numbits);

    void WriteBitCoord(const float f);

    void WriteBitCoordMP(const float f, bool bIntegral, bool bLowPrecision);

    void WriteBitFloat(float val);

    void WriteBitVec3Coord(const Vector& fa);

    void WriteBitNormal(float f);

    void WriteBitVec3Normal(const Vector& fa);

    void WriteBitAngles(const Vector& fa);

    // Byte functions.
public:

    void WriteChar(int val);

    void WriteByte(int val);

    void WriteShort(int val);

    void WriteWord(int val);

    void WriteLong(long val);

    void WriteLongLong(int64_t val);

    void WriteFloat(float val);

    bool WriteBytes(const void* pBuf, int nBytes);

    // Returns false if it overflows the buffer.
    bool WriteString(const char* pStr);

    // Status.
public:

    // How many bytes are filled in?
    int GetNumBytesWritten() const;

    int GetNumBitsWritten() const;

    int GetMaxNumBits();

    int GetNumBitsLeft();

    int GetNumBytesLeft();

    unsigned char* GetData();

    const unsigned char* GetData() const;

    // Has the buffer overflowed?
    bool CheckForOverflow(int nBits);

    inline bool IsOverflowed() const
    {
        return m_bOverflow;
    }

    void SetOverflowFlag();

public:
    // The current buffer.
    unsigned long* m_pData;
    int m_nDataBytes;
    int m_nDataBits;

    // Where we are in the buffer.
    int m_iCurBit;

private:

    // Errors?
    bool m_bOverflow;

    bool m_bAssertOnOverflow;
    const char* m_pDebugName;
};

class bf_read
{
public:
    uintptr_t base_address;
    uintptr_t cur_offset;

    bf_read::bf_read(uintptr_t addr)
    {
        base_address = addr;
        cur_offset = 0;
    }

    void bf_read::SetOffset(uintptr_t offset)
    {
        cur_offset = offset;
    }

    void bf_read::Skip(uintptr_t length)
    {
        cur_offset += length;
    }

    int bf_read::ReadByte()
    {
        auto val = *reinterpret_cast<char*>(base_address + cur_offset);
        ++cur_offset;
        return val;
    }

    bool bf_read::ReadBool()
    {
        auto val = *reinterpret_cast<bool*>(base_address + cur_offset);
        ++cur_offset;
        return val;
    }

    std::string bf_read::ReadString()
    {
        char buffer[256];
        auto str_length = *reinterpret_cast<char*>(base_address + cur_offset);
        ++cur_offset;
        memcpy(buffer, reinterpret_cast<void*>(base_address + cur_offset), str_length > 255 ? 255 : str_length);
        buffer[str_length > 255 ? 255 : str_length] = '\0';
        cur_offset += str_length + 1;
        return std::string(buffer);
    }
};
