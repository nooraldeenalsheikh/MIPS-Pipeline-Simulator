#include "Utilities.h"

string cut_space(string s) {
    while (!s.empty() && isspace(s.back())) s.pop_back();
    while (!s.empty() && isspace(s.front())) s.erase(s.begin());
    return s;
}

int num_reg(string& t) {
    if (t.size() >= 2 && (t[0] == 'r' || t[0] == 'R')) {
        try { return stoi(t.substr(1)); }
        catch (...) { return -1; }
    }
    return -1;
}

vector<string> split_fun( string line) {
    vector<string> split_1instruction;
    string cur;
    for (char c : line) {
        if (c == ',' || isspace(c)) {
            if (!cur.empty()) {
                split_1instruction.push_back(cur);
                cur.clear();
            }
        }
        else cur.push_back(c);
    }
    if (!cur.empty()) split_1instruction.push_back(cur);
    return split_1instruction;
}
