#include "connectivity.h"
#include "MST.h"
#include "SPT.h"
#include <chrono>
#define CHNGWT 100
#define ITER 5

void test(int func,string file) {
	if(func == 1) {
		ifstream ifs(file, ifstream::in);
		ofstream ofs("timefile.txt", ofstream::out | ios::app);
		int V,E;
		ifs >> V >> E;
		connectivity g = connectivity();
		long update = 0,query = 0;
		double querytime = 0, updatetime = 0;
		for(int i = 0; i < V; i++)
			g.add_vertex(i);

		//cout << V <<", " << E << endl;
		for(int i = 0; i < E; i++) {
			int u, v;
			ifs >> u >> v;
			//cout << u << "==="<<v << endl;
			g.add_edge(u, v);

		}
		
		for(int i = 0; i < ITER; i++) {
			int qtype = rand() % 3;
			//cout << qtype << endl;
			if(qtype == 0) {
				int u = rand() % V;
				int v = rand() % V;
				auto t1 = chrono::high_resolution_clock::now();
				g.add_edge(u, v);
				auto t2 = chrono::high_resolution_clock::now();
				updatetime +=chrono::duration_cast<chrono::nanoseconds>(t2-t1).count();
				update++;
			}
			else if(qtype == 1) {
				int u = rand() % V;
				int v = rand() % V;
				auto t1 = chrono::high_resolution_clock::now();
				g.delete_edge(u, v);
				auto t2 = chrono::high_resolution_clock::now();
				updatetime +=chrono::duration_cast<chrono::nanoseconds>(t2-t1).count();
				update++;
			}
			else if(qtype == 2) {
				int u = rand() % V;
				int v = rand() % V;
				auto t1 = chrono::high_resolution_clock::now();
				bool res = g.is_connected(u, v);
				auto t2 = chrono::high_resolution_clock::now();
				querytime +=chrono::duration_cast<chrono::nanoseconds>(t2-t1).count();
				query++;
			}
		}
		ofs << updatetime/(update * 1.0) << " " << querytime/(query * 1.0) << endl;
	}
	else if(func == 2) {
		ifstream ifs(file, ifstream::in);
		ofstream ofs("timefile.txt", ofstream::out | ios::app);
		int V,E;
		ifs >> V >> E;
		MST g = MST();
		long update = 0,query = 0;
		double querytime = 0, updatetime = 0;
		for(int i = 0; i < V; i++)
			g.add_vertex(i);

		//cout << V <<", " << E << endl;
		for(int i = 0; i < E; i++) {
			int u, v, wt;
			ifs >> u >> v >> wt;
			//cout << u << "==="<<v << endl;
			g.add_edge(u, v, wt);

		}
		g.build_static_MST();
		
		for(int i = 0; i < ITER; i++) {
			int qtype = rand() % 2;
			//cout << qtype << endl;
			if(qtype == 0) {
				int u = rand() % V;
				int v = rand() % V;
				auto t1 = chrono::high_resolution_clock::now();
				g.delete_edge(u, v);
				auto t2 = chrono::high_resolution_clock::now();
				updatetime +=chrono::duration_cast<chrono::nanoseconds>(t2-t1).count();
				update++;
			}
			else if(qtype == 1) {
				auto t1 = chrono::high_resolution_clock::now();
				set <pair<pair<int,int>,int>> res = g.find_MST();
				auto t2 = chrono::high_resolution_clock::now();
				querytime +=chrono::duration_cast<chrono::nanoseconds>(t2-t1).count();
				query++;
			}
		}
		ofs << updatetime/(update * 1.0) << " " << querytime/(query * 1.0) << endl;
	}
	else if(func == 3) {
		ifstream ifs(file, ifstream::in);
		ofstream ofs("timefile.txt", ofstream::out | ios::app);
		int V,E;
		ifs >> V >> E;
		SPT g = SPT();
		long update = 0,query = 0;
		double querytime = 0, updatetime = 0;
		for(int i = 0; i < V; i++)
			g.add_vertex(i);

		// cout << V <<", " << E << endl;
		for(int i = 0; i < E; i++) {
			int u, v, wt;	int c = 0;
			ifs >> u >> v >> wt;
			//cout << u << "===" << v << endl;
			g.add_edge(u, v, wt);

		}

		g.source = rand() % V;
		g.build_static_SPT();
		
		for(int i = 0; i < ITER; i++) {
			int qtype = rand() % 2;
			//cout << qtype << endl;
			if(qtype == 0) {
				int u = rand() % V;
				int v = rand() % V;
				int wt = rand() % CHNGWT;
				auto t1 = chrono::high_resolution_clock::now();
				g.change_weight(u, v, wt);
				auto t2 = chrono::high_resolution_clock::now();
				updatetime +=chrono::duration_cast<chrono::nanoseconds>(t2-t1).count();
				update++;
			}
			else if(qtype == 1) {
				auto t1 = chrono::high_resolution_clock::now();
				vector<pair<int,int>> res =  g.find_SPT();
				auto t2 = chrono::high_resolution_clock::now();
				querytime +=chrono::duration_cast<chrono::nanoseconds>(t2-t1).count();
				query++;
			}
		}
		// cout << querytime << " " << query << endl;
		ofs << updatetime/(update * 1.0) << " " << querytime/(query * 1.0) << endl;
	}
	else {
		cout << "Wrong choice.. Enter 1,2 or 3" << endl;
	}
}

int main() {
	srand(0);
	int ch = 1;
	string st;
	while(ch != 0) {
		cout << "Enter choice -\n1- Fully dynamic connectivity\n2- Decremental MST\n3- Fully dynamic SPT\nand file name to run on:" << endl;
		cin >> ch >> st;
		test(ch,st);
	}
	return 0;
}