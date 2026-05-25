#include "Speedup.h"
#include "Simulator.h"

double computeSpeedup(const vector<Instr>& originalProg) {

    // ‰”Œ «·»—‰«„Ã (·√‰ «·„Õ«ﬂ«… » ⁄œ¯· IF/RR/EX...)
    vector<Instr> progNoFwd = originalProg;
    vector<Instr> progFwd = originalProg;

    vector<StallEv> stalls1, stalls2;
    vector<FwdEv> fwd1, fwd2;

    int cyclesNoFwd = 0;
    int cyclesFwd = 0;

    simulatePipeline(progNoFwd, false, stalls1, fwd1, cyclesNoFwd);
    simulatePipeline(progFwd, true, stalls2, fwd2, cyclesFwd);

    return (double)cyclesNoFwd / (double)cyclesFwd;
}
