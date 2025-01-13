#include <cassert>
#include <fstream>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

constexpr char WALL = '#';
constexpr char OPEN_SPOT = '.';
constexpr char EXIT = 'E';
constexpr char START = 'S';

typedef long long ll;
typedef pair<int, int> pii;

pii dl = {0, -1};
pii dr = {0, 1};
pii du = {-1, 0};
pii dd = {1, 0};
vector<pii> deltas = {dl, dr, du, dd};

enum class dir { LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3, UNKNOWN = 4 };
string dirts(dir d) {
  switch (d) {
    case dir::LEFT:
      return "LEFT";
    case dir::RIGHT:
      return "RIGHT";
    case dir::UP:
      return "UP";
    case dir::DOWN:
      return "DOWN";
    case dir::UNKNOWN:
      return "UNKNOWN";
    default:
      assert(false);
  }
}

string operator~(pii p) {
  return "{" + to_string(p.first) + "," + to_string(p.second) + "}";
}

ostream& operator<<(ostream& o, pii rhs) {
  o << ~rhs;
  return o;
}

pii operator+(pii lhs, pii rhs) {
  return {lhs.first + rhs.first, lhs.second + rhs.second};
}

pii operator-(pii lhs, pii rhs) {
  return {lhs.first - rhs.first, lhs.second - rhs.second};
}

struct hash_pair {
  template <class T1, class T2>
  size_t operator()(const pair<T1, T2>& p) const {
    // Hash the first element
    size_t hash1 = hash<T1>{}(p.first);
    // Hash the second element
    size_t hash2 = hash<T2>{}(p.second);
    // Combine the two hash values
    return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
  }
};

void pg(vector<vector<char>> grid) {
  for (const vector<char>& row : grid) {
    for (char c : row) {
      cout << c;
    }
    cout << endl;
  }
}

string pv(vector<pii> v) {
  string s;
  string separator = "";
  for (pii p : v) {
    s += separator + ~p;
    separator = ", ";
  }
  return s;
}

bool inb(pii pos, size_t max_i, size_t max_j) {
  return 0 <= pos.first && pos.first < max_i && 0 <= pos.second &&
         pos.second < max_j;
}

ll find_minimum_score() { return 0; }

void solve(string file_name) {
  ifstream fin(file_name);
  pii exit_pos;
  pii start_pos;
  vector<vector<char>> grid;
  string line;
  while (getline(fin, line)) {
    vector<char> row;
    for (int j = 0; j < line.length(); ++j) {
      row.push_back(line[j]);
      if (line[j] == EXIT) {
        exit_pos = {grid.size(), j};
      } else if (line[j] == START) {
        start_pos = {grid.size(), j};
      }
    }
    grid.push_back(row);
  }

  size_t maxi = grid.size();
  size_t maxj = grid[0].size();
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[i].size(); ++j) {
      char c = grid[i][j];
      pii cur_pos = {i, j};
      if (c != WALL) {
        // Now look up down left and right
        pii left = cur_pos + dl;
        pii right = cur_pos + dr;
        pii up = cur_pos + du;
        pii down = cur_pos + dd;
      }
    }
  }
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
