#include <cassert>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

void solve2() {
  ifstream fin("day1.in");
  vector<int> left;
  unordered_map<int, int> counts;
  int a;
  int b;
  while ((fin >> a) && (fin >> b)) {
    left.push_back(a);
    if (counts.find(b) != counts.end()) {
      counts[b] = counts[b] + 1;
    } else {
      counts[b] = 1;
    }
  }

  int score = 0;
  for (int x : left) {
    if (counts.find(x) != counts.end()) {
      score += x * counts[x];
    }
  }
  cout << score << endl;
}

void solve() {
  ifstream fin("day1.in");
  vector<int> left;
  vector<int> right;
  int sum = 0;
  int a;
  int b;
  while ((fin >> a) && (fin >> b)) {
    cout << "a: " << a << ", b: " << b << endl;
    left.push_back(a);
    right.push_back(b);
  }
  sort(left.begin(), left.end());
  sort(right.begin(), right.end());

  assert(left.size() == right.size());

  for (int i = 0; i < left.size(); ++i) {
    sum += abs(left[i] - right[i]);
  }
  cout << sum << endl;
}

int main() {
  solve2();
  return 0;
}