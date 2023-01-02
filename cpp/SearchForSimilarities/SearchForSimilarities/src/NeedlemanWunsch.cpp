#include "NeedlemanWunsch.h"


#include <iostream>

/**************** Constructor ****************/
NeedlemanWunsch::NeedlemanWunsch(const vector<unsigned int>& S1, const vector<unsigned int>& S2, const vector<vector<float>>& pwm, float d, float e) {
    this->S1 = S1;
    this->S2 = S2;
    this->pwm = pwm;
    this->d = d;
    this->e = e;

    this->L = S1.size();
    this->nodes = vector<vector<Node*>>(this->L + 1, vector<Node*>(this->L + 1, nullptr));

    unsigned int i = 0;
    unsigned int j = 0;

    nodes[0][0] = new Node();
    for (i = 1; i <= this->L; i++) {
        for (j = 1; j <= this->L; j++) {
            nodes[i][j] = new Node();
        }
    }

    for (i = 1; i <= this->L; i++) { nodes[i][0] = new Node((-1) * this->d * i, nodes[i - 1][0]); }
    for (j = 1; j <= this->L; j++) { nodes[0][j] = new Node((-1) * this->d * j, nodes[0][j - 1]); }
}

NeedlemanWunsch::Node::Node() {
    this->prev_node = nullptr;
    this->f_max = 0;
}

NeedlemanWunsch::Node::Node(float f_value, Node* prev_node) {
    this->prev_node = prev_node;
    this->f_max = f_value;
}



/**************** Methods ****************/
float NeedlemanWunsch::calc_Fx(unsigned int i, unsigned int j) {
    if (i > this->L || j > this->L)
        throw runtime_error("Error. Indexes are out of range.");
    else if (i == 0 || j == 0)
        return nodes[i][j]->get_F();

    float r1;
    float r2;
    float max_r;

    r1 = this->calc_F(i - 1, j) - this->d;
    r2 = this->calc_Fx(i - 1, j) - this->e;

    max_r = max(r1, r2);

    if (max_r == r1)
        this->nodes[i][j + 1]->set_value_parent(max_r, nodes[i - 1][j]);
    else if (max_r == r2)
        this->nodes[i][j + 1]->set_value_parent(max_r, nodes[i - 1][j]);

    return max_r;
}


float NeedlemanWunsch::calc_Fy(unsigned int i, unsigned int j) {
    if (i > this->L || j > this->L)
        throw runtime_error("Error. Indexes are out of range.");
    else if (i == 0 || j == 0)
        return nodes[i][j]->get_F();
    
    float r1;
    float r3;
    float max_r;

    r1 = this->calc_F(i, j - 1) - this->d;
    r3 = this->calc_Fy(i, j - 1) - this->e;

    max_r = max(r1, r3);

    if (max_r == r1)
        this->nodes[i + 1][j]->set_value_parent(max_r, nodes[i][j - 1]);
    else if (max_r == r3)
        this->nodes[i + 1][j]->set_value_parent(max_r, nodes[i + 1][j - 1]);

    return max_r;
}


float NeedlemanWunsch::calc_F(unsigned int i, unsigned int j) {
    if (i > this->L || j > this->L)
        throw runtime_error("Error. Indexes are out of range.");
    else if (i == 0 || j == 0)
        return nodes[i][j]->get_F();
    else if (this->nodes[i][j] && this->nodes[i][j]->has_prev())
        return this->nodes[i][j]->get_F();

    Node* node;
    Node* prev_node;
    unsigned int n1 = 0;
    unsigned int n2 = 0;
    float r1;
    float r2;
    float r3;
    float max_r;

    if (i == 1 && j == 1) {
        n1 = S1[i - 1];

        r1 = this->calc_F(i - 1, j - 1) + this->pwm[n1 - 1][S2[j - 1] - 1];
        r2 = this->calc_F(i - 1, j) - this->d;
        r3 = this->calc_F(i, j - 1) - this->d;;
    }
    else if (i == 1) {
        n1 = S1[i - 1];

        r1 = this->calc_F(i - 1, j - 1) + this->pwm[n1 - 1][S2[j - 1] - 1];
        r2 = this->calc_F(i - 1, j) - this->d;
        r3 = this->calc_Fy(i - 1, j - 1) + this->pwm[n1 - 1][S2[j - 1] - 1];
    }
    else {
        unsigned int ii = i - 1;
        unsigned int jj = j;
        unsigned int t = 1;

        n1 = S1[i - 1 - t] + (S1[i - 1] - 1) * 4;
        r1 = this->calc_F(i - 1, j - 1) + this->pwm[n1 - 1][S2[j - 1] - 1];

        if (j == 1) r3 = this->calc_F(i, j - 1) - this->d;
        else        r3 = this->calc_Fy(i - 1, j - 1) + this->pwm[S1[i - 1] - 1][0];

        for (ii; ii > 1; ii--) {
            t++;
            node = this->nodes[ii][jj];
            prev_node = node->get_prev();
            if (prev_node == this->nodes[ii - 1][jj - 1])
                break;
        }
        
        n2 = S1[i - 1 - t] + (S1[i - 1] - 1) * 4;
        r2 = this->calc_Fx(i - 1, j - 1) + this->pwm[n2 - 1][S2[j - 1] - 1];
    }

    max_r = max({ r1, r2, r3 });

    if (max_r == r1)
        this->nodes[i][j]->set_value_parent(max_r, nodes[i - 1][j - 1]);
    else if (max_r == r2)
        this->nodes[i][j]->set_value_parent(max_r, nodes[i - 1][j]);
    else
        this->nodes[i][j]->set_value_parent(max_r, nodes[i][j - 1]);

    return max_r;
}