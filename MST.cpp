#include "MST.h"
#define INF 0x3f3f3f3f

void MST::add_vertex(int u) {
	vertices.insert(u);
	min_spf.push_back(EulerTourTree());
	adj.push_back(unordered_map<int, multiset<pair<int,int>>>());
	treeadj.push_back(unordered_map<int, multiset<pair<int,int>>>());
}

void MST::add_edge_level(int u, int v, int level, bool is_treeedge) {
	if(u > v)
		swap(u, v);
	edge_level[make_pair(u,v)].insert(level);

	if(is_treeedge){
		treeadj[level][u].insert(make_pair(weight[make_pair(u,v)],v));
		treeadj[level][v].insert(make_pair(weight[make_pair(u,v)],u));
	}
	else {
		adj[level][u].insert(make_pair(weight[make_pair(u,v)],v));
		adj[level][v].insert(make_pair(weight[make_pair(u,v)],u));
	}

	min_spf[level].update_adjacent(u, 1, is_treeedge);
	min_spf[level].update_adjacent(v, 1, is_treeedge);
}

void MST::remove_edge_level(int u, int v, int level, bool is_treeedge) {
	if(u > v)
		swap(u, v);
	pair <int ,int> p = make_pair(u, v);
	edge_level[p].erase(edge_level[p].find(level));

	if(is_treeedge) {
		treeadj[level][u].erase(treeadj[level][u].find(make_pair(weight[make_pair(u,v)],v)));
		treeadj[level][v].erase(treeadj[level][v].find(make_pair(weight[make_pair(u,v)],u)));
	}
	else {
		adj[level][u].erase(adj[level][u].find(make_pair(weight[make_pair(u,v)],v)));
		adj[level][v].erase(adj[level][v].find(make_pair(weight[make_pair(u,v)],u)));
	}

	min_spf[level].update_adjacent(u, -1, is_treeedge);
	min_spf[level].update_adjacent(v, -1, is_treeedge);
}

int MST::level(int u, int v) {
	if(u > v)
		swap(u, v);
	auto it = edge_level.find(make_pair(u,v));
	if(it == edge_level.end())
		return -1;
	return *it->second.rbegin();
}

bool MST::add_edge(int u, int v, int wt) {
	if(!vertices.count(u) || !vertices.count(v)) {
        return false;
    }

    adj_list[u].push_back(make_pair(v,wt));
	adj_list[v].push_back(make_pair(u,wt));
    weight[make_pair(u,v)] = wt;
	weight[make_pair(v,u)] = wt;

    return true;
}

bool MST::delete_edge(int u, int v) {
	if(!vertices.count(u) || !vertices.count(v))
		return false;		//if u or v are absent in graph
	int lvl = level(u, v);
	if(lvl == -1)
		return false;
	if(!min_spf[0].cut(u, v)) {		//if not a tree edge
		remove_edge_level(u, v, lvl, false);
		return true;
	}
	
	remove_edge_level(u, v, lvl, true);

	for(int i = lvl; i >= 0; i--) {
		if(min_spf[i].size(u) > min_spf[i].size(v))
			swap(u, v);
		while(1) {
			int x = min_spf[i].get_adjacent(u, true);
			if(x == -1)
				break;
			while(!treeadj[i][x].empty()) {
				int y = (*treeadj[i][x].begin()).second;
				remove_edge_level(x, y, i, true);
				add_edge_level(x, y, i + 1, true);
				min_spf[i + 1].link(x, y); 
			}
		}

		bool ff = false;

		while(!ff) {
			int x = min_spf[i].get_adjacent(u, false);
			if(x == -1) 
				break;
			while(!adj[i][x].empty()) {
				int y = (*adj[i][x].begin()).second;//get y based on edge weights and not in any order as it comes from a priority queue
				if(min_spf[i].is_connected(y, v)) {
					for(int j = 0; j <= i; j++)
						min_spf[j].link(x, y);
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

set <pair<pair<int,int>,int>> MST::find_MST()  {
	set <pair<pair<int,int>,int>> res;
	treeadj[0].erase(-1);
	for(auto &i:treeadj[0]) {
		for(auto &k:i.second) {
			res.insert(make_pair(make_pair(i.first,k.second),k.first));
		}
	}
	return res;
}

void MST::build_static_MST() {
	priority_queue< pair<int,int>, vector <pair<int,int>> , greater<pair<int,int>> > pq; 
    int src = 0;
    vector<int> key(vertices.size(), INF); 
  	vector<int> parent(vertices.size(), -1); 
  	vector<bool> inMST(vertices.size(), false); 
  	pq.push(make_pair(0, src)); 
    key[src] = 0; 
    while(!pq.empty()) { 
    	int u = pq.top().second; 
        pq.pop(); 
  		inMST[u] = true;
        for(auto &i:adj_list[u]) {
            int v = i.first; 
            int weight = i.second; 
  		    if(inMST[v] == false && key[v] > weight) { 
                key[v] = weight; 
                pq.push(make_pair(key[v], v)); 
                parent[v] = u; 
        	} 
        } 
    } 
	for(int i = 1; i < vertices.size(); ++i) {
		min_spf[0].link(i, parent[i]);
		add_edge_level(i, parent[i], 0, true);
	}
	for(auto &u:adj_list) {
		for(auto &v:u.second) {
			if(parent[u.first] != v.first && parent[v.first] != u.first) 
				add_edge_level(u.first, v.first , 0, false);
		}
	}
}