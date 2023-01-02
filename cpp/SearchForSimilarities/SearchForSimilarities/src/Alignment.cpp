#include "Alignment.h"

#include <iostream>


/**************** Constructor ****************/
Alignment::Alignment(const string align_path) {
	ifstream input;
	string line;

	this->open_file(input, align_path);

	while (getline(input, line)) {
		if (line.find("-") != -1) {
			this->aligned_lines.push_back(line);
		}
	}

	this->N = aligned_lines.size();
	this->L = aligned_lines[0].length();

	this->pwm = vector<vector<float>>(16, vector<float>(this->L, 0));

	input.close();
}


/**************** Methods ****************/
vector<vector<float>> Alignment::calculate_pwm() {
	vector<vector<float>> freq_arr(4, vector<float>(this->L, 0));	// frequency of each base in the i-position
	vector<float> proba(4, 0);										// probabilities of each base
	vector<float> f = vector<float>(16, 0);							// probabilities of finding a pair i and j
	vector<vector<float>> t(16, vector<float>(this->L, 0));			// number of pairs (i, j) in positions k-1 and k, where k=2..L, 16xL

	bases_frequancy(freq_arr, this->aligned_lines);
	bases_proba(proba, freq_arr);

	for (unsigned int i = 0; i < 4; i++) {
		for (unsigned int j = 0; j < 4; j++) {
			f[i * 4 + j] = proba[i] * proba[j];
		}
	}

	bases_pairs(t, this->aligned_lines, freq_arr);
	pwm_matrix(this->pwm, t, f, proba);

	return this->pwm;
}



/**************** Secondary functions ****************/
void Alignment::open_file(ifstream& input, const string& path) const {
	input.open(path);

	if (!input.is_open()) {
		throw runtime_error("Error. Could not open file.");
	}
	return;
}


void Alignment::bases_frequancy(vector<vector<float>>& freq_arr, const vector<string>& lines) const {
	for (auto line : lines) {
		for (unsigned int i = 0; i < this->L; i++) {
			switch (line[i])
			{
			case 'A': freq_arr[0][i] += 1; break;
			case 'T': freq_arr[1][i] += 1; break;
			case 'C': freq_arr[2][i] += 1; break;
			case 'G': freq_arr[3][i] += 1; break;
			default: break;
			}
		}
	}
}

void Alignment::bases_proba(vector<float>& proba, const vector<vector<float>>& freq_arr) const {
	unsigned int i = 0;
	int N = 0;
	vector<int> n(4, 0);

	for (auto base_freq : freq_arr) {
		N += accumulate(base_freq.begin(), base_freq.end(), 0);
	}

	for (i = 0; i < freq_arr.size(); i++) {
		n[i] += accumulate(freq_arr[i].begin(), freq_arr[i].end(), 0);
	}

	for (i = 0; i < freq_arr.size(); i++) {
		proba[i] = float(n[i]) / N;
	}
}


void Alignment::bases_pairs(vector<vector<float>>& t, const vector<string>& lines, const vector<vector<float>>& freq_arr) const {
	float freq;
	unsigned int i = 0;
	unsigned int k = 0;
	unsigned int j = 0;

	for (k = 0; k < 4; k++) {
		freq = freq_arr[k][0];
		for (j = 0; j < 4; j++, i++) {
			t[i][0] = freq;
		}
	}

	for (auto line : lines) {
		for (k = 1; k < this->L; k++) {
			char first = line[k - 1];
			char second = line[k];

			switch (first)
			{
			case 'A':
				switch (second)
				{
				case 'A': t[0][k] += 1; break;
				case 'T': t[1][k] += 1; break;
				case 'C': t[2][k] += 1; break;
				case 'G': t[3][k] += 1; break;
				default: break;
				}
				break;
			case 'T':
				switch (second)
				{
				case 'A': t[4][k] += 1; break;
				case 'T': t[5][k] += 1; break;
				case 'C': t[6][k] += 1; break;
				case 'G': t[7][k] += 1; break;
				default: break;
				}
				break;
			case 'C':
				switch (second)
				{
				case 'A': t[8][k] += 1; break;
				case 'T': t[9][k] += 1; break;
				case 'C': t[10][k] += 1; break;
				case 'G': t[11][k] += 1; break;
				default: break;
				}
				break;
			case 'G':
				switch (second)
				{
				case 'A': t[12][k] += 1; break;
				case 'T': t[13][k] += 1; break;
				case 'C': t[14][k] += 1; break;
				case 'G': t[15][k] += 1; break;
				default: break;
				}
				break;
			default: break;
			}
		}
	}
}

void Alignment::pwm_matrix(vector<vector<float>>& pwm, const vector<vector<float>>& t, const vector<float>& f, const vector<float>& proba) {
	float f_ij;
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int k = 0;

	for (k = 1; k < this->L; k++) {
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				f_ij = f[4 * i + j];
				pwm[4 * i + j][k] = (t[4 * i + j][k] - this->N * f_ij) / sqrt(this->N * f_ij * (1 - f_ij));
			}
		}
	}

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			f_ij = proba[i];
			pwm[4 * i + j][0] = (t[4 * i + j][0] - this->N * f_ij) / sqrt(this->N * f_ij * (1 - f_ij));
		}
	}
}


