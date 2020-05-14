#pragma once
#include "../SDK/SDK.h"
#include "../Settings.h"
#include "../Utils/GlobalVars.h"
#include "../Hooks.h"

#include <fstream>

class Chams
{
public:
    Chams();
    ~Chams();

    void OverrideMaterial(bool ignoreZ, int nType, const float* rgba);

    void DrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
protected:
    IMaterial* materialRegular;
    IMaterial* materialRegularIgnoreZ;
    IMaterial* materialFlatIgnoreZ;
    IMaterial* materialFlat;

    // Customs
    IMaterial* materialGloss;
    IMaterial* materialGlossIgnoreZ;
};