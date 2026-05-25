#pragma once
#include "Common.h"

struct Instr {
    string op;
    int rd = -1, rs = -1, rt = -1;
    int IF = 0, RR = 0, EX = 0, MA = 0, WR = 0;
    string text;

    bool isLoad()  const { return op == "ld"; }
    bool isStore() const { return op == "sd"; }
    bool isALU()   const {
        return op == "dadd" || op == "dsub" || op == "add" || op == "sub";
    }
};
 