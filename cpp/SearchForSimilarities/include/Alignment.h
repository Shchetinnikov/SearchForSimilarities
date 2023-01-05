#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <numeric>

using namespace std;

class Alignment {
private:
	unsigned int L;				   // length of sequence
	int N;						   // count of sequences
	vector<string> aligned_lines;  // aligned sequences
	vector<vector<float>> pwm;     // positional-weight matrix
private:
	void open_file(ifstream& input, const string& path) const;	
	void bases_frequancy(vector<vector<float>>& freq_arr, const vector<string>& lines) const;
	void bases_proba(vector<float>& proba, const vector<vector<float>>& freq_arr) const;
	void bases_pairs(vector<vector<float>>& t, const vector<string>& lines, const vector<vector<float>>& freq_arr) const;
	void pwm_matrix(vector<vector<float>>& pwm, const vector<vector<float>>& t, const vector<float>& f, const vector<float>& proba);
public:
	Alignment(const string align_file);
public:
	int get_count()  { return this->N; }
	unsigned int get_length() { return this->L; }
	vector<vector<float>> get_pwm() { return this->pwm; }
public:
	vector<vector<float>> calculate_pwm();
public:
	~Alignment() {}
};