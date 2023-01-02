#pragma once

#include <vector>
#include <algorithm>
#include <stdexcept>

using namespace std;

class NeedlemanWunsch {
protected:
    class Node {
    private:
        Node* prev_node;
        float f_max;
    public:
        Node();
        Node(float value, Node* prev_node);
    public:
        float get_F() { return this->f_max; }
        Node* get_prev() { return this->prev_node; }
        bool has_prev() { return bool(this->prev_node); }
    public:
        void set_value(float value) { this->f_max = value; }
        void set_parent(Node* node) { this->prev_node = node; }
        void set_value_parent(float value, Node* node) { 
            this->f_max = value; 
            this->prev_node = node; 
        }
    public:
        ~Node() {}
    };
private:
    vector<vector<Node*>> nodes;
    vector<unsigned int> S1;
    vector<unsigned int> S2;
    vector<vector<float>> pwm;
    unsigned int L;
    float d = 35;
    float e = 8;
public:
    NeedlemanWunsch(const vector<unsigned int>& S1, const vector<unsigned int>& S2, const vector<vector<float>>& pwm, float d = 35, float e = 8);
public:
    unsigned int get_matrix_size() { return this->L; }
private:
    float calc_Fx(unsigned int i, unsigned int j);
    float calc_Fy(unsigned int i, unsigned int j);
public:
    float calc_F(unsigned int i, unsigned int j);
public:
    ~NeedlemanWunsch() {
        for (auto node_i : this->nodes)
            for (auto node_ij : node_i)
                if (node_ij)
                    delete node_ij;
    }
};