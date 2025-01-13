#include <cassert>
#include <fstream>
#include <iostream>
#include <queue>
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

struct elem {
  bool visited = false;
  bool is_wall = true;
  ll dist = LLONG_MAX;
  dir facing_dir = dir::UNKNOWN;
  pii pos = {-1, -1};
  vector<pii> parents;

  string to_string() {
    return "{visited=" + (visited ? string("true") : string("false")) +
           ", is_wall=" + (is_wall ? "true" : "false") +
           ", dist=" + std::to_string(dist) + ", " + dirts(facing_dir) +
           ", pos=" + ~pos + ", " + "parents={" + pv(parents) + "}}";
  }
};

struct comp {
  bool operator()(const elem& a, const elem& b) {
    // Sorting on the basis of height(Just for example)
    return (a.dist > b.dist);
  }
};

ll find_minimum_score(vector<vector<char>>& grid, pii start_pos, pii exit_pos) {
  vector<vector<elem>> elems;
  ll total_nodes = 0;
  for (int i = 0; i < grid.size(); ++i) {
    vector<elem> row;
    for (int j = 0; j < grid[i].size(); ++j) {
      char c = grid[i][j];
      row.push_back(elem{.visited = false,
                         .is_wall = c == WALL,
                         .dist = INT_MAX,
                         .facing_dir = dir::UNKNOWN,
                         .pos = {i, j}});
      total_nodes += (c != WALL);
    }
    elems.push_back(row);
  }
  elems[start_pos.first][start_pos.second].dist = 0;
  elems[start_pos.first][start_pos.second].facing_dir = dir::RIGHT;

  size_t max_i = grid.size();
  size_t max_j = grid[0].size();
  ll nodes_visited = 0;

  while (nodes_visited < total_nodes) {
    // cout << "---" << endl;
    // cout << "nodes_visited: " << nodes_visited
    //      << ", total_nodes: " << total_nodes << endl;
    // Find unvisited node with minimum distance to start
    ll min_dist = LLONG_MAX;
    pii node_pos = {-1, -1};
    for (int i = 0; i < elems.size(); ++i) {
      for (int j = 0; j < elems[i].size(); ++j) {
        if (!elems[i][j].is_wall && !elems[i][j].visited &&
            elems[i][j].dist < min_dist) {
          node_pos = {i, j};
          min_dist = elems[i][j].dist;
        }
      }
    }

    elem& cur = elems[node_pos.first][node_pos.second];
    // cout << cur.to_string() << endl;
    cur.visited = true;
    nodes_visited++;

    pii left = cur.pos + dl;
    pii right = cur.pos + dr;
    pii up = cur.pos + du;
    pii down = cur.pos + dd;

    if (inb(left, max_i, max_j) && !elems[left.first][left.second].is_wall) {
      elem& e = elems[left.first][left.second];
      ll cost = (cur.facing_dir == dir::LEFT) ? 1 : 1001;
      if ((cur.dist + cost) < e.dist) {
        e.dist = cur.dist + cost;
        e.parents = {cur.pos};
      } else if ((cur.dist + cost) == e.dist) {
        e.parents.push_back(cur.pos);
      }
      e.facing_dir = dir::LEFT;
    }

    if (inb(right, max_i, max_j) && !elems[right.first][right.second].is_wall) {
      elem& e = elems[right.first][right.second];
      ll cost = (cur.facing_dir == dir::RIGHT) ? 1 : 1001;
      if ((cur.dist + cost) < e.dist) {
        e.dist = cur.dist + cost;
        e.parents = {cur.pos};
      } else if ((cur.dist + cost) == e.dist) {
        e.parents.push_back(cur.pos);
      }
      e.facing_dir = dir::RIGHT;
    }

    if (inb(up, max_i, max_j) && !elems[up.first][up.second].is_wall) {
      elem& e = elems[up.first][up.second];
      ll cost = (cur.facing_dir == dir::UP) ? 1 : 1001;
      if ((cur.dist + cost) < e.dist) {
        e.dist = cur.dist + cost;
        e.parents = {cur.pos};
      } else if ((cur.dist + cost) == e.dist) {
        e.parents.push_back(cur.pos);
      }
      e.facing_dir = dir::UP;
    }

    if (inb(down, max_i, max_j) && !elems[down.first][down.second].is_wall) {
      elem& e = elems[down.first][down.second];
      ll cost = (cur.facing_dir == dir::DOWN) ? 1 : 1001;
      e.dist = min(e.dist, cur.dist + cost);
      e.facing_dir = dir::DOWN;
    }
  }
  return elems[exit_pos.first][exit_pos.second].dist;
}

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
  ll output = find_minimum_score(grid, start_pos, exit_pos);
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
