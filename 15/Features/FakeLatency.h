#pragma once
#include "../SDK/SDK.h"
#include "../Settings.h"
#include "../Utils/GlobalVars.h"

namespace FakeLatency
{
    void AddLatencyToNetchan(INetChannel*, float);
    void ClearIncomingSequences();
    void UpdateIncomingSequences();
    void FireGameEvent(IGameEvent*);
}