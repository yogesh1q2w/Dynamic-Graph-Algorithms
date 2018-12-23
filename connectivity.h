#ifndef Connectivity_H
#define Connectivity_H

#include "EulerTourTree.h"

class connectivity {
public:
	void add_vertex(int u);
	bool add_edge(int u, int v);
	bool delete_edge(int u, int v);
	bool is_connected(int u, int v);
	
private:
	set <int> vertices;
	vector <EulerTourTree> spf;
	vector <unordered_map<int, unordered_multiset<int>>> adj;
	vector <unordered_map<int, unordered_multiset<int>>> treeadj;
	unordered_map<pair<int, int>, multiset<int>, hash_pair> edge_level;
	void add_edge_level(int u, int v, int level, bool is_treeedge);
	void remove_edge_level(int u, int v, int level, bool is_treeedge);
	int level(int u, int v);
};

#endif	