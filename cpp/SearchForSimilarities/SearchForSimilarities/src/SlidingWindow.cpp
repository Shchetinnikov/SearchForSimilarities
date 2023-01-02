#include "SlidingWindow.h"


#include <iostream>


/**************** Constructor ****************/
SlidingWindow::SlidingWindow(const string align_path, const string dna_path) {
    unsigned int L;

    this->align_path = align_path;
    this->dna_path = dna_path;
    this->alignment = new Alignment(align_path);
    this->alignment->calculate_pwm();

    L = this->alignment->get_length();
    this->S1 = vector<unsigned int>(L, 0);
    this->S2 = vector<unsigned int>(L, 0);
    for (unsigned int i = 0; i < L; i++) { S2[i] = i + 1; }
}


/**************** Methods ****************/
vector<float> SlidingWindow::evaluate() {
    ifstream input;

    this->open_file(input, this->dna_path);

    unsigned int L = this->alignment->get_length();

    vector<vector<float>> pwm = this->alignment->get_pwm();

    string input_line;            // input line
    string window_line = "";      // line under window visor
    float F_max;                  // evaluation function

    getline(input, input_line);   // skip heading

    while (!input.eof()) {
        if (window_line.length() < L) {
            getline(input, input_line);
            input_line = regex_replace(input_line, regex("N"), "");
            window_line += input_line;
        }
        else {
            input_line = window_line.substr(0, L);
            codering(this->S1, input_line);

            NeedlemanWunsch* NW = new NeedlemanWunsch(this->S1, this->S2, pwm);
            F_max = NW->calc_F(L, L);

            cout << "Fi_max: " << F_max << endl;

            this->f_eval.push_back(F_max);

            window_line = window_line.substr(offset, window_line.length() - offset);
            delete NW;
        }
    }
    input.close();

    return this->f_eval;
}



/**************** Secondary functions ****************/
void SlidingWindow::open_file(ifstream& input, const string& path) {
    input.open(path);

    if (!input.is_open()) {
        throw runtime_error("Error. Could not open file.");
    }
    return;
}

void SlidingWindow::codering(vector<unsigned int>& S1, const string& line) const {
    for (unsigned int i = 0; i < line.length(); i++) {
        switch (line[i])
        {
        case 'A':
            S1[i] = 1;
            break;
        case 'T':
            S1[i] = 2;
            break;
        case 'C':
            S1[i] = 3;
            break;
        case 'G':
            S1[i] = 4;
            break;
        default:
            break;
        }
    }
}