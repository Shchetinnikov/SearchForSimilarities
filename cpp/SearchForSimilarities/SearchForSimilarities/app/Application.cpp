#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>

using namespace std;
namespace fs = std::filesystem;


#include "SlidingWindow.h"


int main() {
    vector<string> dna_paths;          // dna directory paths
    vector<string> align_paths;        // alignment directory paths
    string path;                       // current path

    /******************** DNA files ********************/

    path = "../../data/dna/fasta/";
    for (const auto& entry : fs::directory_iterator(path))
        dna_paths.push_back(entry.path().string());


    /****************** Aligments files *****************/

    path = "../../data/methods/mahds/align/selection/fasta/";
    for (const auto& entry : fs::directory_iterator(path))
        align_paths.push_back(entry.path().string());


    ////////////////////////// TEST(exon_intron(forward, first segment) ///////////////////////////
    vector<float> f_evals;

    bool exists = false;
    string file_name = "results.txt";
    path = "../../data/dna/fasta/";
    for (const auto& entry : fs::directory_iterator(path))
        if (entry.path().string() == path + file_name) {
            exists = true;
            break;
        }

    if (!exists) {
        cout << "****** Test-1 ******" << endl;
        cout << dna_paths[1] << endl;

        try {
            SlidingWindow sw(align_paths[0], dna_paths[1]);
            f_evals = sw.evaluate();
        }
        catch (std::bad_alloc e) {
            e.what();
            cout << "****** MemoryError ******" << endl;
        }
        cout << endl;
        cout << f_evals.size() << endl;

        ofstream output(path + file_name);

        for (auto f : f_evals)
            output << f << endl;
    }
    else {
        ifstream input(path + file_name);
        string line;
        
        while (getline(input, line)) {
            f_evals.push_back(stof(line));
        }

        for (auto f : f_evals)
            cout << f << " " << endl;
    }
   

    return 0;
}