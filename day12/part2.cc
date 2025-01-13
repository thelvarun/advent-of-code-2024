#include <cassert>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

typedef long long ll;
typedef pair<int, int> pll;

constexpr char INTERNAL_WALL = 'x';
constexpr char UNMARKED = '.';
constexpr char MARKED = '*';
pll ld = {0, -1};
pll rd = {0, 1};
pll ud = {-1, 0};
pll dd = {1, 0};
vector<pll> deltas = {ld, rd, ud, dd};

bool inb(size_t maxi, size_t maxj, pll pos) {
  return 0 <= pos.first && pos.first < maxi && 0 <= pos.second &&
         pos.second < maxj;
}

pll operator+(const pll& lhs, const pll& rhs) {
  return {lhs.first + rhs.first, lhs.second + rhs.second};
}

ostream& operator<<(ostream& o, const pll& p) {
  o << "{" << p.first << ", " << p.second << "}";
  return o;
}

struct area_sides {
  ll area = -1;
  ll sides = -1;
};

struct plot {
  char plant = '.';
  bool visited = false;
  char top_wall = UNMARKED;
  char right_wall = UNMARKED;
  char bot_wall = UNMARKED;
  char left_wall = UNMARKED;
};

ostream& operator<<(ostream& o, const plot& p) {
  o << "{ plant=" << p.plant << ", visited=" << (p.visited ? "yes" : "no")
    << ", top_wall=" << p.top_wall << ", right_wall=" << p.right_wall
    << ", bot_wall=" << p.bot_wall << ", left_wall=" << p.left_wall << "}";
  return o;
}

void pv(vector<vector<char>> grid) {
  for (auto x : grid) {
    for (auto y : x) {
      cout << y;
    }
    cout << endl;
  }
}

void pv(vector<vector<plot>>& grid) {
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[i].size(); ++j) {
      cout << (grid[i][j].visited ? (char)tolower(grid[i][j].plant)
                                  : (char)grid[i][j].plant);
    }
    cout << endl;
  }
}

bool is_outside_topleft_corner(vector<vector<plot>>& grid, size_t maxi,
                               size_t maxj, pll ij) {
  char plant = grid[ij.first][ij.second].plant;
  pll left = ij + ld;
  pll up = ij + ud;
  return ((!inb(maxi, maxj, left) ||
           grid[left.first][left.second].plant != plant) &&
          (!inb(maxi, maxj, up) || grid[up.first][up.second].plant != plant));
}

bool is_outside_topright_corner(vector<vector<plot>>& grid, size_t maxi,
                                size_t maxj, pll ij) {
  char plant = grid[ij.first][ij.second].plant;
  pll right = ij + rd;
  pll up = ij + ud;
  return ((!inb(maxi, maxj, right) ||
           grid[right.first][right.second].plant != plant) &&
          (!inb(maxi, maxj, up) || grid[up.first][up.second].plant != plant));
}

bool is_outside_botleft_corner(vector<vector<plot>>& grid, size_t maxi,
                               size_t maxj, pll ij) {
  char plant = grid[ij.first][ij.second].plant;
  pll left = ij + ld;
  pll down = ij + dd;
  return (
      (!inb(maxi, maxj, left) ||
       grid[left.first][left.second].plant != plant) &&
      (!inb(maxi, maxj, down) || grid[down.first][down.second].plant != plant));
}

bool is_outside_botright_corner(vector<vector<plot>>& grid, size_t maxi,
                                size_t maxj, pll ij) {
  char plant = grid[ij.first][ij.second].plant;
  pll right = ij + rd;
  pll down = ij + dd;
  return (
      (!inb(maxi, maxj, right) ||
       grid[right.first][right.second].plant != plant) &&
      (!inb(maxi, maxj, down) || grid[down.first][down.second].plant != plant));
}

bool is_inside_botleft_corner(vector<vector<plot>>& grid, size_t maxi,
                              size_t maxj, pll ij) {
  char plant = grid[ij.first][ij.second].plant;
  pll up = ij + ud;
  pll right = ij + rd;
  pll upright = ij + ud + rd;
  return (inb(maxi, maxj, up) && inb(maxi, maxj, right) &&
          inb(maxi, maxj, upright) &&
          grid[up.first][up.second].plant == plant &&
          grid[right.first][right.second].plant == plant &&
          grid[upright.first][upright.second].plant != plant);
}

