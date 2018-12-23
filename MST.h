#ifndef MST_H
#define MST_H

#include "EulerTourTree.h"

class MST {
public:
    unordered_map<pair<int,int>, int, hash_pair> weight;
    void add_vertex(int u);
    bool add_edge(int u, int v, int wt);
    bool delete_edge(int u, int v);
    void build_static_MST();
    set <pair<pair<int,int>,int>> find_MST();

private:
    set <int> vertices;
    vector <EulerTourTree> min_spf;
    unordered_map <int, vector< pair<int,int>>> adj_list;
    vector <unordered_map<int, multiset<pair<int,int>>>> adj;
	vector <unordered_map<int, multiset<pair<int,int>>>> treeadj;
    unordered_map<pair<int, int>, multiset<int>, hash_pair> edge_level;
    void add_edge_level(int u, int v, int level, bool is_treeedge);
	void remove_edge_level(int u, int v, int level, bool is_treeedge);
	int level(int u, int v);
};


#endif