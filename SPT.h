#ifndef SPT_H
#define SPT_H

#include "EulerTourTree.h"

class SPT {
public:
    int source;
    unordered_map<pair<int,int>,int,hash_pair> weight;
    void add_vertex(int u);
    bool add_edge(int u, int v, int wt);
    bool change_weight(int u,int v, int newwt);
    void build_static_SPT();
    vector <pair<int, int>> find_SPT();

private:
    set <int> vertices;
    unordered_map<int, vector <int>> adj;
    unordered_map<int, vector <int>> revadj;
    vector<pair<int,int>> spt;
    vector<int> dist;
    void modify_inc(int u, int v, int oldwt, int diff);
    void modify_dec(int u, int v, int oldwt, int diff);
    void descendants(int s, vector<int> &res, vector<pair<int,int>> treeadj[]);
};


#endif