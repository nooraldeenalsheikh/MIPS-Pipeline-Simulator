
#include "Common.h"
#include "Instr.h"
#include "Utilities.h"
#include "Simulator.h"
#include "Printer.h"
#include "Speedup.h"
#include "Reorder.h"

int main() 
{
    cout << " 5-stage Pipeline Simulator (IF RR EX MA WR) \n";
    cout << "Enter instructions line-by-line. End with empty line.\n";
    cout << "Supported: ld, sd, dadd, dsub, add, sub\n\n";
    cout << "Enter instructions like this form:\n"
         << "ld r1, A(r0)\n"
         << "ld r2, B(r0)\n"
         << "add r3, r1, r2\n"
         << "sd r3, C(r0)\n\n"
         << "Enter:\n\n";
      
    vector<Instr> prog;
    string line;

    while (true) {
        if (!getline(cin, line)) break;
        line = cut_space(line);
        if (line.empty()) break;

        Instr ins;
        ins.text = line;

        vector <string> split = split_fun(line);
        if (split.empty()) continue;

        string op = split[0];
        for (char &ch : op) ch = tolower(ch);
        ins.op = op;

        if (op == "ld") {
            if (split.size() >= 2) ins.rd = num_reg(split[1]);
        }
        else if (op == "sd") {
            if (split.size() >= 2) ins.rs = num_reg(split[1]);
        }
        else if (op == "dadd" || op == "dsub" || op == "add" || op == "sub") {
            if (split.size() >= 3) {
                ins.rd = num_reg(split[1]);
                if (split.size() == 3) {
                    ins.rs = num_reg(split[1]);
                    ins.rt = num_reg(split[2]);
                }
                else {
                    ins.rs = num_reg(split[2]);
                    ins.rt = num_reg(split[3]);
                }
            }
        }
        else {
            cout << "Warning: unknown op '" << op << "' ignored\n";
            continue;
        }

        prog.push_back(ins);
    }

    if (prog.empty()) {
        cout << "No instructions entered.\n";
        return 0;
    }

    
    bool enableForwarding = false;
    bool enableReordering = false;

    string s;

    cout << "Enable reordering? (y/n) [n]: ";
    getline(cin, s);
    if (!s.empty() && (s[0] == 'y' || s[0] == 'Y'))
        enableReordering = true;

    if (enableReordering) {
        prog = reorderProgram(prog, 4);
    }

    cout << "Enable forwarding? (y/n) [n]: ";
    getline(cin, s);
    if (!s.empty() && (s[0] == 'y' || s[0] == 'Y'))
        enableForwarding = true;

    vector<StallEv> stalls;
    vector<FwdEv> forwardings;
    int lastWR = 0;

    simulatePipeline(prog, enableForwarding, stalls, forwardings, lastWR);

    vector<vector<string>> table = buildTable(prog, stalls, lastWR);

    printSchedule(prog, table, lastWR);
    printForwardings(enableForwarding, forwardings);
    printStalls(stalls);

    cout << "\nTotal cycles = " << lastWR << "\n";
    cout << "\nThroughput = " << prog.size() / (double)lastWR << "\n";

    cout << "\nSpeedup = " << computeSpeedup(prog) << "\n";
   
    return 0;
}
