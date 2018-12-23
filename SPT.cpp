#include "SPT.h"
#define INF 0x3f3f3f3f

void SPT::add_vertex(int u) {
    vertices.insert(u);
    adj[u] = vector<int>();
    revadj[u] = vector<int>();
}

bool SPT::add_edge(int u, int v, int wt) {
    if(!vertices.count(u) || !vertices.count(v)) {
        return false;
    }

    adj[u].push_back(v);
    revadj[v].push_back(u);
    weight[make_pair(u,v)] = wt;
    return true;
}

void SPT::build_static_SPT() {

    dist.resize(vertices.size());
    fill(dist.begin(),dist.end(),INF);
    spt.resize(vertices.size());
    fill(spt.begin(),spt.end(),make_pair(-1,-1));
    priority_queue<pair<int,int>, vector <pair<int,int>> , greater<pair<int,int>> > pq; 
    pq.push(make_pair(0, source)); 
    dist[source] = 0; 
    spt[source] = make_pair(-1,-1);
  
    while (!pq.empty()) 
    {
        int u = pq.top().second; 
        pq.pop(); 
  
        for (auto &v : adj[u]) 
        {
            int wt = weight[make_pair(u,v)]; 
  
            if (dist[v] > dist[u] + wt) 
            {
                dist[v] = dist[u] + wt;
                spt[v] = make_pair(u,wt);
                pq.push(make_pair(dist[v], v));
            }
        } 
    }
}

bool SPT::change_weight(int u,int v, int newwt) {
    if(!vertices.count(u) || !vertices.count(v) || weight.find(make_pair(u,v)) == weight.end()) {
        return false;
    }
    
    int oldwt = weight[make_pair(u,v)];
    weight[make_pair(u,v)] = newwt;
    
    if(newwt > oldwt && spt[v].first == u) {
        int diff = newwt - oldwt;
        modify_inc(u,v,oldwt,diff);
        return true;
    }

    else if(newwt < oldwt && dist[u] + newwt < dist[v]) {
        int diff = dist[u] + newwt - dist[v];
        modify_dec(u,v,oldwt,diff);
        return true;
    }
    return true;

}

void SPT::modify_inc(int u, int v, int oldwt, int diff) {
    set<int> M;
    unordered_map<int,int> M_inc;
    M.insert(v);
    M_inc[v] = diff;
    multiset<pair<int,pair<int,int>>> Q;
    Q.insert(make_pair(diff,make_pair(u,v)));
    
    vector<pair<int,int>> treeadj[spt.size()];
    int root;// get adjacency list representation of tree
    for (int i = 0; i < spt.size(); i++) { 
        if (spt[i].first == -1) 
            root = i; 
        else
            treeadj[spt[i].first].push_back(make_pair(i,spt[i].second));
    }

    vector<int> desc_v;
    descendants(v,desc_v,treeadj);
    for(auto &d: desc_v) {
        int min_inc = diff;
        pair<int,int> e_min_inc = make_pair(u,v);
        for(auto &sd: revadj[d]) {
            if(find(desc_v.begin(),desc_v.end(),sd) == desc_v.end()) {
                min_inc = min(dist[sd] + weight[make_pair(sd,d)] - dist[d], min_inc);
                e_min_inc = make_pair(d,sd);
            }
        }
        if(min_inc < M_inc[d]) {
            Q.insert(make_pair(min_inc,e_min_inc));
            M_inc[d] = min_inc;
        }
        for(auto &k:treeadj[d]) {
            M.insert(k.first);
            M_inc[k.first] = M_inc[d];
        }
    }

    while(!Q.empty()) {
        pair<int,pair<int,int>> e1 = *(Q.begin());
        Q.erase(Q.begin());
        spt[e1.second.second] = make_pair(e1.second.first,weight[e1.second]);
        treeadj[e1.second.first].push_back(make_pair(e1.second.second,e1.first));
        vector<int> desc_end_e1;
        descendants(e1.second.second,desc_end_e1,treeadj);
        for(auto &i:desc_end_e1) {
            dist[i] = dist[i] + e1.first;
            M.erase(i);
        }
        for(auto &i:Q) {
            if(find(desc_end_e1.begin(), desc_end_e1.end(), i.second.second) != desc_end_e1.end())
                Q.erase(i);
        }

        for(auto &i:desc_end_e1) {
            for(auto &j: M) {
                if(M.find(i) == M.end() && find(desc_end_e1.begin(),desc_end_e1.end(),j) == desc_end_e1.end() && weight.find(make_pair(i,j)) != weight.end()) {
                    int min_inc = dist[i] + weight[make_pair(i,j)] - dist[j];
                    if(min_inc < M_inc[j]) {
                        M_inc[j] = min_inc;
                        Q.insert(make_pair(min_inc,make_pair(i,j)));
                    }
                }
            }
        }
    }
}

void SPT::modify_dec(int u, int v, int oldwt, int diff) {
    
    multiset<pair<int,pair<int,int>>> Q;
    
    vector<pair<int,int>> treeadj[spt.size()];
    int root;// get adjacency list representation of tree
    for (int i = 0; i < spt.size(); i++) { 
        if (spt[i].first == -1) 
            root = i; 
        else
            treeadj[spt[i].first].push_back(make_pair(i,spt[i].second));
    }

    vector<int> desc_v;
    descendants(v,desc_v,treeadj);
    for(auto &d: desc_v) {
        dist[d] += diff;
        for(auto &j:adj[d]) {
            int min_inc = dist[d] + weight[make_pair(d,j)] - dist[j];
            if(min_inc < 0)
                Q.insert(make_pair(min_inc,make_pair(d,j)));
        }
    }

    while(!Q.empty()) {
        pair<int,pair<int,int>> e1 = *(Q.begin());
        Q.erase(Q.begin());
        spt[e1.second.second] = make_pair(e1.second.first,weight[e1.second]);
        treeadj[e1.second.first].push_back(make_pair(e1.second.second,e1.first));
        vector<int> desc_end_e1;
        descendants(e1.second.second,desc_end_e1,treeadj);
        for(auto &i:desc_end_e1) {
            dist[i] = dist[i] + e1.first;
        }
        for(auto &i:Q) {
            if(find(desc_end_e1.begin(), desc_end_e1.end(), i.second.second) != desc_end_e1.end())
                Q.erase(i);
        }

        for(auto &i:desc_end_e1) {
            for(auto &j:adj[i]) {
                if(find(desc_end_e1.begin(),desc_end_e1.end(),j) == desc_end_e1.end()) {
                    int min_inc = dist[i] + weight[make_pair(i,j)] - dist[j];
                    if(min_inc < 0) {
                        Q.insert(make_pair(min_inc,make_pair(i,j)));
                    }
                }
            }
        }
    }
}

void SPT::descendants(int s, vector<int> &res, vector<pair<int,int>> treeadj[]) {
    for(auto &child: treeadj[s]) {
        res.push_back(child.first);
        descendants(child.first, res, treeadj);
    }
}

vector<pair<int,int>> SPT::find_SPT() {
    return spt;
}