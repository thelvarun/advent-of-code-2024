#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

typedef long long ll;
typedef pair<ll, ll> pll;

ostream& operator<<(ostream& o, const pll& rhs) {
  o << "(" << rhs.first << "," << rhs.second << ")";
  return o;
}

struct parse_input_struct {
  pll a;
  pll b;
  pll prize;
};

pll parse_button(string& line) {
  stringstream ss(line);
  string token;
  ss >> token;  // Button
  ss >> token;  // Letter
  ss >> token;  // X...
  ll x = stoll(token.substr(token.find('+') + 1, token.length() - 1));
  ss >> token;  // Y...
  ll y = stoll(token.substr(token.find('+') + 1));
  return {x, y};
}

pll parse_prize(string& line) {
  stringstream ss(line);
  string token;
  ss >> token;  // Prize
  ss >> token;  // X...
  ll x = stoll(token.substr(token.find('=') + 1, token.length() - 1));
  ss >> token;  // Y...
  ll y = stoll(token.substr(token.find('=') + 1));
  return {x, y};
}

parse_input_struct parse_input(string& line1, string& line2, string& line3) {
  return {parse_button(line1), parse_button(line2), parse_prize(line3)};
}

ll min_tokens(pll a, pll b, pll prize) {
  ll min_token = -1;
  for (int a_button_press = 0; a_button_press <= 500; ++a_button_press) {
    for (int b_button_press = 0; b_button_press <= 500; ++b_button_press) {
      if (a_button_press == 0 && b_button_press == 0) {
        continue;
      }
      ll cost = a_button_press * 3 + b_button_press;
      pll loc = {a.first * a_button_press + b.first * b_button_press,
                 a.second * a_button_press + b.second * b_button_press};
      if (loc.first == prize.first && loc.second == prize.second) {
        if (min_token == -1) {
          min_token = cost;
        } else {
          min_token = min(min_token, cost);
        }
      }
    }
  }
  return min_token;
}

void solve(string file_name) {
  ifstream fin(file_name);
  string line1;
  string line2;
  string line3;
  ll output = 0;
  while (getline(fin, line1)) {
    getline(fin, line2);
    getline(fin, line3);
    parse_input_struct pis = parse_input(line1, line2, line3);
    ll min_token = min_tokens(pis.a, pis.b, pis.prize);
    if (min_token != -1) {
      cout << "min token to reach " << pis.prize << "=" << min_token << endl;
      output += min_token;
    }
    getline(fin, line1);  // skip the blank line
  }
  cout << output << endl;
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
