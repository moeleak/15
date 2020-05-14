#include <deque>
#include "FakeLatency.h"
#include "../Hooks.h"

namespace
{
    int                      lastincomingsequencenumber;
    std::deque<SequenceData> sequences;
}

void FakeLatency::AddLatencyToNetchan(INetChannel* NetChan, float latency)
{
    if (!g_Settings.Miscellaneous.bFakeLatency)
        return;

    for (auto& seq : sequences)
    {
        if (g_pGlobalVars->realtime - seq.Realtime >= latency)
        {
            NetChan->m_nInReliableState = seq.inreliablestate;
            NetChan->m_nInSequenceNr = seq.sequencenr;
            break;
        }
    }
}


void FakeLatency::ClearIncomingSequences()
{
    g_pGlobalVars->Real_m_nInSequencenumber = 0;
    sequences.clear();
}

void FakeLatency::UpdateIncomingSequences()
{
    if (!g_Settings.Miscellaneous.bFakeLatency)
        return;

    if (g_pClientState)
    {
        INetChannel* NetChannel = g_pClientState->m_NetChannel;
        if (NetChannel)
        {
            if (g_pGlobalVars->Real_m_nInSequencenumber == 0)
            {
                g_pGlobalVars->Real_m_nInSequencenumber = NetChannel->m_nInSequenceNr;
                lastincomingsequencenumber = NetChannel->m_nInSequenceNr;
            }

            if (NetChannel->m_nInSequenceNr > lastincomingsequencenumber) 
            {
                lastincomingsequencenumber = NetChannel->m_nInSequenceNr;
                sequences.push_front(SequenceData(NetChannel->m_nInReliableState, NetChannel->m_nOutReliableState, NetChannel->m_nInSequenceNr, g_pGlobalVars->realtime));
            }

            if (sequences.size() > 2048)
                sequences.pop_back();
        }
    }
}

void FakeLatency::FireGameEvent(IGameEvent* pEvent)
{
    if (!g_Settings.Miscellaneous.bFakeLatency)
    {
        auto userid = g_pEngine->GetPlayerForUserID(pEvent->GetInt("userid"));
        if (!userid)
            return;

        if (userid == g_pEngine->GetLocalPlayer())
        {
            /*g_Hooks.pNetChannelHook->Unhook(vtable_indexes::senddatagram);
            g_Hooks.pNetChannelHook = nullptr;
            g_Hooks.oSendDatagram = NULL;
            Utils::Log("oSendDatagram = NULL");*/
        }
    }
}