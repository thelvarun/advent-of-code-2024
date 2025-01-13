#include <fstream>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>

using namespace std;

typedef long long ll;
typedef pair<int, int> pll;

vector<pll> deltas{{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

pll operator+(const pll& lhs, const pll& rhs) {
  return {lhs.first + rhs.first, lhs.second + rhs.second};
}

ostream& operator<<(ostream& o, const pll& rhs) {
  o << "{" << rhs.first << ", " << rhs.second << "}" << endl;
  return o;
}

bool inb(int maxi, int maxj, pll pos) {
  return 0 <= pos.first && pos.first < maxi && 0 <= pos.second &&
         pos.second < maxj;
}

ll get_trailhead_score(vector<vector<int>>& grid, pll trailhead_start) {
  // Implement a BFS.
  vector<vector<bool>> seen(grid.size(), vector<bool>(grid[0].size(), false));
  ll sum = 0;
  queue<pll> q;
  q.push(trailhead_start);
  while (!q.empty()) {
    pll pos = q.front();
    int cur_elem = grid[pos.first][pos.second];
    if (cur_elem == 9) {
      sum++;
      q.pop();
      continue;
    }
    // go through the four directions
    for (pll delta : deltas) {
      pll newpos = pos + delta;
      if (inb(grid.size(), grid[0].size(), newpos) &&
          !seen[newpos.first][newpos.second] &&
          grid[newpos.first][newpos.second] == (cur_elem + 1)) {
        q.push(newpos);
        seen[newpos.first][newpos.second] = true;
      }
    }
    q.pop();
  }
  return sum;
}

ll get_trailhead_rating(vector<vector<int>>& grid, pll trailhead_start) {
  // Implement a BFS.
  ll sum = 0;
  queue<pll> q;
  q.push(trailhead_start);
  while (!q.empty()) {
    pll pos = q.front();
    int cur_elem = grid[pos.first][pos.second];
    if (cur_elem == 9) {
      sum++;
      q.pop();
      continue;
    }
    // go through the four directions
    for (pll delta : deltas) {
      pll newpos = pos + delta;
      if (inb(grid.size(), grid[0].size(), newpos) &&
          grid[newpos.first][newpos.second] == (cur_elem + 1)) {
        q.push(newpos);
      }
    }
    q.pop();
  }
  return sum;
}

void solve() {
  ifstream fin("day10.in");
  vector<vector<int>> grid;
  string line;
  while (getline(fin, line)) {
    vector<int> row;
    for (char c : line) {
      row.push_back(c - '0');
    }
    grid.push_back(row);
  }

  // for (int i = 0; i < grid.size(); ++i) {
  //   for (int j = 0; j < grid[i].size(); ++j) {
  //     cout << grid[i][j];
  //   }
  //   cout << endl;
  // }

  ll total = 0;
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[0].size(); ++j) {
      if (grid[i][j] == 0) {
        total += get_trailhead_score(grid, {i, j});
      }
    }
  }
  cout << total << endl;
  return;
}

void solve2() {
  ifstream fin("day10.in");
  vector<vector<int>> grid;
  string line;
  while (getline(fin, line)) {
    vector<int> row;
    for (char c : line) {
      row.push_back(c - '0');
    }
    grid.push_back(row);
  }

  // for (int i = 0; i < grid.size(); ++i) {
  //   for (int j = 0; j < grid[i].size(); ++j) {
  //     cout << grid[i][j];
  //   }
  //   cout << endl;
  // }

  ll total = 0;
  // ll trailhead_score = get_trailhead_rating(grid, {0, 2});
  // cout << trailhead_score << endl;
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[0].size(); ++j) {
      if (grid[i][j] == 0) {
        total += get_trailhead_rating(grid, {i, j});
      }
    }
  }
  cout << total << endl;
  return;
}

int main() {
  solve2();
  return 0;
}
