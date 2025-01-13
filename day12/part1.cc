#include <fstream>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

typedef long long ll;
typedef pair<int, int> pll;

constexpr char marked = '.';
vector<pll> deltas = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

bool inb(size_t maxi, size_t maxj, int i, int j) {
  return 0 <= i && i < maxi && 0 <= j && j < maxj;
}

pll operator+(const pll& lhs, const pll& rhs) {
  return {lhs.first + rhs.first, lhs.second + rhs.second};
}

ostream& operator<<(ostream& o, const pll& p) {
  o << "{" << p.first << ", " << p.second << "}";
  return o;
}

struct area_perimeter {
  ll area = -1;
  ll perimeter = -1;
};

void pv(vector<vector<char>> grid) {
  for (auto x : grid) {
    for (auto y : x) {
      cout << y;
    }
    cout << endl;
  }
}

area_perimeter dfs(vector<vector<char>>& grid, int start_i, int start_j) {
  stack<pll> stack;
  ll area = 0;
  ll perimeter_shared_sides = 0;
  stack.push({start_i, start_j});
  char region = grid[start_i][start_j];
  char seen = tolower(region);
  // cout << "-----" << endl;
  // cout << "inside dfs..." << endl;
  // cout << "region: " << region << endl;
  // cout << "seen char: " << seen << endl;
  // cout << "+++grid before+++" << endl;
  // pv(grid);
  grid[start_i][start_j] = seen;
  while (!stack.empty()) {
    // Check left up right and down. If you share sides, tally and add.
    // cout << "-------" << endl;
    pll cur = stack.top();
    // cout << "cur position: " << cur << endl;
    // cout << "grid[cur.first][cur.second]=" << grid[cur.first][cur.second]
    //  << endl;
    stack.pop();
    area++;
    for (pll delta : deltas) {
      pll newpos = cur + delta;
      if (inb(grid.size(), grid[0].size(), newpos.first, newpos.second)) {
        if (grid[newpos.first][newpos.second] == region) {
          stack.push(newpos);
          grid[newpos.first][newpos.second] = seen;
          perimeter_shared_sides++;
        } else if (grid[newpos.first][newpos.second] == seen) {
          perimeter_shared_sides++;
        }
      }
    }
  }
  // cout << "##### grid after #####" << endl;
  // pv(grid);
  return {area, area * 4 - perimeter_shared_sides};
}
void solve(string file_name) {
  ifstream fin(file_name);
  vector<vector<char>> grid;
  string line;
  while (getline(fin, line)) {
    grid.push_back(vector<char>(line.begin(), line.end()));
  }

  ll output = 0;
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[i].size(); ++j) {
      if (!islower(grid[i][j])) {
        // cout << grid[i][j] << endl;
        area_perimeter ap = dfs(grid, i, j);
        // cout << "area: " << ap.area << endl;
        // cout << "perimeter: " << ap.perimeter << endl;
        output += (ap.area * ap.perimeter);
      }
    }
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
