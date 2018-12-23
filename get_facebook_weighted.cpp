#include <bits/stdc++.h>
using namespace std;

int main() {
	ifstream ifs("facebook_combined.txt");
	ofstream ofs("facebook_combined_weighted.txt");
	int a,b;
	ifs >> a >> b;
	ofs << a << " " << b << endl;
	while(ifs >> a >> b)
		ofs << a << " " << b << " " << (rand() % 100) + 1 << endl;
  	return 0;
}