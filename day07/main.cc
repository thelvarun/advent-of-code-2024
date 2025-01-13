#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

typedef long long ll;

void parse_line(string& line, ll& target, vector<ll>& operands) {
  stringstream ss(line);
  string target_str;
  ss >> target_str;
  target = stoll(target_str.substr(0, target_str.size() - 1));
  string x;
  while (ss.good()) {
    ss >> x;
    operands.push_back(stoi(x));
  }
}

bool foo(ll cur_val, ll index, vector<ll>& operands, ll target) {
  if (index == operands.size()) {
    return cur_val == target;
  }
  return foo(cur_val + operands[index], index + 1, operands, target) ||
         foo(cur_val * operands[index], index + 1, operands, target) ||
         foo(cur_val * (pow(10, to_string(operands[index]).size())) +
                 operands[index],
             index + 1, operands, target);
}

bool could_work(ll target, vector<ll>& operands) {
  return foo(operands[0], 1, operands, target);
}

void solve() {
  ifstream fin("day7.in");
  ll sum = 0;
  string line;
  while (getline(fin, line)) {
    ll target;
    vector<ll> operands;
    parse_line(line, target, operands);
    if (could_work(target, operands)) {
      sum += target;
    }
  }
  cout << sum << endl;

  return;
}
int main() {
  solve();
  return 0;
}