bool is_inside_botright_corner(vector<vector<plot>>& grid, size_t maxi,
                               size_t maxj, pll ij) {
  char plant = grid[ij.first][ij.second].plant;
  pll up = ij + ud;
  pll left = ij + ld;
  pll upleft = ij + ud + ld;
  return (inb(maxi, maxj, up) && inb(maxi, maxj, left) &&
          inb(maxi, maxj, upleft) && grid[up.first][up.second].plant == plant &&
          grid[left.first][left.second].plant == plant &&
          grid[upleft.first][upleft.second].plant != plant);
}

bool is_inside_topleft_corner(vector<vector<plot>>& grid, size_t maxi,
                              size_t maxj, pll ij) {
  char plant = grid[ij.first][ij.second].plant;
  pll down = ij + dd;
  pll right = ij + rd;
  pll downright = ij + dd + rd;
  return (inb(maxi, maxj, down) && inb(maxi, maxj, right) &&
          inb(maxi, maxj, downright) &&
          grid[down.first][down.second].plant == plant &&
          grid[right.first][right.second].plant == plant &&
          grid[downright.first][downright.second].plant != plant);
}

bool is_inside_topright_corner(vector<vector<plot>>& grid, size_t maxi,
                               size_t maxj, pll ij) {
  char plant = grid[ij.first][ij.second].plant;
  pll down = ij + dd;
  pll left = ij + ld;
  pll downleft = ij + dd + ld;
  return (inb(maxi, maxj, down) && inb(maxi, maxj, left) &&
          inb(maxi, maxj, downleft) &&
          grid[down.first][down.second].plant == plant &&
          grid[left.first][left.second].plant == plant &&
          grid[downleft.first][downleft.second].plant != plant);
}

ll corners(vector<vector<plot>>& grid, size_t maxi, size_t maxj, pll ij) {
  ll count = 0;
  count += is_outside_topleft_corner(grid, maxi, maxj, ij);
  count += is_outside_topright_corner(grid, maxi, maxj, ij);
  count += is_outside_botleft_corner(grid, maxi, maxj, ij);
  count += is_outside_botright_corner(grid, maxi, maxj, ij);

  count += is_inside_topleft_corner(grid, maxi, maxj, ij);
  count += is_inside_topright_corner(grid, maxi, maxj, ij);
  count += is_inside_botleft_corner(grid, maxi, maxj, ij);
  count += is_inside_botright_corner(grid, maxi, maxj, ij);

  return count;
}

area_sides compute_area_and_sides(vector<vector<plot>>& grid, int i, int j) {
  queue<pll> q;
  q.push({i, j});
  grid[i][j].visited = true;
  // vector<pii> nodes;
  size_t maxi = grid.size();
  size_t maxj = grid[0].size();
  char plant = grid[i][j].plant;
  ll area = 0;
  ll sides = 0;
  while (!q.empty()) {
    pll curpos = q.front();
    // nodes.push_back(curpos);
    area++;
    sides += corners(grid, grid.size(), grid[0].size(), curpos);
    for (pll delta : deltas) {
      pll newpos = curpos + delta;
      if (inb(maxi, maxj, newpos) &&
          grid[newpos.first][newpos.second].plant == plant &&
          !grid[newpos.first][newpos.second].visited) {
        q.push(newpos);
        grid[newpos.first][newpos.second].visited = true;
      }
    }
    q.pop();
  }

  // Now that we have the nodes, we need to determine an edge. An new edge is
  // seen when we have a corner.
  return {area, sides};
}
void solve(string file_name) {
  ifstream fin(file_name);
  vector<vector<plot>> grid;
  vector<vector<char>> grid2;
  string line;
  while (getline(fin, line)) {
    grid2.push_back(vector<char>(line.begin(), line.end()));
    vector<plot> row;
    for (int i = 0; i < line.length(); ++i) {
      plot p;
      p.plant = line[i];
      row.push_back(p);
    }
    grid.push_back(row);
  }

  ll output = 0;
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[i].size(); ++j) {
      if (!grid[i][j].visited) {
        area_sides as = compute_area_and_sides(grid, i, j);
        output += (as.area * as.sides);
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
