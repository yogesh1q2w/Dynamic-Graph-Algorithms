#include "connectivity.h"

void connectivity::add_vertex(int u) {
	vertices.insert(u);
	spf.push_back(EulerTourTree());
	adj.push_back(unordered_map<int, unordered_multiset<int> >());
	treeadj.push_back(unordered_map<int, unordered_multiset<int> >());
	//cout << "OK tested add_vertex" << endl;
}

void connectivity::add_edge_level(int u, int v, int level, bool is_treeedge) {
	if(u > v)
		swap(u, v);
	edge_level[make_pair(u,v)].insert(level);

	if(is_treeedge){
		treeadj[level][u].insert(v);
		treeadj[level][v].insert(u);
	}
	else {
		adj[level][u].insert(v);
		adj[level][v].insert(u);
	}

	spf[level].update_adjacent(u, 1, is_treeedge);
	spf[level].update_adjacent(v, 1, is_treeedge);
}

void connectivity::remove_edge_level(int u, int v, int level, bool is_treeedge) {
	if(u > v)
		swap(u, v);
	pair <int ,int> p = make_pair(u, v);
	edge_level[p].erase(edge_level[p].find(level));

	if(is_treeedge) {
		treeadj[level][u].erase(treeadj[level][u].find(v));
		treeadj[level][v].erase(treeadj[level][v].find(u));
	}
	else {
		adj[level][u].erase(adj[level][u].find(v));
		adj[level][v].erase(adj[level][v].find(u));
	}

	spf[level].update_adjacent(u, -1, is_treeedge);
	spf[level].update_adjacent(v, -1, is_treeedge);
}

int connectivity::level(int u, int v) {
	if(u > v)
		swap(u, v);
	auto it = edge_level.find(make_pair(u,v));
	if(it == edge_level.end())
		return -1;
	return *it->second.rbegin();
}

bool connectivity::add_edge(int u, int v) {
	if(!vertices.count(u) || !vertices.count(v)) {
		//cout<<"F"<<endl;
		return false;		//if u or v are absent in graph
	}
	if(!spf[0].is_connected(u, v)) {
		//cout << u <<"," << v << endl;
		spf[0].link(u, v);
		add_edge_level(u, v, 0, true);
	}
	else
		add_edge_level(u, v, 0, false);

	return true;
}

bool connectivity::delete_edge(int u, int v) {
	if(!vertices.count(u) || !vertices.count(v))
		return false;		//if u or v are absent in graph
	int lvl = level(u, v);
	if(lvl == -1)
		return false;
	if(!spf[0].cut(u, v)) {		//if not a tree edge
		remove_edge_level(u, v, lvl, false);
		return true;
	}
	//in case of tree edge, find a replacement edge in the tree
	remove_edge_level(u, v, lvl, true);

	for(int i = lvl; i >= 0; i--) {
		if(spf[i].size(u) > spf[i].size(v))
			swap(u, v);
		while(1) {
			int x = spf[i].get_adjacent(u, true);
			if(x == -1)
				break;
			while(!treeadj[i][x].empty()) {
				int y = *treeadj[i][x].begin();
				remove_edge_level(x, y, i, true);
				add_edge_level(x, y, i + 1, true);
				spf[i + 1].link(x, y);
			}
		}

		bool ff = false;

		while(!ff) {
			int x = spf[i].get_adjacent(u, false);
			if(x == -1) 
				break;
			while(!adj[i][x].empty()) {
				int y = *adj[i][x].begin();
				if(spf[i].is_connected(y, v)) {
					for(int j = 0; j <= i; j++)
						spf[j].link(x, y);
					ff = true;
					remove_edge_level(x, y, i, false);
					add_edge_level(x, y, i, true);
					break;
				}
				else {
					remove_edge_level(x, y, i, false);
					add_edge_level(x, y, i + 1, false);
				}
			}
		}
		if(ff)
			break;
	}
	return true;
}

bool connectivity::is_connected(int u, int v) {
	if(!vertices.count(u) || !vertices.count(v))
		return false;
	if(u == v)
		return true;
	return spf[0].is_connected(u, v);
}