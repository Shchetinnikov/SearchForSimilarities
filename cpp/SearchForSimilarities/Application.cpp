#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>

using namespace std;
namespace fs = std::filesystem;


#include "include/SlidingWindow.h"


void run_sliding_window(SlidingWindow* sw, string dna_file, string target_file) {
    vector<float> f_evals;
    
    f_evals = sw->evaluate(dna_file);

    cout << f_evals.size() << endl;


    ofstream output(target_file);
    for (auto f : f_evals)
        output << f << endl;
    output.close();

    sw->clear_evaluations();
}


int main() {
    /// PROGRAMM:
    ///     TARGET: find similarities of borders in chromosomes
    ///     DATA:
    ///         sequences: set of chromosomes (size 25)
    ///         alignments(MAHDS): - exon-intron(forward)
    ///                            - exon-intron(inverse)
    ///                            - intron-exon(forward)
    ///                            - intron-exon(inverse)
    ///         returns: files of similarities

    //vector<string> dna_paths;          // dna directory paths
    //vector<string> align_paths;        // alignment directory paths
    //string path;                       // current path

    ///******************** DNA files ********************/

    //path = "../../../../../data/dna/";
    //for (const auto& entry : fs::directory_iterator(path))
    //    dna_paths.push_back(entry.path().string());

    //
    ///****************** Aligments files *****************/

    //path = "../../../../../data/methods/mahds/align/selection/fasta/";
    //for (const auto& entry : fs::directory_iterator(path))
    //    align_paths.push_back(entry.path().string());


     //vector<float> f_evals;

    //bool exists = false;
    //string file_name = "results_shuffled.txt";
    //path = "../../../../../data/dna/";
    //for (const auto& entry : fs::directory_iterator(path))
    //    if (entry.path().string() == path + file_name) {
    //        exists = true;
    //        break;
    //    }

    //if (!exists) {
    //    cout << "****** Test-1 ******" << endl;

    //    cout << dna_paths[4] << endl;

    //   try {
    //        SlidingWindow sw(align_paths[0], dna_paths[4]);
    //        f_evals = sw.evaluate();
    //    }
    //    catch (std::bad_alloc e) {
    //        e.what();
    //        cout << "****** MemoryError ******" << endl;
    //    }
    //    cout << endl;
    //    cout << f_evals.size() << endl;

    //    ofstream output(path + file_name);

    //    for (auto f : f_evals)
    //        output << f << endl;
    //}
    //else {
    //    ifstream input(path + file_name);
    //    string line;
    //    
    //    while (getline(input, line)) {
    //        f_evals.push_back(stof(line));
    //    }

    //    for (auto f : f_evals)
    //        cout << f << " " << endl;
    //}


    //////////////////////////// TEST //////////////////////////////////
#if 1
    /// TEST-1:
    ///     TARGET: find F-threashhold
    ///     DATA:
    ///         sequence: piece of chromosome-1 (from N to N)
    ///         alignment(MAHDS): exon-intron(forward)
    ///         returns: files of F for:
    ///                     - original sequence
    ///                     - shuffled sequence

    string path = "C:/Users/kshch/Projects/diploma/source/data/";
    string target_path = path + "methods/slidingwindow/align/";

    string dna_test_file = path + "dna/test.fasta";
    string dna_test_sh_file = path + "dna/test_shuffled.fasta";
    string align_file = path + "dna/exon_intron_forward_200.fasta";
    
    float d = 0;
    float e = 0;

    string test = "_d=" + to_string(d) + ",e=" + to_string(e);
    string file_name = "res" + test + ".txt";
    string file_name_sh = "res_shuffled" + test + ".txt";


    bool exists = false;
    bool exists_sh = false;

    for (const auto& entry : fs::directory_iterator(target_path))
        if (entry.path().string() == target_path + file_name) {
            exists = true;
            break;
        }

    for (const auto& entry : fs::directory_iterator(target_path))
        if (entry.path().string() == target_path + file_name_sh) {
            exists_sh = true;
            break;
        }


    SlidingWindow* sw = new SlidingWindow(align_file);
    try {
        if (exists && exists_sh) 
            cout << "Files already exist." << endl;
        else if (exists) {
            cout << "File for original sequence already exists." << endl;
            run_sliding_window(sw, dna_test_sh_file, target_path+file_name_sh);
        }
        else if (exists_sh) {
            cout << "File for shuffled sequence already exists." << endl;
            run_sliding_window(sw, dna_test_file, target_path+file_name);
        }
        else {
            run_sliding_window(sw, dna_test_file, target_path + file_name);
            cout << endl << endl;
            run_sliding_window(sw, dna_test_sh_file, target_path + file_name_sh);
        }
    }
    catch (std::bad_alloc e) {
        e.what();
        cout << "****** Error. Buffer overflowed. ******" << endl;
    }
    delete sw;
#elif 0
    /// TEST-2:
    ///     TARGET: check the algorithm
    ///     DATA:
    ///         sequences: exon-intron(forward) borders
    ///         alignment(MAHDS): exon-intron(forward)
    ///         returns: files of F for each sequence
    /// 
    /// 

    string path = "C:/Users/kshch/Projects/diploma/source/data/";
    //string target_path = path + "methods/slidingwindow/exon_intron(forward)/";
    string target_path = path + "methods/slidingwindow/align/";

    string dna_test_file = path + "dna/exon_intron_forward_200.fasta";
    string dna_test_sh_file = path + "dna/exon_intron_forward_200_shuffled.fasta";
    //string align_file = path + "methods/mahds/align/selection/fasta/exon_intron_forward_200.fasta";
    string align_file = path + "dna/exon_intron_forward_200.fasta";
    
    float d = 1;
    float e = 0.25;

    string test = "_d=" + to_string(d) + ",e=" + to_string(e);
    string file_name = "res" + test + ".txt";
    string file_name_sh = "res_shuffled" + test + ".txt";

    bool exists = false;
    bool exists_sh = false;

    for (const auto& entry : fs::directory_iterator(target_path))
        if (entry.path().string() == target_path + file_name) {
            exists = true;
            break;
        }

    for (const auto& entry : fs::directory_iterator(target_path))
        if (entry.path().string() == target_path + file_name_sh) {
            exists_sh = true;
            break;
        }


    SlidingWindow* sw = new SlidingWindow(align_file);
    try {
        if (exists && exists_sh)
            cout << "Files already exist." << endl;
        else 
        if (exists) {
            cout << "File for original sequence already exists." << endl;
            run_sliding_window(sw, dna_test_sh_file, target_path + file_name_sh);
        }
        else if (exists_sh) {
            cout << "File for shuffled sequence already exists." << endl;
            run_sliding_window(sw, dna_test_file, target_path + file_name);
        }
        else {
            run_sliding_window(sw, dna_test_file, target_path + file_name);
            cout << endl << endl;
            run_sliding_window(sw, dna_test_sh_file, target_path + file_name_sh);
        }
    }
    catch (std::bad_alloc e) {
        e.what();
        cout << "****** Error. Buffer overflowed. ******" << endl;
    }
    delete sw;

#endif // 0
    return 0;
}