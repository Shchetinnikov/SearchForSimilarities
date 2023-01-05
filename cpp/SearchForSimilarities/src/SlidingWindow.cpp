#include <iostream>
#include <tuple>

#include "../include/SlidingWindow.h"


/**************** Constructor ****************/
SlidingWindow::SlidingWindow(const string align_file) {
    unsigned int L;

    this->align_file = align_file;
    this->alignment = new Alignment(align_file);
    this->alignment->calculate_pwm();

    L = this->alignment->get_length();
    this->S1 = vector<unsigned int>(100, 0);
    this->S2 = vector<unsigned int>(L, 0);
    for (unsigned int i = 0; i < L; i++) { S2[i] = i + 1; }
}


/**************** Methods ****************/
vector<float> SlidingWindow::evaluate(const string dna_file) {
    this->dna_file = dna_file;
    ifstream input;

    this->open_file(input, this->dna_file);

    unsigned int L = this->alignment->get_length();

    vector<vector<float>> pwm = this->alignment->get_pwm();

    string input_line;            // input line
    string window_line = "";      // line under window visor
    unsigned int prev_len = 0;    // previos length of window line
    float F_max;                  // evaluation function

    getline(input, input_line);   // skip heading

    while (!input.eof()) {
        if (window_line.length() < L) {
            getline(input, input_line);
            input_line = regex_replace(input_line, regex("N"), "");
            window_line += input_line;
            
            //if (window_line.length() == prev_len) {
            //    window_line = "";
            //    prev_len = 0;
            //}
            //else
            //    prev_len = window_line.length();
        }
        else {
            input_line = window_line.substr(0, L);
            codering(this->S1, input_line);

            NeedlemanWunsch* NW = new NeedlemanWunsch(this->S1, this->S2, pwm);
            F_max = NW->calc_F(L, this->S2.size());
            
            ///////////////
            //vector<tuple<unsigned int, unsigned int>> matrix_path = NW->get_matrix_path();
            /*for (int i = 0; i < matrix_path.size(); i++) {
                cout << "i: " << get<0>(matrix_path[i]) << " j: " << get<1>(matrix_path[i]) << endl;
            }*/
            cout << "Fi_max: " << F_max << endl;
            cout << endl;
            ///////////////


            this->f_eval.push_back(F_max);

            window_line = window_line.substr(offset, window_line.length() - offset);
            delete NW;
        }
    }

    //while (!input.eof()) {
    //    getline(input, input_line);
    //    codering(this->S1, input_line);

    //    cout << input_line << endl;

    //    NeedlemanWunsch* NW = new NeedlemanWunsch(this->S1, this->S2, pwm);
    //    F_max = NW->calc_F(L, this->S2.size());
    //       
    //    ///////////////
    //    //vector<tuple<unsigned int, unsigned int>> matrix_path = NW->get_matrix_path();
    //    /*for (int i = 0; i < matrix_path.size(); i++) {
    //        cout << "i: " << get<0>(matrix_path[i]) << " j: " << get<1>(matrix_path[i]) << endl;
    //    }*/
    //    cout << "Fi_max: " << F_max << endl;
    //    cout << endl;
    //    ///////////////


    //    this->f_eval.push_back(F_max);

    //    //window_line = window_line.substr(offset, window_line.length() - offset);
    //    delete NW;
    //}

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