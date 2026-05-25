#pragma once
#include "Instr.h"
#include "Hazards.h"

vector<vector<string>> buildTable( vector<Instr>& prog, vector<StallEv>& stalls,int lastWR);

void printSchedule(vector<Instr>& prog,vector<vector<string>>& table,int lastWR);

void printForwardings( bool enableForwarding,vector<FwdEv>& forwardings);

void printStalls( vector<StallEv>& stalls);
