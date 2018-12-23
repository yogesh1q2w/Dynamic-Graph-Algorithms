#include "connectivity.h"
#include "MST.h"
#include "SPT.h"

void test(int func,string file) {
	if(func == 1) {
		ifstream ifs(file, ifstream::in);
		int V,E;
		ifs >> V >> E;
		connectivity g = connectivity();
		for(int i = 0; i < V; i++)
			g.add_vertex(i);

		for(int i = 0; i < E; i++) {
			int u, v;
			ifs >> u >> v;
			g.add_edge(u, v);
		}

		cout << (g.is_connected(0,2)?"PASS":"FAIL") << endl;
        cout << (!g.is_connected(2,3)?"PASS":"FAIL") << endl;
        cout << (g.add_edge(2,3)?"PASS":"FAIL") << endl;
        cout << (g.is_connected(0,5)?"PASS":"FAIL") << endl;
        cout << (g.delete_edge(2,3)?"PASS":"FAIL") << endl;
        cout << (!g.is_connected(0,5)?"PASS":"FAIL") << endl;
        
	}
	else if(func == 2) {
		ifstream ifs(file, ifstream::in);
		int V,E;
		ifs >> V >> E;
		MST g = MST();
		for(int i = 0; i < V; i++)
			g.add_vertex(i);

		for(int i = 0; i < E; i++) {
			int u, v, wt;
			ifs >> u >> v >> wt;
			g.add_edge(u, v, wt);

		}
		g.build_static_MST();
        
        set <pair<pair<int,int>,int>> res = g.find_MST();
        cout << "MST edges as u v weight((u,v))" << endl;
        for(auto &i: res)
            cout << i.first.first << " " << i.first.second << " " << i.second << endl;
        cout << "Query output ends" << endl;

        cout << (g.delete_edge(2, 3)?"e ={2,3} is deleted-PASS":"FAIL") << endl;

        res = g.find_MST();
        cout << "MST edges as u v weight((u,v))" << endl;
        for(auto &i: res)
            cout << i.first.first << " " << i.first.second << " " << i.second << endl;
        cout << "Query output ends" << endl;

        cout << (g.delete_edge(3, 4)?"e ={3,4} is deleted-PASS":"FAIL") << endl;

        res = g.find_MST();
        cout << "MST edges as u v weight((u,v))" << endl;
        for(auto &i: res)
            cout << i.first.first << " " << i.first.second << " " << i.second << endl;
        cout << "Query output ends" << endl;        
	}
	else if(func == 3) {
		ifstream ifs(file, ifstream::in);
		int V,E;
		ifs >> V >> E;
		SPT g = SPT();
		for(int i = 0; i < V; i++)
			g.add_vertex(i);

		for(int i = 0; i < E; i++) {
			int u, v, wt;
			ifs >> u >> v >> wt;
			g.add_edge(u, v, wt);

		}

        g.source = 0;
		g.build_static_SPT();

        vector<pair<int,int>> res =  g.find_SPT();
        cout << "SPT source and edges as u v weight((u,v))" << endl;
        cout << "Source = " << g.source << endl;
        for(auto &i: res)
            cout << i.first << " " << i.second << endl;
        cout << "Query output ends" << endl;
        
        cout << (g.change_weight(2, 3, 1)?"e ={2,3} is changed - New wt = 1 -PASS":"FAIL") << endl;
        
        res =  g.find_SPT();
        cout << "SPT source and edges as u v weight((u,v))" << endl;
        cout << "Source = " << g.source << endl;
        for(auto &i: res)
            cout << i.first << " " << i.second << endl;
        cout << "Query output ends" << endl;
        
        cout << (g.change_weight(0, 1, 20)?"e ={0,1} is changed - New wt = 20 -PASS":"FAIL") << endl;
        
        res =  g.find_SPT();
        cout << "SPT source and edges as u v weight((u,v))" << endl;
        cout << "Source = " << g.source << endl;
        for(auto &i: res)
            cout << i.first << " " << i.second << endl;
        cout << "Query output ends" << endl;
	}
}

int main() {
	srand(0);
	test(1,"test_conn.txt");
    test(2,"test_weighted.txt");
    test(3,"test_weighted.txt");
	return 0;
}