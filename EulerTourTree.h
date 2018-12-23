#ifndef EulerTourTree_H
#define EulerTourTree_H

#include "BinarySearchTree.h"
#include <bits/stdc++.h>
using namespace std;

struct hash_pair {
	template <class T1,class T2> 
		size_t operator()(const pair<T1,T2> &p) const {
			auto h1 = hash<T1>{}(p.first);
			auto h2 = hash<T2>{}(p.second);
			return int(((long long)(1000000007)) * h1 + h2);
	}
};

class EulerTourTree {
public:
	bool link(int u, int v);
	bool cut(int u, int v);
	bool is_connected(int u, int v);
	
	int size(int u);
    int get_adjacent(int u, bool is_treeedge);
	void update_adjacent(int u, int add_adj, bool is_treeedge);

private:
	// Data members in EulerTourTree
	unordered_map<int, node*> IDtoNode;					//map int ID's of vertices to nodes in BST
	unordered_map<int, set<node*> > NodeSet;
	unordered_map<pair<int, int>, node*, hash_pair> edgemap;//maps (u,v) edge to node u
	unordered_map<int, int> adj_map[2];						//map a vertex to number of adjacent vertices-tree and non-tree
	//Data members end

	node * get_node(int u);
	node * get_edge(int u, int v);
	void add_node(int u, node * nn);
	void add_edge(int u, int v, node * nn);
	void remove_node(int u, node * nn);
	void remove_edge(int u, int v);
	void re_root(node * nn);
};

#endif

