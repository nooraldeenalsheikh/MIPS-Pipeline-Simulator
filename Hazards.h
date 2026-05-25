#pragma once
#include "Common.h"

struct StallEv {
    int instr_number;
    int stalls_count;
    int startCycle;
    int endCycle;
};

struct FwdEv {
    int producer_ins;
    string prodStage;
    int consumer_ins;
    int consEX;
};
