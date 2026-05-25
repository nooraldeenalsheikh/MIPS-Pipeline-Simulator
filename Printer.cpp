#include "Printer.h"

vector<vector<string>> buildTable( vector<Instr>& prog, vector<StallEv>& stalls, int lastWR) {

    vector<vector<string>> table(prog.size(), vector<string>(lastWR + 1 ,"   "));

    for (int i = 0; i < prog.size(); ++i) {
        if (prog[i].IF <= lastWR) table[i][prog[i].IF] = "IF ";
        if (prog[i].RR <= lastWR) table[i][prog[i].RR] = "RR ";
        if (prog[i].EX <= lastWR) table[i][prog[i].EX] = "EX ";
        if (prog[i].MA <= lastWR) table[i][prog[i].MA] = "MA ";
        if (prog[i].WR <= lastWR) table[i][prog[i].WR] = "WR ";
    }

    for ( StallEv &st : stalls) {
        for (int c = st.startCycle; c <= st.endCycle && c <= lastWR; ++c) {
            table[st.instr_number][c] = " - ";
        }
    }

    return table;
}

void printSchedule(vector<Instr>& prog,vector<vector<string>>& table, int lastWR) 
{
    cout << "\n--- Schedule (IF RR EX MA WR) ---\n";
    cout << "Cycles: ";
    for (int c = 1; c <= lastWR; ++c) cout << setw(4) << c;
    cout << "\n";
    
    for (int i = 0; i < prog.size(); i++) {
        cout << setw(4) << ("I" + to_string(i+1)) << ":     ";
        for (int c = 1; c <= lastWR; ++c) cout << setw(4) << table[i][c];
        cout << "   |  " << prog[i].text << "\n";
    }
}

void printForwardings(bool enableForwarding, vector<FwdEv>& forwardings) {
    cout << "\n--- Forwarding (I#.stage -> I#.EX at cycle) ---\n";
    if (!enableForwarding) {
        cout << "Forwarding is OFF\n";
    }
    else if (forwardings.empty()) {
        cout << "None\n";
    }
    else {
        for (FwdEv& f : forwardings) {
            cout << "I" << (f.producer_ins + 1) << "." << f.prodStage
                << "  ->  I" << (f.consumer_ins + 1) << ".EX"
                << "   (used at cycle " << f.consEX << ")\n";
        }
    }
}

void printStalls(vector<StallEv>& stalls) {
    cout << "\n--- Stalls ---\n";
    if (stalls.empty()) cout << "None\n";
    else {
        for (StallEv& st : stalls) {
            cout << "I" << (st.instr_number + 1) << " stalled " << st.stalls_count
                << " cycle(s) at cycles " << st.startCycle << " .. " << st.endCycle << "\n";
        }
    }
}
