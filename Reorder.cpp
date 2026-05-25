#include "Reorder.h"

 static bool writesReg( Instr& ins, int r) {
    return (r != -1 && ins.rd == r);
}

 static bool readsReg( Instr& ins, int r) {
    return (r!= -1 && ((ins.rs == r) || (ins.rt == r)));
}

static bool canMoveUp( vector<Instr>& v, int blockStart, int checking_inst) {
      Instr& C = v[checking_inst];

    for (int k = blockStart; k < checking_inst; ++k) {
        Instr& A = v[k];

        // RAW
        if (C.rs != -1 && writesReg(A, C.rs)) return false;
        if (C.rt != -1 && writesReg(A, C.rt)) return false;

        // WAR
        if (C.rd != -1 && readsReg(A, C.rd)) return false;

        // WAW
        if (C.rd != -1 && writesReg(A, C.rd)) return false;
    }
    return true;
}

vector<Instr> reorderProgram( vector<Instr>& prog, int windowSize) {
    vector<Instr> v = prog;
    vector<Instr> out;
    out.reserve(v.size());
    
    int i = 0;
    while (i < v.size()) {
        int end = min((int)v.size(), i + windowSize);
        
        int chosen = i;
        bool found = false;

        for (int checking_inst = i; checking_inst < end; ++checking_inst) {
            if (!canMoveUp(v, i, checking_inst)) continue;

            if (!found) { chosen = checking_inst; found = true; }

        //load preference
            if (v[checking_inst].isLoad()) { chosen = checking_inst; break; }
        }

        //ALU preference
        if (found && !v[chosen].isLoad()) {
            for (int checking_inst = i; checking_inst < end; ++checking_inst) {
                if (canMoveUp(v, i, checking_inst) && v[checking_inst].isALU()) { chosen = checking_inst; break; }
            }
        }


        out.push_back(v[chosen]);
        v.erase(v.begin() + chosen);
        
    }
    
    return out;
}
