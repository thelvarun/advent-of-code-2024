#include <iostream>
#include <fstream>

using namespace std;

void solve(string file_name) {
	ifstream fin(file_name);
	cout << "file name: " << file_name << endl;
	return;	
}

int main(int argc, char* argv[]) {
	// Default to sample.in if no arguments were provided.
	string file_name = "sample.in";
	if (argc == 2) {
		file_name = argv[1];
	}
	solve(file_name);
	return 0;
}
