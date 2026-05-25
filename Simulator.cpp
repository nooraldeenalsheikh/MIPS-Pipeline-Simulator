#include "Simulator.h"

int findLastProducer(vector<Instr>& prog, int idx, int reg) {
    for (int j = idx - 1; j >= 0; --j) {
        if (prog[j].rd == reg) return j;
    }
    return -1;
}

void simulatePipeline(vector<Instr>& prog, bool enableForwarding,
                      vector<StallEv>& stalls, vector<FwdEv>& forwardings,
                                       int& lastWR ) 
{
    stalls.clear();
    forwardings.clear();

    int fetchTime = 1;

    for (int i = 0; i < prog.size(); ++i) {
        prog[i].IF = fetchTime;

        int baseRR = prog[i].IF + 1;
        int baseEX = prog[i].IF + 2;

        vector<int> srcs;
        if (prog[i].rs != -1) srcs.push_back(prog[i].rs);
        if (prog[i].rt != -1) srcs.push_back(prog[i].rt);

        int stallsNeeded = 0;

        for (int reg : srcs) {
            int p = findLastProducer(prog, i, reg);
            if (p == -1) continue;

            int prodProduceCycle;
            if (prog[p].isLoad())      prodProduceCycle = prog[p].MA; // MA
            else if (prog[p].isALU())  prodProduceCycle = prog[p].EX; // EX
            else                       prodProduceCycle = prog[p].WR; // WR 

            int producerWR = prog[p].WR;

            if (!enableForwarding) {
                int neededRR = producerWR + 1;
                stallsNeeded = max(stallsNeeded,  neededRR - baseRR);
            }
            else {
                int availableForEX = prodProduceCycle + 1;
                stallsNeeded = max(stallsNeeded,  availableForEX - baseEX);
            }
        }

        prog[i].RR = baseRR + stallsNeeded;
        prog[i].EX = baseEX + stallsNeeded;
        prog[i].MA = prog[i].IF + 3 + stallsNeeded;
        prog[i].WR = prog[i].IF + 4 + stallsNeeded;

        if (stallsNeeded > 0) {
            int stStart = prog[i].IF + 1;
            int stEnd = stStart + stallsNeeded - 1;
            stalls.push_back({ i, stallsNeeded, stStart, stEnd });
        }

        if (enableForwarding) {
            for (int reg : srcs) {
                int p = findLastProducer(prog, i, reg);
                if (p == -1) continue;

                int prodProduceCycle;
                string prodStage;
                if (prog[p].isLoad()) { prodProduceCycle = prog[p].MA; prodStage = "MA"; }
                else if (prog[p].isALU()) { prodProduceCycle = prog[p].EX; prodStage = "EX"; }
                else { prodProduceCycle = prog[p].WR; prodStage = "WR"; }

                bool canUseInEX = (prog[i].EX >= prodProduceCycle + 1);
                bool needsForwarding = (prog[p].WR > prog[i].RR);

                if (canUseInEX && needsForwarding) {
                    bool already = false;
                    for (int k = 0; k < (int)forwardings.size(); k++) {
                        if (forwardings[k].producer_ins == p &&
                            forwardings[k].consumer_ins == i &&
                            forwardings[k].consEX == prog[i].EX &&
                            forwardings[k].prodStage == prodStage) {
                            already = true;
                            break;
                        }
                    }
                    if (!already) {
                        forwardings.push_back({ p, prodStage, i, prog[i].EX });
                    }
                }
            }
        }

        fetchTime = prog[i].IF + 1 + stallsNeeded;
    }

    lastWR = 0;
    for (Instr ins : prog) lastWR = max(lastWR, ins.WR);
}
