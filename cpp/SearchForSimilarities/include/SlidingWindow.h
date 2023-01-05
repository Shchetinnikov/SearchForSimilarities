#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <regex>

#include "Alignment.h"
#include "NeedlemanWunsch.h"

using namespace std;


class SlidingWindow {
private:
    string align_file;          // dna directory paths
    string dna_file;            // alignment directory paths        
    Alignment* alignment;       // alignment
    vector<unsigned int> S1;    // rows of pwm (codering sequence)
    vector<unsigned int> S2;    // columns of pwm
    vector<float> f_eval;       // values of evaluation function during dna sequence
    unsigned int offset = 10;   // step of sliding window
private:
    void open_file(ifstream& input, const string& path);
    void codering(vector<unsigned int>& S1, const string& line) const;
public:
    SlidingWindow(const string align_file);
public:
    vector<float> get_f_eval() { return this->f_eval; }
    unsigned int get_offset() { return this->offset; }
public:
    vector<float> evaluate(const string dna_file);
    void clear_evaluations() { this->f_eval.clear(); }
public:
    ~SlidingWindow() { delete this->alignment; }
};