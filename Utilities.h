#pragma once
#include "Instr.h"

int findLastProducer(const vector<Instr>& prog, int idx, int reg);

string cut_space(string s);
int num_reg(string& t);
vector<string> split_fun( string line);
