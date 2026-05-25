#pragma once
#include "Instr.h"
#include "Hazards.h"

void simulatePipeline(
    vector<Instr>& prog,
    bool enableForwarding,
    vector<StallEv>& stalls,
    vector<FwdEv>& forwardings,
    int& lastWR
);
